#include "lost/SunEngine.h"

#include "lost/Log.h"
#include "lost/Bundle.h"
#include "lost/Bitmap.h"
#include "lost/Texture.h"
#include "lost/Context.h"
#include "lost/BufferLayout.h"
#include "lost/Mesh.h"
#include "lost/HybridIndexBuffer.h"
#include "lost/HybridVertexBuffer.h"
#include "lost/Camera2D.h"
#include "lost/TruetypeFont.h"
#include "lost/TextRender.h"
#include "lost/TextMesh.h"
#include "lost/Material.h"
#include "lost/TextBuffer.h"
#include <cmath>

#include "guiro/UserInterface.h"
#include "lost/ResourceManager.h"

#include "lost/MeshAlgo.h"
#include "lost/EventQueue.h"
#include "lost/Event.h"

#include "lost/FrameBuffer.h"
#include "lost/Quad.h"

#include "lost/Canvas.h"

using namespace std;

namespace lost
{

void SunEngine::updateSpline(const vector<Vec2>& cp, u32 numPoints, MeshPtr& triangles)
{
  uint32_t numVertices = numPoints;
  vector<Vec2> ip; // interpolated points
  vector<Vec2> nv; // tangent vectors
  ip.reserve(numVertices);
  nv.reserve(numVertices);
  
  // a minimum of 4 control points is required for the initial segment.
  // Each consecutive is made up by following point + 3 previous.
  uint32_t numSegments = ((uint32_t)cp.size()-4)+1;
  vector<uint32_t> pn; // number of points per segment
  pn.reserve(numSegments);

  // vertices spread evenly between segments
  // leftovers are attached to last segment
  uint32_t nps = numVertices / numSegments;
  uint32_t pbudget = numVertices;
  for(uint32_t i=0; i<numSegments; ++i)
  {
    if(2*nps > pbudget)
    {
      pn[i] = pbudget; // should only be the last one
    }
    else
    {
      pn[i] = nps;
      pbudget -= nps;
    }
//    DOUT("seg "<<pn[i]);
  }
  
  uint32_t pointOffset = 0;
  for(uint32_t segnum = 0; segnum < numSegments; ++segnum)
  {
    catmullRomSegment(ip, pointOffset, pn[segnum], cp, segnum);
    pointOffset += pn[segnum];
  }

  // fix normal vectors
  ASSERT(numVertices >= 2, "numVertices must be at least 2");
  for(u32 i=0; i<(numVertices-1); ++i)
  {
    Vec2 tv = ip[i+1] - ip[i+0];
    Vec2 tnv;
    tnv.x = -tv.y;
    tnv.y = tv.x;
    normalise(tnv);
    nv[i] = tnv;
  }
  nv[numVertices-1] = nv[numVertices-2];
    
  // adjust triangle mesh, only writes to points
  f32 halfWidth = splineWidth / 2;
  u32 j=0;
  
  f32 falloff = halfWidth / numVertices;
  f32 hw = halfWidth;
  for(u32 i=0; i<numVertices; i+=1)
  {
    Vec2 p = ip[i];
    Vec2 n = nv[i];
    Vec2 halfdir = (n*hw);
    Vec2 leftPoint = p+halfdir;
    Vec2 rightPoint = p-halfdir;
    triangles->set(j, UT_position, leftPoint);
    triangles->set(j+1, UT_position, rightPoint);
    
    triangles->set(j, UT_texcoord0, Vec2(0,0));
    triangles->set(j+1, UT_texcoord0, Vec2(1,0));
//    DOUT("p "<< p << " n "<<n);
//    DOUT("left "<<leftPoint<<" right "<<rightPoint );
    j+=2;
    hw -= falloff;
  }
  
}

void SunEngine::fbsetup()
{
  offscreenCanvas.reset(new Canvas(winSize));
  hblurCanvas.reset(new Canvas(winSize));
  vblurCanvas.reset(new Canvas(winSize));

  canvasQuad = Quad::create(offscreenCanvas->framebuffer->texture(0), false);
  canvasQuad->material->color = whiteColor;
  canvasQuad->material->shader = textureShader;
  canvasQuad->material->blendPremultiplied();
}

void SunEngine::startup()
{
  ResourceBundle mainBundle;
  colorShader = resourceManager->shader("resources/glsl/color");
  textureShader = resourceManager->shader("resources/glsl/texture");
  hblurShader = resourceManager->shader("resources/glsl/hblur");
  vblurShader = resourceManager->shader("resources/glsl/vblur");

  winSize = Vec2(800, 600);
  cam = Camera2D::create(Rect(0,0,winSize.width, winSize.height));
  
  
  //////////////////////////////////////////
  /// SPLINE
    
  fbsetup();
    
  Color skyBlue(.4, .8, 1, 1);
  
  mainRenderFunc = [this] ()
  {
    // offscreen pass
    offscreenCanvas->drawToCanvas(sceneRenderFunc);
    
    // horizontal blur pass
    canvasQuad->material->setTexture(0, offscreenCanvas->framebuffer->texture(0));
    canvasQuad->material->shader = vblurShader;
    hblurCanvas->drawToCanvas([this]()
    {
      glContext->draw(canvasQuad);
    });

    // vertical blur pass
    canvasQuad->material->setTexture(0, hblurCanvas->framebuffer->texture(0));
    canvasQuad->material->shader = vblurShader;
    vblurCanvas->drawToCanvas([this]()
    {
      glContext->draw(canvasQuad);
    });
    
    // onscreen pass
    glContext->bindDefaultFramebuffer();
    
    canvasQuad->material->setTexture(0, vblurCanvas->framebuffer->texture(0));
    canvasQuad->material->shader = textureShader;
    
    glContext->clearColor(blackColor);
    glContext->camera(cam);
    glContext->clear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

    sceneRenderFunc();
    glContext->draw(canvasQuad);
  };
  
  
  setupSplines();
  updateSplines();
    
  sceneRenderFunc = [this] () {
    for(auto mesh : splines)
    {
      glContext->draw(mesh);
    }
  };
}

void SunEngine::setupSplineTexture()
{
  Color splineBorderColor = Color(.39,.75,0.1,1);
  Color splineColor = Color(.24, .55, .0, 1);  
  BitmapPtr splineBitmap(new Bitmap(splineWidth+2, 1, GL_RGBA));
  splineBitmap->clear(Color(0,0,0,0));
  for(u32 i=1; i<(splineWidth-1); ++i)
  {
    splineBitmap->pixel(i, 0, splineColor);
  }
  
  f32 borderWith = floorf(splineWidth/3);
  for(u32 i=0; i<borderWith; ++i)
  {
    splineBitmap->pixel(i+1, 0, splineBorderColor);
    splineBitmap->pixel(splineWidth-1-i, 0, splineBorderColor);
  }
    
  splineBitmap->premultiplyAlpha();
  splineTexture.reset(new Texture(splineBitmap));
  splineTexture->filter(GL_LINEAR);
}

// one time setup, memory and meshes
void SunEngine::setupSplines()
{
  numInterpolatedPoints = 50;
  splineWidth = 202;
  d = 0;
  numCircles = 5;
  numSplines = 20;
  minRadius = 5;
  maxRadius = 300;
  circleCenter = Vec2(winSize.width/2, winSize.height/2);
  dotSize = 7;
  ASSERT(numCircles >= 2, "numCircles must be >= 2");
  circlePoints = new Vec2[numCircles*numSplines];
  circleRadius.resize(numCircles);

  setupSplineTexture();
  // create spline meshes
  for(u32 i=0; i<numSplines; ++i)
  {
    MeshPtr mesh = newTriangleStrip((numInterpolatedPoints*2)-2);
    mesh->material->blendPremultiplied();
    mesh->material->shader=colorShader;
    mesh->material->color = greenColor;
//    mesh->material->textures.push_back(splineTexture);
    splines.push_back(mesh);
  }  
}

// continuous update
void SunEngine::updateSplines()
{
  maxRadius = min(winSize.width, winSize.height)/2;
  circleCenter = Vec2(winSize.width/2, winSize.height/2);  
  // precalculate radii
  for (u32 i=0; i<numCircles; ++i)
  {
    f32 d = numCircles > 1 ? (((maxRadius - minRadius)/(numCircles-1))*i) : 0;
    circleRadius[i] = minRadius + d;
//    DOUT(circleRadius[i]);
  }
  
  // calculate circle points
  for (u32 ci=0; ci<numCircles; ++ci)
  {
    f32 r = circleRadius[ci];
    for(u32 cp=0; cp<numSplines; ++cp)
    {
      f32 f = ((f32)cp)/(f32(numSplines));
      f *= 2*M_PI;
      if(ci==2)
      {
        f+=o2;
      }
      else if(ci==4)
      {
        f-=o1;
      }
      else if(ci==0)
      {
        f-=o3;
      }
      f32 x = sinf(f)*r;
      f32 y = cosf(f)*r;
      Vec2 p = Vec2(x,y)+circleCenter;
      u32 idx = ci*numSplines + cp;
      circlePoints[idx] = p;
    }
  }
  
  
  // update splines
  vector<Vec2> cps;
  cps.resize(numCircles);
  for(u32 i=0; i<numSplines; ++i)
  {
    for(u32 pi = 0; pi<numCircles; ++pi)
    {
      Vec2 v = circlePoints[pi*numSplines+i];
      cps[pi] = v;
    }
    
    vector<Vec2> dv;
    dv.push_back(cps[0]);
    for(auto v : cps)
    {
      dv.push_back(v);
    }
    dv.push_back(cps.back());
    
    updateSpline(dv, numInterpolatedPoints, splines[i]);
  }  
}

void SunEngine::update()
{
  EventQueue::Container events = eventQueue->getCurrentQueue();
    
  for(Event* event : events)
  {
    if(event->base.type == ET_WindowResize)
    {
      f32 w = event->windowResizeEvent.width;
      f32 h = event->windowResizeEvent.height;
      DOUT("updating viewport "<<int(w)<<"/"<<int(h));
      winSize = Vec2(w, h);
      cam->viewport(Rect(0,0,w,h));
      fbsetup();
    }
    else if(event->base.type == ET_MouseMoveEvent)
    {
    }
  }

  d += clock.deltaUpdate;


  f32 ff = .1;

  o1 = sin(d)*ff;
  o2 = cos(d)/2*ff;
  o3 = sin(d)/3*ff;

  updateSplines();
  mainRenderFunc();
}

void SunEngine::shutdown()
{
}

}


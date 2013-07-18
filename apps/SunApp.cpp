#include "apps/SunApp.h"

#include "lost/Log.h"
#include "lost/Bitmap.h"
#include "lost/Context.h"
#include "lost/Mesh.h"
#include "lost/Camera2D.h"
#include "lost/Material.h"
#include <cmath>
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

void SunApp::updateSpline(const vector<Vec2>& cp, u32 numPoints, MeshPtr& triangles)
{
  vector<Vec2> ip; // interpolated points
  vector<Vec2> nv; // tangent vectors
  
  catmullRom(cp, numPoints, ip);
  calculateNormals(ip, nv);
  
  // adjust triangle mesh, only writes to points
  f32 halfWidth = splineWidth / 2;
  u32 j=0;
  
//  f32 falloff = halfWidth / numPoints;
  f32 hw = halfWidth;
  for(u32 i=0; i<numPoints; i+=1)
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
//    hw -= falloff;
  }
  
}

void SunApp::fbsetup()
{
  offscreenCanvas.reset(new Canvas(windowSize));
  hblurCanvas.reset(new Canvas(windowSize));
  vblurCanvas.reset(new Canvas(windowSize));

  canvasQuad = Quad::create(offscreenCanvas->framebuffer->texture(0), false);
  canvasQuad->material->color = whiteColor;
  canvasQuad->material->shader = textureShader;
  canvasQuad->material->blendPremultiplied();
}

void SunApp::startup()
{
  ResourceBundle mainBundle;
  colorShader = resourceManager->shader("resources/glsl/color");
  textureShader = resourceManager->shader("resources/glsl/texture");
  hblurShader = resourceManager->shader("resources/glsl/hblur");
  vblurShader = resourceManager->shader("resources/glsl/vblur");

  cam = Camera2D::create(Rect(0,0,windowSize.width, windowSize.height));
  
  
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
    canvasQuad->material->shader = hblurShader;
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

void SunApp::setupSplineTexture()
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
void SunApp::setupSplines()
{
  numInterpolatedPoints = 50;
  splineWidth = 12;
  d = 0;
  numCircles = 5;
  numSplines = 20;
  minRadius = 5;
  maxRadius = 300;
  circleCenter = Vec2(windowSize.width/2, windowSize.height/2);
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
    mesh->material->shader=textureShader;
    mesh->material->color = whiteColor;
    mesh->material->textures.push_back(splineTexture);
    splines.push_back(mesh);
  }  
}

// continuous update
void SunApp::updateSplines()
{
  maxRadius = min(windowSize.width, windowSize.height)/2;
  circleCenter = Vec2(windowSize.width/2, windowSize.height/2);  
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

void SunApp::update()
{
  const EventQueue::Container& events = eventQueue->getCurrentQueue();
    
  for(Event* event : events)
  {
    if(event->base.type == ET_WindowResize)
    {
      DOUT("updating viewport "<<windowSize);
      cam->viewport(Rect(0,0,windowSize));
      fbsetup();
    }
    else if(event->base.type == ET_MouseMove)
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

void SunApp::shutdown()
{
}

}


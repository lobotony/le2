#include "lost/DemoEngine.h"

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

namespace lost
{

  const uint32_t dotsize = 5;
  f32 splineWidth = 62;

void DemoEngine::updateSpline(const vector<Vec2>& cp, MeshPtr& lineMesh, MeshPtr& normalMesh, MeshPtr& triangles)
{
  uint32_t numVertices = lineMesh->numVertices();
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
  
  f32 normalLenght = 20;
  ipdots.clear();
  for(uint32_t i=0; i<numVertices; ++i)
  {
    // copy interpolated points into mesh
    lineMesh->set(i, UT_position, ip[i]);
    normalMesh->set(i*2,UT_position, ip[i]);
    
    normalMesh->set(i*2+1, UT_position, ip[i]+nv[i]*normalLenght);
    
/*    // visualize interpolated points with quads for debugging
    MeshPtr p = dot->clone();
    p->transform = MatrixTranslation(Vec3(ip[i].x-(dotsize/2), ip[i].y-(dotsize/2), 0));
    p->material = dot->material->clone();
    p->material->color = Color(1.0, 0,0,.5);
    p->material->blendNormal();
    ipdots.push_back(p);*/
  }
  
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

void DemoEngine::startup()
{
  ResourceBundle mainBundle;
  colorShader = resourceManager->shader("resources/glsl/color");
  textureShader = resourceManager->shader("resources/glsl/texture");
	ringTexture = resourceManager->texture("resources/images/rings.png");
  cam = Camera2D::create(Rect(0,0,1024,768));
  
  resourceManager->registerFontBundle("resources/fonts/vera.lefont");
  resourceManager->registerFontBundle("resources/fonts/fontawesome.lefont");
//  DataPtr fontData = mainBundle.load("resources/fonts/vera.lefont/Vera.ttf");
//  font.reset(new TruetypeFont(fontData, 18));
  font = resourceManager->font("Vera", 18);
  
  FontPtr f1 = resourceManager->font("Vera", 10);
  FontPtr f2 = resourceManager->font("Vera", 18);
  FontPtr f3 = resourceManager->font("Vera", 20);
  FontPtr f4 = resourceManager->font("Vera", 30);
  FontPtr f5 = resourceManager->font("Vera", 18);
  FontPtr fa = resourceManager->font("fontawesome", 100);
  
  font = f4;
  
  resourceManager->logStats();

  ui = new UserInterface;

  rt2.reset(new TextMesh());
  TextBuffer tb;
  tb.font(font);
  tb.width(200);
  tb.breakMode(BREAKMODE_WORD);
  tb.text("This here is a relatively long text, that's hopefully gonna be rendered in multiple lines.");
  tb.setAlign(2);
  tb.reset();
  tb.renderAllPhysicalLines(rt2);
  rt2->material->blendPremultiplied();
  rt2->material->shader = textureShader;
  rt2->transform = MatrixTranslation(Vec3(200,200,0));

  rt1 = render("I vant to drink your blood!", font, false);
  rt1->material->shader = textureShader;
  rt1->material->blendPremultiplied();
  rt1->transform = MatrixTranslation(Vec3(50,50,0));

  utf32_string utf32s;
  utf32s = 0xf085;
  rt3 = render(utf32s, fa, false);
  rt3->material->shader = textureShader;
  rt3->material->color = blackColor;
  rt3->material->blendPremultiplied();
  rt3->transform = MatrixTranslation(Vec3(600,300,0));


  coloredQuad = Quad::create(Rect(0,0,50,50));
  coloredQuad->material->shader = colorShader;
  coloredQuad->material->color = redColor;

  texturedQuad = Quad::create(ringTexture);
  texturedQuad->material->shader = textureShader;
  texturedQuad->material->color = whiteColor;
  texturedQuad->material->blendNormal();
  texturedQuad->transform = MatrixTranslation(Vec3(100,100,0));
  
  dot = Quad::create(Rect(0,0,dotsize,dotsize));
  dot->material->shader = colorShader;
  dot->material->color = greenColor;

  dot->transform = MatrixTranslation(Vec3(10,10,0));

  dot2 = dot->clone();
  dot2->transform = MatrixTranslation(Vec3(30,30,0));

  lines = newLineStrip(4);
  lines->material->shader = colorShader;
  lines->set(0, UT_position, Vec2(0,0));
  lines->set(1, UT_position, Vec2(10,10));
  lines->set(2, UT_position, Vec2(20,30));
  lines->set(3, UT_position, Vec2(40,50));

  
  //////////////////////////////////////////
  /// SPLINE
  
  u32 numInterpolatedPoints = 200;
  spline = newLineStrip(numInterpolatedPoints);
  spline->material->shader = colorShader;
  normals = newLineGroup(spline->numVertices());
  normals->material->shader = colorShader;
  
  controlPoints.push_back(Vec2(110,110));
  controlPoints.push_back(Vec2(110,110));

  controlPoints.push_back(Vec2(310,310));
  controlPoints.push_back(Vec2(610,110));
  controlPoints.push_back(Vec2(710,410));
  controlPoints.push_back(Vec2(410,310));
  controlPoints.push_back(Vec2(210,210));
  
  controlPoints.push_back(Vec2(110,410));
  controlPoints.push_back(Vec2(110,410));

  triangulatedSpline = newTriangleStrip((numInterpolatedPoints*2)-2);
  triangulatedSpline->material->blendPremultiplied();
  triangulatedSpline->material->shader=textureShader;
  
  Color splineBorderColor = Color(.39,.75,0.1,1);
  Color splineColor = Color(.24, .55, .0, 1);
  
  BitmapPtr splineBitmap(new Bitmap(splineWidth+2, 1, GL_RGBA));
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
  triangulatedSpline->material->textures.push_back(splineTexture);
  
/*  cp.push_back(Vec2(200,200));
  cp.push_back(Vec2(20,200));
  cp.push_back(Vec2(110,110));
  cp.push_back(Vec2(110,110));*/
  updateSpline(controlPoints, spline, normals, triangulatedSpline);
  
  for(uint32_t i=0; i<controlPoints.size(); ++i)
  {
    MeshPtr p = dot->clone();
    p->transform = MatrixTranslation(Vec3(controlPoints[i].x-(dotsize/2), controlPoints[i].y-(dotsize/2), 0));
    cpdots.push_back(p);
  }
  
  cp2 = controlPoints;
  d = 0;
  
  
  ////////////////////////////////////////////
  ////////////////////////////////////////////
  //////// Framebuffer
  ////////////////////////////////////////////
  ////////////////////////////////////////////

  Vec2 fbsize(512, 512);
  fb = FrameBuffer::create(fbsize, GL_RGBA);
  fb->check();
  glBindFramebuffer(GL_FRAMEBUFFER, 0); //  switch to default framebuffer again
  fbquad = Quad::create(fb->colorBuffers[0]->texture, false);
  fbquad->material->shader = textureShader;
  fbquad->material->color = whiteColor;
  
  fbcam = Camera2D::create(Rect(0,0,fbsize.width,fbsize.height));
  
}


void DemoEngine::update()
{
  EventQueue::Container events = eventQueue->getCurrentQueue();
  
  int mp = 3;
  
  for(Event* event : events)
  {
    if(event->base.type == ET_WindowResize)
    {
      f32 w = event->windowResizeEvent.width;
      f32 h = event->windowResizeEvent.height;
//      DOUT("updating viewport "<<int(w)<<"/"<<int(h));
      cam->viewport(Rect(0,0,w,h));
    }
    else if(event->base.type == ET_MouseMoveEvent)
    {
      controlPoints[mp].x = event->mouseEvent.x;
      controlPoints[mp].y = event->mouseEvent.y;
    }
  }

  d += clock.deltaUpdate;
  f32 v1 = sin(d);
  f32 v2 = cos(d);

  f32 ix = 50;
  f32 iy = 80;
  
  for(u32 i=0; i<controlPoints.size();++i)
  {
    if(i != mp)
    {
      f32 f = sin(i)+cos(4*i);
      controlPoints[i] = Vec2(cp2[i].x+v1*ix*f, cp2[i].y+v2*iy*f);
    }
  }

  updateSpline(controlPoints, spline, normals, triangulatedSpline);

  //////////////////////
  // framebuffer pass
  fb->bind();
  glContext->clearColor(Color(.3, .3, 0, 1));
  glContext->camera(fbcam);
  glContext->clear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

  glContext->draw(triangulatedSpline);

  glBindFramebuffer(GL_FRAMEBUFFER, 0); // switch to default framebuffer
  
  //////////////////////
  // screen pass


// light blue  glContext->clearColor(Color(.45, .84, 1, 1));
//  glContext->clearColor(Color(.3, .3, 0, 1));
  glContext->clearColor(Color(0, 0, 0, 1));
  glContext->camera(cam);
  glContext->clear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

//  glContext->draw(coloredQuad);
//  glContext->draw(texturedQuad);
//  glContext->draw(rt1);
//  glContext->draw(rt2);
//  glContext->draw(rt3);
//  glContext->draw(dot);
//  glContext->draw(dot2);
//  glContext->draw(lines);
//  glContext->draw(spline);
//  glContext->draw(normals);

//  glContext->draw(triangulatedSpline);

    glContext->draw(fbquad);

/*  for(uint32_t i=0; i<ipdots.size(); ++i)
  {
    glContext->draw(ipdots[i]);
  }
  
  for(uint32_t i=0; i<cpdots.size(); ++i)
  {
    glContext->draw(cpdots[i]);
  }*/
  
  ui->update();
  ui->draw(glContext);
}

void DemoEngine::shutdown()
{
}

}


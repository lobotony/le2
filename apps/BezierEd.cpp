//
//  BezierEd.cpp
//  LostEngine2
//
//  Created by Tony Kostanjsek on 19.07.13.
//  Copyright (c) 2013 Tony Kostanjsek. All rights reserved.
//

#include "BezierEd.h"
#include "lost/UserInterface.h"
#include "lost/layers/Layer.h"
#include "lost/layers/TextLayer.h"
#include "lost/Context.h"
#include "lost/views/View.h"
#include "lost/ResourceManager.h"
#include "lost/Camera2D.h"
#include "lost/MeshAlgo.h"
#include "lost/Mesh.h"
#include "lost/Material.h"
#include "lost/Bitmap.h"
#include "lost/Texture.h"
#include "lost/TimingFunction.h"

namespace lost
{

BezierEd::BezierEd() : Application("resources/bezierEdConfig.json")
{
}

ViewPtr BezierEd::pointView(f32 x, f32 y)
{
  ViewPtr result(new View);
  
  result->layer->backgroundColor(greenColor);
  result->rect(x, y, psize, psize);
  result->name("pointView");
  result->addEventHandler(ET_MouseDown, downHandler, EP_Target);
  result->addEventHandler(ET_MouseUp, upHandler, EP_Target);  
  
  return result;
}

TextLayerPtr BezierEd::label(f32 x, f32 y)
{
  TextLayerPtr result(new TextLayer);
  
  result->name = "text";
  result->font(font);
  result->text("test");
  result->textColor(whiteColor);
  result->backgroundColor(clearColor);
  result->rect(x, y, labelWidth, labelHeight);
  
  return result;
}

void BezierEd::pointsToViews()
{
  f32 h = floorf(psize/2);
  Vec2 hp(h,h);
  for(u32 i=0; i<4; ++i)
  {
    Vec2 pos = offset+Vec2(areaSize.x*p[i].x, areaSize.y*p[i].y);
    pv[i]->pos(pos-hp);
  }
}

void BezierEd::updateSpline()
{
  f32 dt = 1.0f/(numInterpolatedPoints-1);
  for(u32 i=0; i<numInterpolatedPoints; ++i)
  {
    f32 t = i*dt;
    ip[i] = bezierInterpolate(t, p[0], p[1], p[2], p[3]);
    ip[i].x *= areaSize.x;
    ip[i].y *= areaSize.y;
    ip[i] += offset;
  }
  
  calculateNormals(ip, nv);
  
  // adjust triangle mesh, only writes to points
  f32 halfWidth = splineWidth / 2;
  u32 j=0;
  
  f32 hw = halfWidth;
  for(u32 i=0; i<numInterpolatedPoints; i+=1)
  {
    Vec2 p = ip[i];
    Vec2 n = nv[i];
    Vec2 halfdir = (n*hw);
    Vec2 leftPoint = p+halfdir;
    Vec2 rightPoint = p-halfdir;
    bezier->set(j, UT_position, leftPoint);
    bezier->set(j+1, UT_position, rightPoint);
    
    bezier->set(j, UT_texcoord0, Vec2(0,0));
    bezier->set(j+1, UT_texcoord0, Vec2(1,0));
    j+=2;
  }
  
}

std::string string_format(const std::string fmt, ...) {
    int size = 100;
    std::string str;
    va_list ap;
    while (1) {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {
            str.resize(n);
            return str;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    }
    return str;
}

void BezierEd::updateLabels()
{
  u32 i=0;
  for(TextLayerPtr tl : labels)
  {
    tl->text(string_format("(%.2f/%.2f) ", p[i].x, p[i].y));
    ++i;
  }
}

u32 BezierEd::indexFromView(View* v)
{
  u32 result = 0;
  for(ViewPtr view : pv)
  {
    if(view.get() == v)
    {
      break;
    }
    ++result;
  }
  return result;
}

void BezierEd::setupLabels()
{
  resourceManager->registerFontBundle("resources/fonts/vera.lefont");
  font = resourceManager->font("Vera", 30);

  labelWidth = floorf(areaSize.x / 4);
  labelHeight = 40;
  
  f32 yspacing = 10;
  
  labels.push_back(label(offset.x, areaSize.y+offset.y+yspacing));
  labels.push_back(label(offset.x+labelWidth, areaSize.y+offset.y+yspacing));
  labels.push_back(label(offset.x+2*labelWidth, areaSize.y+offset.y+yspacing));
  labels.push_back(label(offset.x+3*labelWidth, areaSize.y+offset.y+yspacing));

  for(TextLayerPtr l : labels)
  {
    ui->rootView->layer->addSublayer(l);
  }
}

void BezierEd::setupSplineTexture()
{
  Color splineColor = whiteColor; //Color(.24, .55, .0, 1);
  BitmapPtr splineBitmap(new Bitmap(splineWidth+2, 1, GL_RGBA));
  splineBitmap->clear(Color(0,0,0,0));
  for(u32 i=1; i<(splineWidth-1); ++i)
  {
    splineBitmap->pixel(i, 0, splineColor);
  }
  
  splineBitmap->premultiplyAlpha();
  splineTexture.reset(new Texture(splineBitmap));
  splineTexture->filter(GL_LINEAR);
}


void BezierEd::startup()
{
  areaSize.x = config["areaWidth"].asInt();
  areaSize.y = config["areaHeight"].asInt();
  psize = config["pointSize"].asInt();
  offset.x = config["offsetX"].asInt();
  offset.y = config["offsetY"].asInt();
  ui->enable();

  p.resize(4);
  p[0] = Vec2(0,0);
  p[1] = Vec2(.25,.3);
  p[2] = Vec2(.7,.55);
  p[3] = Vec2(1,1);
  
  downHandler = [this](Event* event)
  {
    clickedView = event->base.target;
//    DOUT(event->mouseEvent.x << " "<<event->mouseEvent.y);
  };

  moveHandler = [this](Event* event)
  {
    if(clickedView)
    {
      u32 idx = indexFromView(clickedView);
      if((idx==0) || (idx==3))
      {
      }
      else
      {
        f32 x = event->mouseEvent.x;
        f32 y = event->mouseEvent.y;
        
        x = std::min(x,areaSize.x+offset.x);
        x = std::max(x,offset.x);
        f32 px = (x - offset.x)/areaSize.x;
        x = floorf(x-(psize/2));
        y = std::min(y,areaSize.y+offset.y);
        y = std::max(y,offset.y);
        f32 py = (y - offset.y)/areaSize.y;
        y = floorf(y-(psize/2));
        
        clickedView->pos(Vec2(x, y));
        
        p[idx] = Vec2(px, py);
        
        updateSpline();
        updateLabels();
      }
    }
  };

  upHandler = [this](Event* event)
  {
    clickedView = NULL;
//    DOUT(eventTypeToString(event->base.type)<<" "<<eventPhaseToString(event->base.phase));
  };
  
  for(u32 i=0; i<4; ++i)
  {
    ViewPtr v = pointView(10,10);
    ui->rootView->addSubview(v);
    pv.push_back(v);
  }
  
  
  ui->rootView->addEventHandler(ET_MouseMove, moveHandler, EP_Target);
  ui->rootView->addEventHandler(ET_MouseMove, moveHandler, EP_Bubble);
  
  pointsToViews();

  colorShader = resourceManager->shader("resources/glsl/color");
  textureShader = resourceManager->shader("resources/glsl/texture");
  cam = Camera2D::create(Rect(0,0,windowSize.width, windowSize.height));

  numInterpolatedPoints = 50;
  splineWidth = 12;

  ip.resize(numInterpolatedPoints);
  nv.resize(numInterpolatedPoints);

  bezier = newTriangleStrip((numInterpolatedPoints*2)-2);
  bezier->material->blendPremultiplied();
  bezier->material->shader=textureShader;
  bezier->material->color = whiteColor;
  setupSplineTexture();
  bezier->material->textures.push_back(splineTexture);
  
  setupLabels();
  
  updateSpline();
  updateLabels();
  
  TimingFunction tf = TimingFunction::linear();
  u32 steps = 30;
  for(u32 i=0; i<steps; ++i)
  {
    f32 x = (1.0f/(steps-1))*i;
    f32 y = tf.yAtX(x);
    DOUT(i << " = ("<<x<<"/"<<y<<")");
  }
}

void BezierEd::update()
{
  glContext->clearColor(blackColor);
  glContext->clear(GL_COLOR_BUFFER_BIT);
  glContext->camera(cam);
  glContext->draw(bezier);
}

void BezierEd::shutdown()
{
}

}


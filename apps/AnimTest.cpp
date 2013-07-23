#include "apps/AnimTest.h"
#include "lost/layers/Layer.h"
#include "lost/UserInterface.h"
#include "lost/Context.h"
#include "lost/Animation.h"

namespace lost
{

AnimTest::AnimTest() : Application("resources/animTestConfig.json")
{
}

LayerPtr layer(f32 x, f32 y, f32 w, f32 h)
{
  LayerPtr result(new Layer);
  
  result->backgroundColor(whiteColor);
  result->cornerRadius(12);
  result->rect(x, y, w, h);
  
  Variant v = result->getValue("opacity");
  v = v * .8;
  result->setValue("opacity", v);
  
  return result;
}

void AnimTest::startup()
{
  ui->enable();

  f32 offset = 20;
  f32 spacing = 4;
  Vec2 sz(100,100);
//  Vec2 num(23,14);
  Vec2 num(10,7);
//  Vec2 num(1,1);
  
  for(u32 y=0; y<num.y; ++y)
  {
    for(u32 x=0; x<num.x; ++x)
    {
      LayerPtr l = layer(offset+x*(sz.width+spacing),offset+y*(sz.height+spacing),sz.width, sz.height);
      layers.push_back(l);
      ui->rootView->layer->addSublayer(l);
    }
  }
  
  #define SZDOUT(c) DOUT("sizeof("<<#c<<") = "<<u64(sizeof(c)));
  SZDOUT(Animation);
  
  Animation a1;
  DOUT("periodLength: "<<a1.periodLength());
  DOUT("totalDuration: "<<a1.totalDuration());
  a1.repeatCount = 2;
  DOUT("periodLength: "<<a1.periodLength());
  DOUT("totalDuration: "<<a1.totalDuration());
  a1.repeatCount = 0;
  DOUT("periodLength: "<<a1.periodLength());
  DOUT("totalDuration: "<<a1.totalDuration());
  a1.autoreverses = true;
  DOUT("periodLength: "<<a1.periodLength());
  DOUT("totalDuration: "<<a1.totalDuration());
  
  f32 duration = 3;
  f32 speed = 1;
  f32 repeatCount = 100;

  AnimationPtr fadeIn(new Animation);
  fadeIn->key = "opacity";
  fadeIn->beginTime = currentTimeSeconds();
  fadeIn->duration = duration;
  fadeIn->speed = speed;
  fadeIn->startValue = Variant(f32(0));
  fadeIn->endValue = Variant(f32(1));
  fadeIn->autoreverses = true;
  fadeIn->repeatCount = repeatCount;
  
  AnimationPtr resize(new Animation);
  resize->key = "size";
  resize->beginTime = fadeIn->beginTime;
  resize->duration = duration;
  resize->speed = speed;
  resize->startValue = Variant(Vec2(50,50));
  resize->endValue = Variant(Vec2(600,176));
  resize->autoreverses = true;
  resize->repeatCount = repeatCount;
  resize->timeOffset = 0;
  
  layers[0]->addAnimation(fadeIn->key, fadeIn);
  layers[0]->addAnimation(resize->key, resize);
}

void AnimTest::update()
{
  glContext->clearColor(blackColor);
  glContext->clear(GL_COLOR_BUFFER_BIT);
}

void AnimTest::shutdown()
{
}

}
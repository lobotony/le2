#ifndef GUIRO_RENDERSYSTEM_H
#define GUIRO_RENDERSYSTEM_H

#include "guiro/views/View.h"
#include "lost/Context.h"

namespace lost
{
struct RenderSystem
{
  RenderSystem();
  ~RenderSystem();
  
  void draw(Context* glContext, View* rootView);
};
}

#endif


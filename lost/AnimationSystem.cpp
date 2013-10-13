//
//  AnimationSystem.cpp
//  LostEngine2
//
//  Created by Tony Kostanjsek on 19.07.13.
//  Copyright (c) 2013 Tony Kostanjsek. All rights reserved.
//

#include "AnimationSystem.h"
#include "lost/PlatformTime.h"
#include "lost/layers/Layer.h"

namespace lost
{

void AnimationSystem::update()
{
  TimeInterval now = currentTimeSeconds();
  for(Layer* layer : layers)
  {
    if(layer->hasAnimations())
    {
      layer->updateAnimations(now);
    }
    else
    {
      removeLayers.push_back(layer);
    }
  }
  
  // remove Layers that have no animations anymore
  for(Layer* layer : removeLayers)
  {
    auto pos = find(layers.begin(), layers.end(), layer);
    if(pos != layers.end())
    {
      layers.erase(pos);
    }
  }
  removeLayers.clear();
}

void AnimationSystem::startAnimating(Layer* layer)
{
  auto pos = find(layers.begin(), layers.end(), layer);
  if(pos == layers.end())
  {
    layers.push_back(layer);
  }
}

void AnimationSystem::stopAnimating(Layer* layer)
{
  auto pos = find(layers.begin(), layers.end(), layer);
  if(pos != layers.end())
  {
    layers.erase(pos);
  }
}

}


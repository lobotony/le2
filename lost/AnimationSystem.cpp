//
//  AnimationSystem.cpp
//  LostEngine2
//
//  Created by Tony Kostanjsek on 19.07.13.
//  Copyright (c) 2013 Tony Kostanjsek. All rights reserved.
//

#include "AnimationSystem.h"

namespace lost
{

void AnimationSystem::reset()
{
  layers.clear();
}

void AnimationSystem::update()
{
  for(Layer* layer : layers)
  {
    
  }
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


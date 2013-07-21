//
//  AnimationSystem.h
//  LostEngine2
//
//  Created by Tony Kostanjsek on 19.07.13.
//  Copyright (c) 2013 Tony Kostanjsek. All rights reserved.
//

#ifndef __LostEngine2__AnimationSystem__
#define __LostEngine2__AnimationSystem__

namespace lost
{
struct AnimationSystem
{

  void reset();
  void update();
  void startAnimating(Layer* layer);
  void stopAnimating(Layer* layer);

  vector<Layer*> layers;
};
}

#endif /* defined(__LostEngine2__AnimationSystem__) */

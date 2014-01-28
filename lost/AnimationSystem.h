#pragma once

namespace lost
{
struct AnimationSystem
{

  void update();
  void startAnimating(Layer* layer);
  void stopAnimating(Layer* layer);

  vector<Layer*> layers;
  vector<Layer*> removeLayers;
};
}

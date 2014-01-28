#pragma once

#include "lost/views/View.h"

namespace lost
{
struct ImageView : public View
{
  ImageView();
  ImageView(const ImagePtr& v);
  virtual ~ImageView();
  
  void image(const ImagePtr& v);
  ImagePtr image() const;
  
  void images(const vector<ImagePtr>& iv, f32 frameTime);
  
  bool resizeOnImageChange; // if true, the view will update it's
  
  void startAnimation();
  void stopAnimation();
  bool isAnimating();
  void animate(); // don't call directly, will be called by frame update
  
private:
  void init();
  vector<ImagePtr>  _images;
  f32               _frameTime;
  u32               _currentFrame;
  Clock             clock;
  bool              animating;
};
}



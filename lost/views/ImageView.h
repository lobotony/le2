#ifndef LOST_IMAGEVIEW_H
#define LOST_IMAGEVIEW_H

namespace lost
{
struct ImageView : public View
{
  ImageView();
  virtual ~ImageView();
  
  void texture(const TexturePtr& v);
  TexturePtr texture();
  
  void startAnimation();
  void stopAnimation();
  
private:
  
};
}

#endif


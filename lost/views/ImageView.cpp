#include "lost/views/ImageView.h"
#include "lost/Application.h"
#include "lost/UserInterface.h"

namespace lost
{

ImageView::ImageView()
{
  init();
}

ImageView::ImageView(const ImagePtr& v)
{
  init();
  image(v);
}

void ImageView::init()
{
  name("ImageView");
  resizeOnImageChange = true;
  layer->composite(false);
  _frameTime = 0.0f;
  _currentFrame = 0;
  animating = false;
}

ImageView::~ImageView()
{
  stopAnimation();
}

void ImageView::image(const ImagePtr& v)
{
  layer->backgroundImage(v);
  if(resizeOnImageChange)
  {
    size(v->size);
  }
}
ImagePtr ImageView::image() const { return layer->backgroundImage(); }

void ImageView::images(const vector<ImagePtr>& iv, f32 frameTime)
{
  stopAnimation();
  _images = iv;
  _frameTime = frameTime;
  resizeOnImageChange = false;
  _currentFrame = 0;
  if(_images.size() > 0)
  {
    image(_images[_currentFrame]);
  }
}

void ImageView::startAnimation()
{
  if(!animating)
  {
    animating = true;
    clock.reset();
    Application::instance()->ui->addFrameUpdater(this, [this](){
      animate();
    });
  }
}

void ImageView::stopAnimation()
{
  if(animating)
  {
    Application::instance()->ui->removeFrameUpdater(this);
    animating = false;
    _currentFrame = 0;
    image(_images[0]);
  }
}

bool ImageView::isAnimating()
{
  return animating;
}

void ImageView::animate()
{
  clock.update();
  TimeInterval ds = clock.deltaStart;
  u32 frameNum = u32(ds / _frameTime) % _images.size();
  if(frameNum != _currentFrame)
  {
    DOUT(frameNum);
    _currentFrame = frameNum;
    image(_images[frameNum]);
  }
}

}



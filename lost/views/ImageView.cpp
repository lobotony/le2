#include "lost/views/ImageView.h"

namespace lost
{

ImageView::ImageView()
{
}

ImageView::ImageView(const ImagePtr& v)
{
  image(v);
  size(v->size);
}

void ImageView::image(const ImagePtr& v) { layer->backgroundImage(v); }
ImagePtr ImageView::image() const { return layer->backgroundImage(); }

}


#ifndef LOST_IMAGEVIEW_H
#define LOST_IMAGEVIEW_H

#include "lost/views/View.h"

namespace lost
{
struct ImageView : public View
{
  ImageView();
  ImageView(const ImagePtr& v);
  
  void image(const ImagePtr& v);
  ImagePtr image() const;
  
  bool resizeOnImageChange; // if true, the view will update it's 
};
}

#endif


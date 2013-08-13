#ifndef LOST_BUTTON_H
#define LOST_BUTTON_H

#include "lost/views/View.h"

namespace lost
{

enum ButtonState
{
  ButtonStateReleased = 0,
  ButtonStateReleasedHover,
  ButtonStatePressed,
  ButtonStatePressedHover
};

struct Button : public View
{
  Button();
  virtual ~Button();
  
  using View::rect;
  void rect(const Rect& r);
  void rect(f32 x, f32 y, f32 w, f32 h);
  
  LabelPtr titleLabel;
  ImageViewPtr imageView;
  
  ButtonState state() const;
  void state(ButtonState v);

  void title(const string& v, ButtonState s);
  void titleColor(const Color& v, ButtonState s);
  void background(const ImagePtr& v, ButtonState s);
  void image(const ImagePtr& v, ButtonState s);

private:
  void updateFromState();
  void mouseDown(Event* event);
  void mouseUpInside(Event* event);
  void mouseUpOutside(Event* event);
  
  map<ButtonState, string>    state2title;
  map<ButtonState, Color>     state2titleColor;
  map<ButtonState, ImagePtr>  state2background;
  map<ButtonState, ImagePtr>  state2image;
  
  ButtonState _state;
};

}

#endif

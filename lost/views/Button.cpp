#include "lost/views/Button.h"
#include "lost/views/Label.h"
#include "lost/views/ImageView.h"

namespace lost
{

Button::Button()
{
  imageView.reset(new ImageView);
  imageView->name("buttonBackground");
  addSubview(imageView);

  titleLabel.reset(new Label);
  titleLabel->backgroundColor(clearColor);
  titleLabel->name("buttonTitle");
  addSubview(titleLabel);

  backgroundColor(clearColor);

  _state = ButtonStateReleased;
  updateFromState();
  
  addEventHandler(ET_MouseDown, [this](Event* event) { mouseDown(event); }, EP_Bubble);
  addEventHandler(ET_MouseUpInside  , [this](Event* event) { mouseUpInside(event); }, EP_Bubble);
  addEventHandler(ET_MouseUpOutside, [this](Event* event) { mouseUpOutside(event); }, EP_Bubble);
  
  name("Button");
}

Button::~Button()
{
}

void Button::mouseDown(Event* event)
{
  DOUT("");
  state(ButtonStatePressed);
}

void Button::mouseUpInside(Event* event)
{
  DOUT("");
  state(ButtonStateReleased);
}

void Button::mouseUpOutside(Event* event)
{
  DOUT("");
  state(ButtonStateReleased);
}

void Button::rect(const Rect& r)
{
  View::rect(r);
  Rect svr(0,0,r.width, r.height);
  imageView->rect(svr);
  titleLabel->rect(svr);
}

void Button::rect(f32 x, f32 y, f32 w, f32 h)
{
  View::rect(x,y,w,h);
  Rect svr(0,0,w,h);
  imageView->rect(svr);
  titleLabel->rect(svr);
}

ButtonState Button::state() const
{
  return _state;
}

void Button::state(ButtonState v)
{
  _state = v;
  updateFromState();
}

void Button::title(const string& v, ButtonState s) { state2title[s] = v; updateFromState(); }
void Button::titleColor(const Color& v, ButtonState s) { state2titleColor[s] = v; updateFromState();  }
void Button::background(const ImagePtr& v, ButtonState s) { state2background[s] = v; updateFromState();  }
void Button::image(const ImagePtr& v, ButtonState s) { state2image[s] = v; updateFromState();  }

void Button::updateFromState()
{
  auto bg = state2background.find(_state);
  if(bg != state2background.end())
  {
    imageView->image(bg->second);
  }
  
  auto txt = state2title.find(_state);
  if(txt != state2title.end())
  {
    titleLabel->text(txt->second);
  }

  auto txtcol = state2titleColor.find(_state);
  if(txtcol != state2titleColor.end())
  {
    titleLabel->textColor(txtcol->second);
  }
  layer->needsRedraw();
}

}

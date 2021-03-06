#include "lost/views/Button.h"
#include "lost/views/Label.h"
#include "lost/views/ImageView.h"

namespace lost
{

Button::Button()
{
  imageView.reset(new ImageView);
  imageView->name("buttonIcon");
  imageView->resizeOnImageChange = false;
  imageView->composite(false);
  imageView->backgroundColor(whiteColor);
  addSubview(imageView);

  titleLabel.reset(new Label);
  titleLabel->backgroundColor(clearColor);
  titleLabel->name("buttonTitle");
  titleLabel->composite(false);
  addSubview(titleLabel);

  _state = ButtonStateReleased;
  updateFromState();
  
  addEventHandler(ET_MouseDown, [this](Event* event) { mouseDown(event); }, EP_Bubble);
  addEventHandler(ET_MouseUpInside  , [this](Event* event) { mouseUpInside(event); }, EP_Bubble);
  addEventHandler(ET_MouseUpOutside, [this](Event* event) { mouseUpOutside(event); }, EP_Bubble);
  
  name("Button");
  layer->composite(true);
}

Button::~Button()
{
}

void Button::mouseDown(Event* event)
{
  state(ButtonStatePressed);
}

void Button::mouseUpInside(Event* event)
{
  state(ButtonStateReleased);
  triggerActions();
}

void Button::mouseUpOutside(Event* event)
{
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
    layer->backgroundImage(bg->second);
  }
  
  auto img = state2image.find(_state);
  if(img != state2image.end())
  {
    imageView->visible(true);
    imageView->layer->backgroundContentMode(lost::LayerContentModeCenter);
    imageView->image(img->second);
  }
  else
  {
    imageView->visible(false);
  }
  
  auto txt = state2title.find(_state);
  if(txt != state2title.end())
  {
    titleLabel->visible(true);
    titleLabel->text(txt->second);

    auto txtcol = state2titleColor.find(_state);
    if(txtcol != state2titleColor.end())
    {
      titleLabel->textColor(txtcol->second);
    }
  }
  else
  {
    titleLabel->visible(false);
  }

  layer->needsRedraw();
}

u32 Button::addAction(Action action)
{
  actions.push_back(action);
  return u32(actions.size()-1);
}

void Button::removeAction(u32 index)
{
  actions.erase(actions.begin()+index);
}

void Button::removeAllActions()
{
  actions.clear();
}

void Button::triggerActions()
{
  for(auto& action : actions)
  {
    action(this);
  }
}

}

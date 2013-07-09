#ifndef LOST_TEXTLAYER_H
#define LOST_TEXTLAYER_H

#include "lost/layers/Layer.h"

namespace lost
{

struct TextLayer : public Layer
{
  string text() const;
  void text(const string& v);
  
  FontPtr font() const;
  void font(const FontPtr& v);

  Color textColor() const;
  void textColor(const Color& v);
  
  string description();
  
  virtual void draw(DrawContext* rc);

private:
  string  _text;
  FontPtr _font;
  Color   _textColor;
};

}


#endif


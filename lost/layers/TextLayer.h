#pragma once

#include "lost/layers/Layer.h"

namespace lost
{

struct TextLayer : public Layer
{
  TextLayer();

  string text() const;
  void text(const string& v);
  
  FontPtr font() const;
  void font(const FontPtr& v);

  Color textColor() const;
  void textColor(const Color& v);
  
  Insets insets() const;
  void insets(const Insets& v);
  
  TextAlignment alignment() const;
  void alignment(TextAlignment v);
  
  BreakMode breakmode() const;
  void breakmode(BreakMode v);
  
  string description();
  
  virtual void draw(DrawContext* rc);

private:
  string        _text;
  FontPtr       _font;
  Color         _textColor;
  Insets        _insets;
  TextAlignment _alignment;
  BreakMode     _breakmode;
};

}




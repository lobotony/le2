#ifndef LOST_LABEL_H
#define LOST_LABEL_H

#include "lost/views/View.h"

namespace lost
{

struct Label : public View
{
  Label();
  virtual ~Label();

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

  using View::rect;
  void rect(const Rect& r);
  void rect(f32 x, f32 y, f32 w, f32 h);
  
  TextLayerPtr textLayer;
};

}

#endif

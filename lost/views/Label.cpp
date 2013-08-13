#include "lost/views/Label.h"
#include "lost/layers/TextLayer.h"

namespace lost
{
Label::Label()
{
  textLayer.reset(new TextLayer);
  layer->addSublayer(textLayer);
  name("Label");
}

Label::~Label()
{
}

void Label::rect(const Rect& r)
{
  View::rect(r);
  textLayer->rect(0, 0, r.width, r.height);
}

void Label::rect(f32 x, f32 y, f32 w, f32 h)
{
  View::rect(x,y,w,h);
  textLayer->rect(0,0,w,h);
}

#pragma mark - text attribute accessors -

string Label::text() const { return textLayer->text(); }
void Label::text(const string& v) { textLayer->text(v); }

FontPtr Label::font() const { return textLayer->font(); }
void Label::font(const FontPtr& v) { textLayer->font(v); }

Color Label::textColor() const { return textLayer->textColor(); }
void Label::textColor(const Color& v) { textLayer->textColor(v); }

Insets Label::insets() const { return textLayer->insets(); }
void Label::insets(const Insets& v) { textLayer->insets(v); }

TextAlignment Label::alignment() const { return textLayer->alignment(); }
void Label::alignment(TextAlignment v) { textLayer->alignment(v); }

BreakMode Label::breakmode() const { return textLayer->breakmode(); }
void Label::breakmode(BreakMode v) { textLayer->breakmode(v); }


}

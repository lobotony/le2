#include "lost/layers/TextLayer.h"
#include "lost/DrawContext.h"

namespace lost
{
TextLayer::TextLayer()
{
  _textColor = blackColor;
  _alignment = TextAlignmentCenter;
  _breakmode = BREAKMODE_WORD;
}

string TextLayer::text() const { return _text; }
void TextLayer::text(const string& v) { _text = v; needsRedraw(); }

FontPtr TextLayer::font() const { return _font; }
void TextLayer::font(const FontPtr& v) { _font = v; needsRedraw(); }

Color TextLayer::textColor() const { return _textColor; }
void TextLayer::textColor(const Color& v) { _textColor = v; needsRedraw(); }

Insets TextLayer::insets() const { return _insets; }
void TextLayer::insets(const Insets& v) { _insets = v; needsRedraw(); }

TextAlignment TextLayer::alignment() const { return _alignment; }
void TextLayer::alignment(TextAlignment v) { _alignment = v; needsRedraw(); }

BreakMode TextLayer::breakmode() const { return _breakmode; }
void TextLayer::breakmode(BreakMode v) { _breakmode = v; needsRedraw(); }

string TextLayer::description()
{
  StringStream os;
  os << "[TextLayer "<<(u64)this<<" '"<<name<<"']";
  return os.str();
}

void TextLayer::draw(DrawContext* rc)
{
  Layer::draw(rc);
  if(_font)
  {
    Rect cr = rect();
    Rect r(0,0,cr.width, cr.height);
    r.applyInsets(_insets);
    rc->drawText(_text, _font, _textColor, r, Vec2(_insets.l,0), _alignment, _breakmode);
  }
}

}


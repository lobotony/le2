#include "lost/layers/TextLayer.h"
#include "lost/DrawContext.h"

namespace lost
{

string TextLayer::text() const { return _text; }
void TextLayer::text(const string& v) { _text = v; needsRedraw(); }

FontPtr TextLayer::font() const { return _font; }
void TextLayer::font(const FontPtr& v) { _font = v; needsRedraw(); }

Color TextLayer::textColor() const { return _textColor; }
void TextLayer::textColor(const Color& v) { _textColor = v; needsRedraw(); }

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
    rc->drawText(_text, _font, _textColor, Vec2(0,10), 0);
  }
}

}


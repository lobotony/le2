#pragma once

namespace lost
{
struct Insets
{
  Insets() : t(0), l(0), b(0), r(0) {}
  Insets(f32 v) : t(v), l(v), b(v), r(v) {}
  Insets(f32 it, f32 il, f32 ib, f32 ir) : t(it), l(il), b(ib), r(ir) {}

  f32 t,l,b,r;
};
}



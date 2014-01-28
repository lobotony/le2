#pragma once

namespace lost
{

enum VariantType
{
  VT_undefined=0,
  VT_int,
  VT_float,
  VT_bool,
  VT_color,
  VT_vec2,
  VT_vec3,
  VT_vec4,
  VT_matrix
};

struct Variant
{
  
  Variant() : type(VT_undefined) {};
  Variant(GLint v) : i(v), type(VT_int) {};
  Variant(f32 v) : f(v), type(VT_float) {};
  Variant(bool v) : b(v), type(VT_bool) {};
  Variant(const Color& v) : color(v), type(VT_color) {};
  Variant(const Vec2& v) : vec2(v), type(VT_vec2) {};
  Variant(const Vec3& v) : vec3(v), type(VT_vec3) {};
  Variant(const Vec4& v) : vec4(v), type(VT_vec4) {};
  Variant(const Matrix& v) : matrix(v), type(VT_matrix) {};
  
  // can't use union when members have constructors
  GLint   i;
  f32     f;
  bool    b;
  Color   color;
  Vec2    vec2;
  Vec3    vec3;
  Vec4    vec4;
  Matrix  matrix;
  
  VariantType type;
};

inline Variant operator*(const Variant& lhs, f32 rhs)
{
  switch(lhs.type)
  {
    case VT_color: return Variant(lhs.color*rhs);break;
    case VT_int: return Variant(GLint(lhs.i*rhs));break;
    case VT_float: return Variant(f32(lhs.f*rhs));break;
    case VT_vec2: return Variant(lhs.vec2*rhs);break;
    case VT_vec3: return Variant(lhs.vec3*rhs);break;
    case VT_vec4: return Variant(lhs.vec4*rhs);break;
    case VT_matrix: return Variant(lhs.matrix*rhs);break;
    default:
      ASSERT(false, "can't multiply variant of type "<<lhs.type<<" with float");
      return Variant();
  }
}

inline Variant operator+(const Variant& lhs, const Variant& rhs)
{
  switch(lhs.type)
  {
    case VT_color: return Variant(lhs.color+rhs.color);break;
    case VT_int: return Variant(GLint(lhs.i+rhs.i));break;
    case VT_float: return Variant(f32(lhs.f+rhs.f));break;
    case VT_vec2: return Variant(lhs.vec2+rhs.vec2);break;
    case VT_vec3: return Variant(lhs.vec3+rhs.vec3);break;
    case VT_vec4: return Variant(lhs.vec4+rhs.vec4);break;
    default:
      ASSERT(false, "can't add");
      return Variant();
  }
}

inline Variant operator-(const Variant& lhs, const Variant& rhs)
{
  switch(lhs.type)
  {
    case VT_color: return Variant(lhs.color-rhs.color);break;
    case VT_int: return Variant(GLint(lhs.i-rhs.i));break;
    case VT_float: return Variant(f32(lhs.f-rhs.f));break;
    case VT_vec2: return Variant(lhs.vec2-rhs.vec2);break;
    case VT_vec3: return Variant(lhs.vec3-rhs.vec3);break;
    case VT_vec4: return Variant(lhs.vec4-rhs.vec4);break;
    default:
      ASSERT(false, "can't subtract");
      return Variant();
  }
}


}


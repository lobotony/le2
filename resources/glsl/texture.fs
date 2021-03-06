varying vec2 vtexcoord0;
uniform vec4 color; // same semantics as gl_Color
uniform sampler2D texture0;

void main(void)
{
  gl_FragColor = color*texture2D(texture0, vtexcoord0);
}

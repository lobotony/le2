uniform mat4 modelViewMatrix;  // mesh transform
uniform mat4 projectionMatrix; // from camera

attribute vec3 position;
attribute vec2 texcoord0;

varying vec2 vtexcoord0;
varying vec2 v_blurTexCoords[14];

void main(void)
{
  vec4 pos = vec4(position, 1.0);
  vtexcoord0 = texcoord0;

  v_blurTexCoords[ 0] = vtexcoord0 + vec2(0.0, -0.028);
  v_blurTexCoords[ 1] = vtexcoord0 + vec2(0.0, -0.024);
  v_blurTexCoords[ 2] = vtexcoord0 + vec2(0.0, -0.020);
  v_blurTexCoords[ 3] = vtexcoord0 + vec2(0.0, -0.016);
  v_blurTexCoords[ 4] = vtexcoord0 + vec2(0.0, -0.012);
  v_blurTexCoords[ 5] = vtexcoord0 + vec2(0.0, -0.008);
  v_blurTexCoords[ 6] = vtexcoord0 + vec2(0.0, -0.004);
  v_blurTexCoords[ 7] = vtexcoord0 + vec2(0.0,  0.004);
  v_blurTexCoords[ 8] = vtexcoord0 + vec2(0.0,  0.008);
  v_blurTexCoords[ 9] = vtexcoord0 + vec2(0.0,  0.012);
  v_blurTexCoords[10] = vtexcoord0 + vec2(0.0,  0.016);
  v_blurTexCoords[11] = vtexcoord0 + vec2(0.0,  0.020);
  v_blurTexCoords[12] = vtexcoord0 + vec2(0.0,  0.024);
  v_blurTexCoords[13] = vtexcoord0 + vec2(0.0,  0.028);
  
  gl_Position = projectionMatrix*modelViewMatrix*pos; // equivalent to builtin function ftransform()
}

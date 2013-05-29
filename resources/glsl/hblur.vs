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

  v_blurTexCoords[ 0] = vtexcoord0 + vec2(-0.028, 0.0);
  v_blurTexCoords[ 1] = vtexcoord0 + vec2(-0.024, 0.0);
  v_blurTexCoords[ 2] = vtexcoord0 + vec2(-0.020, 0.0);
  v_blurTexCoords[ 3] = vtexcoord0 + vec2(-0.016, 0.0);
  v_blurTexCoords[ 4] = vtexcoord0 + vec2(-0.012, 0.0);
  v_blurTexCoords[ 5] = vtexcoord0 + vec2(-0.008, 0.0);
  v_blurTexCoords[ 6] = vtexcoord0 + vec2(-0.004, 0.0);
  v_blurTexCoords[ 7] = vtexcoord0 + vec2( 0.004, 0.0);
  v_blurTexCoords[ 8] = vtexcoord0 + vec2( 0.008, 0.0);
  v_blurTexCoords[ 9] = vtexcoord0 + vec2( 0.012, 0.0);
  v_blurTexCoords[10] = vtexcoord0 + vec2( 0.016, 0.0);
  v_blurTexCoords[11] = vtexcoord0 + vec2( 0.020, 0.0);
  v_blurTexCoords[12] = vtexcoord0 + vec2( 0.024, 0.0);
  v_blurTexCoords[13] = vtexcoord0 + vec2( 0.028, 0.0);
  
  gl_Position = projectionMatrix*modelViewMatrix*pos; // equivalent to builtin function ftransform()
}

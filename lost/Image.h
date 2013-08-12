#ifndef LOST_IMAGE_H
#define LOST_IMAGE_H

namespace lost
{

/** Describes a piece of a bitmap that can be used for drawing.
 */
 
enum ImageResizeMode
{
  ImageResizeModeStretch = 0,
  ImageResizeModeTile = 1,
  ImageResizeModeNinePatch = 2
};
 
enum ImageOrientation
{
  ImageOrientationUp = 0,
  ImageOrientationDown,
  ImageOrientationLeft,
  ImageOrientationRight,
  ImageOrientationUpMirrored,
  ImageOrientationDownMirrored,
  ImageOrientationLeftMirrored,
  ImageOrientationRightMirrored
};
 
struct Image
{
  Image(const TexturePtr& tex);
  ~Image();
  
  TexturePtr        texture;
  Rect              originalPixelCoords; // these are the original pixel coords of the image inside the texture
  Vec2              size; // size of image in pixels, taking orientation into account
  
  ImageResizeMode   resizeMode;
  ImageOrientation  orientation;

  f32               t,b,l,r; // top/bottom/left/right caps for NinePatch drawing

  // normalised texture coordinates that describe the part of the texture used for drawing.
  Vec2              bl, br, tr, tl;

  void updateTextureCoords();

};

}

#endif
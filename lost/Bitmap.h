/*
Copyright (c) 2012 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_BITMAP_BITMAP_H
#define LOST_BITMAP_BITMAP_H

namespace lost
{
  struct Bitmap
  {
    
    uint8_t*    data;   // points to the raw pixel data
    uint32_t    width;  // width in pixels
    uint32_t    height; // height in pixels
    GLenum      format; // format of bitmap (rgb, rgba)
    bool        premultiplied; // true if alpha was premlultiplied, false otherwise

    /** creates an empty bitmap with zero size.
     * Use init to resize it in a given format.
     */
    Bitmap();
      
    /** creates a bitmap with the given size and format. The initial content is undefined.
     */
    Bitmap(uint32_t inWidth,
           uint32_t inHeight,
           GLenum format);
  
    /** creates a bitmap by reading the provided data with width*height pixels in source format
     *  copying it to a new internal buffer, converting the components to destination format
     * if necessary
     */
    Bitmap(uint32_t inWidth,
           uint32_t inHeight,
           GLenum destComponents,
           GLenum srcComponents,
           uint8_t* data);
           
    // tries to create a bitmap by interpreting the data as a bitmap format understood by
    // the image library.       
    Bitmap(const DataPtr& inData);
    
    static BitmapPtr create() { return BitmapPtr(new Bitmap()); }
    static BitmapPtr create(const DataPtr& inData) { return BitmapPtr(new Bitmap(inData)); }
    static BitmapPtr create(uint32_t inWidth,
           uint32_t inHeight,
           GLenum format) { return BitmapPtr(new Bitmap(inWidth, inHeight, format)); }
    
    void reset();
    virtual ~Bitmap();

    /** inits a bitmap of destComponents format with the given data in the given format.
     * The data is copied and converted if necessary.
     *
     * @param inWidth width of the new bitmap
     * @param inHeight height of the new bitmap
     * @param destComponents format of the bitmap
     * @param srcComponents format of the source data
     * @param srcData source data from which the bitmap is constructed
     */
    void init(uint32_t inWidth,
              uint32_t inHeight,
              GLenum destComponents,
              GLenum srcComponents,
              uint8_t* srcData);

    /** initialises the bitmap with the given size and format. 
     * The initial contents are undefined.
     *
     * @param inWidth width of the bitmap.
     * @param inHeight height of the bitmap.
     * @param format pixel format of the bitmap.
     *
     */
    void init(uint32_t inWidth,
              uint32_t inHeight,
              GLenum format);

    /** attempts to interpret a chunk ofmemory as a bitmap, with the help of the underlying image library.
     */
    void init(const DataPtr& inData);
    
    /** calculates the number of pixels for a given bitmap components constant.
     */
    static uint32_t bytesPerPixelFromComponents(GLenum components);

    /** copies exctaly one pixel from src to dest, converting the format 
     * from srcComponents to destComponents.
     *
     * Conversion is as follows:
     * alpha to rgb/rgba: the alpha value is copied to all r/g/b/a components, 
     *                    resulting in a white picture that fades where its alpha
     *                    fades.
     * rgb to rgba:       rgb components are copied, alpha is set to 1
     * rgba to rgb:       alpha value is omitted
     * rgb to alpha:      alpha is set 1 
     * rgba to alpha:     only the alpha channel is copied
     */
    static void copyPixel(uint8_t* dest,
                          GLenum destComponents, 
                          uint8_t* src,
                          GLenum srcComponents);
                          
                          
    /** clears the bitmap, overwriting all pixels with the given color.
     * Depending on the internal format of the bitmap, not all of the color components might be used.
     * I.e. for COMPONENTS_ALPHA, only the alpha value of the given color will be used for clearing. 
     *
     * @param inColor the clear color.
     *
     */                      
    void clear(const Color& inColor);
    
    /** writes the bitmap to a TGA file.
     * The file is created or overwritten.
     *
     * @param inFullPathname full path name of the file to write to.
     */
    void write(const string inFullPathname);

    /** flips the bitmap vertically.
     * This moves a lot of memory, so be careful performance wise.
     */
    void flip();
  
    /** returns a pointer to the data of the pixel at the given coordinates */
    uint8_t* pixelPointer(uint32_t x, uint32_t y);
      
    /** sets a pixel with the given color */
    void pixel(uint32_t x, uint32_t y, const Color& inColor);
    
    /** reads a pixel from the given coordinates and returns it as a Color. */
    Color pixel(uint32_t x, uint32_t y);
    
    /** draws a horizontal line into the bitmap.
     * No bounds checks are performed, you'll crsah if you draw outside the bitmap.
     * @param y       the y coordinate of the line.
     * @param xl      the left x coordinate of the starting point.
     * @param xr      the right x coordinate of the end point.
     * @param inColor color to draw the line in.
     */
    void hline(uint32_t y, uint32_t xl, uint32_t xr, const Color& inColor);
    
    /** draws a horizontal line into the bitmap.
     * No bounds checks are performed, you'll crash if you draw outside the bitmap.
     * @param x         the x coordinate of the line.
     * @param yb        the bottom y coordinate of the starting point.
     * @param yt        the top y coordinate of the end point.
     * @param inColor   color to draw the line in.
     */
    void vline(uint32_t x, uint32_t yb, uint32_t yt, const Color& inColor);

    // draws a filled disc at given center and radius
    // the disc is drawn by evaluating all pixels of the bitmap and setting them accordingly
    void disc(float x, float y, float r);
    void ring(float x, float y, float r, float t);
  
    // draws a filled rect at pos with size
    void filledRect(const Color& col, uint32_t posx, uint32_t posy, uint32_t sizew, uint32_t sizeh);
       
    /** draws the rect outline into the bitmap with the given color
     */
    void drawRectOutline(const Rect& inRect, const Color& inColor);  
      
    /** rotates the bitmap clock wise and returns it as a new bitmap*/
    BitmapPtr rotCW();
    
    /** returns the occupied are in pixels. */
    float area() { return (float)(width*height); }
    
    /** draws this bitmap into the specified target. */
    void draw(uint32_t x, uint32_t y, BitmapPtr target);
    
    void premultiplyAlpha(); // multiplies each pixels color channels with its alpha channel.
    
  private:
    DISALLOW_COPY_AND_ASSIGN(Bitmap);
    void destroy();
    bool loaded; // true if the image was loaded with the image library and data must be freed by it.
                 // false if data is just a chunk of memory and can simply be deleted

    void clearA(const Color& inColor);
    void clearRGB(const Color& inColor);
    void clearRGBA(const Color& inColor);
  };
} // namespace lost

#endif

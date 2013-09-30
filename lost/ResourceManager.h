#ifndef LOST_RESOURCEMANAGER_H
#define LOST_RESOURCEMANAGER_H

#include "lost/Bundle.h"

namespace lost
{

/** central storage for all kinds of resources.
 * The ResourceManager might, at any point in time, garbage collect resources.
 */
struct ResourceManager
{
  ResourceManager();
  ~ResourceManager();
  
  BitmapPtr bitmap(const string& bitmapPath); // takes an explicit resource path, e.g. "resources/images/background.png"
  BitmapPtr bitmap(ResourceId bitmapHash);

  bool hasTexture(const string& texturePath);
  bool hasTexture(ResourceId rid);

  TexturePtr texture(const string& bitmapPath); // loads a bitmap and creates a texture from it, caching it
  TexturePtr texture(const string& bitmapPath, const TexturePtr& tex); // takes tex and adds it to the resources under path 'bitmapPath', returning tex. Nothing is loaded from disk
  TexturePtr texture(ResourceId bitmapHash);

  bool hasImage(const string& path);
  bool hasImage(ResourceId rid);

  ImagePtr image(const string& bitmapPath); // loads a bitmap and creates a texture from it, caching it
  ImagePtr image(const string& bitmapPath, const ImagePtr& tex); // takes tex and adds it to the resources under path 'bitmapPath', returning tex. Nothing is loaded from disk
  ImagePtr image(ResourceId bitmapHash);

  ShaderProgramPtr shader(const string& shaderPath); // base path to a pair of files with .vs/.fs extensions
  ShaderProgramPtr shader(ResourceId bitmapHash);
  
  // register font bundles first
  // then access the fonts that are defined in the bundles via font(name, size)
  // only supports truetype fonts for now
  void registerFontBundle(const string& fontBundlePath); // reads the meta.json file and registers the font names
  FontPtr font(const string& fontName, u32 fontSize); // loads the font with given name and instantiates a truetype font with the specified size, caching the font
  FontPtr font(ResourceId rid, u32 fontSize); // same as above, with resourceId instead of fontName
  
  void logStats();

  void useCacheAt(const string& cacheDir);

private:
  Path cacheDir; // will create cache and reuse data on start/texture load if this is set
  bool useCache;
  ResourceId stringToHash(const string& resourcePath);
  ResourceId hashPath(const string& resourcePath);

  ResourceBundle mainBundle;
  
  map<ResourceId, string>             hash2string;
  
  map<ResourceId, BitmapPtr>          hash2bitmap;
  map<ResourceId, TexturePtr>         hash2texture;
  map<ResourceId, ImagePtr>           hash2image;
  map<ResourceId, ShaderProgramPtr>   hash2shaderprogram;
  
  // ResourceId is derived from font name in meta file, NOT from data path
  // data is loaded only once, but multiple font instances can exist, if the instances differ
  // in size. 
  map<ResourceId, Path>             fontId2dataPath;
  map<ResourceId, DataPtr>            fontId2data;
  map<pair<ResourceId, u32>, FontPtr> fontIdSize2font;
};
}

#endif


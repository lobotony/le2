#ifndef LOST_RESOURCEMANAGER_H
#define LOST_RESOURCEMANAGER_H

#include "lost/Bundle.h"

namespace lost
{
struct ResourceManager
{
  ResourceManager();
  ~ResourceManager();
  
  ResourceId stringToHash(const string& resourcePath);
  
  BitmapPtr bitmap(const string& bitmapPath); // takes an explicit resource path, e.g. "resources/images/background.png"
  BitmapPtr bitmap(ResourceId bitmapHash);

  TexturePtr texture(const string& bitmapPath); // loads a bitmap and creates a texture from it, caching it
  TexturePtr texture(ResourceId bitmapHash);

  void logStats();

  ResourceBundle mainBundle;
  
  map<ResourceId, string>           hash2string; 
  
  map<ResourceId, BitmapPtr>        hash2bitmap;
  map<ResourceId, TexturePtr>       hash2texture;
  map<ResourceId, ShaderProgramPtr> hash2shaderprogram;
};
}

#endif


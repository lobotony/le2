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
  
  // convenience methods that take paths and hash them before passing them on
  BitmapPtr bitmap(const string& resourcePath); // takes an explicit resource path, e.g. "images/background.png"
  BitmapPtr bitmap(ResourceId bitmapHash);
//  TexturePtr texture(u32 bitmapHash);
//  FontPtr font(u32 );

  ResourceBundle mainBundle;
  
  map<ResourceId, string>           hash2string; 
  
  map<ResourceId, BitmapPtr>        hash2bitmap;
  map<ResourceId, TexturePtr>       hash2texture;
  map<ResourceId, ShaderProgramPtr> hash2shaderprogram;
};
}

#endif


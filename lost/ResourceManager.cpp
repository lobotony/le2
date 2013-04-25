#include "lost/ResourceManager.h"
#include "lost/Bitmap.h"
#include "lost/Texture.h"

namespace lost
{

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

u32 djb2Hash(const char* data)
{
  u32 hash = 5381;
  s32 c;

  const unsigned char* str = reinterpret_cast<const unsigned char*> (data);
  while ((c = *str++))
  {
      hash = ((hash << 5) + hash) + c;
  }

  return hash;  
}

ResourceId ResourceManager::stringToHash(const string& resourcePath)
{
  ResourceId result = djb2Hash(resourcePath.c_str());
//  DOUT(result << " = '" << resourcePath << "'");
  return result;
}

ResourceId ResourceManager::hashPath(const string& resourcePath)
{
  ResourceId rid = stringToHash(resourcePath);
  if(hash2string.find(rid) == hash2string.end())
  {
    hash2string[rid] = resourcePath;
  }
  ASSERT(hash2string[rid] == resourcePath, "tried to overwrite resource id:"<<rid << " old path:'"<<hash2string[rid] << "new path:" <<resourcePath);
  return rid;
}

BitmapPtr ResourceManager::bitmap(const string& resourcePath) 
{
  return bitmap(hashPath(resourcePath));
}

BitmapPtr ResourceManager::bitmap(ResourceId rid)
{
  BitmapPtr result;
  
  if(hash2bitmap.find(rid) == hash2bitmap.end())
  {
    // hash to string mapping MUST exist if it wasn't loaded yet
    ASSERT(hash2string.find(rid) != hash2string.end(), "couldn't find bitmap resource with id:"<<rid);
    DOUT("caching bitmap: " << rid << " -> " << hash2string[rid]);
    DataPtr data = mainBundle.load(hash2string[rid]);
    result.reset(new Bitmap(data));
    hash2bitmap[rid] = result;
  }
  else
  {
    result = hash2bitmap[rid];
  }
  
  return result;
}

TexturePtr ResourceManager::texture(const string& bitmapPath)
{
  return texture(hashPath(bitmapPath));
}

TexturePtr ResourceManager::texture(ResourceId rid)
{
  TexturePtr result;
  
  if(hash2texture.find(rid) == hash2texture.end())
  {
    // hash to string mapping MUST exist if it wasn't loaded yet
    ASSERT(hash2string.find(rid) != hash2string.end(), "couldn't find texture resource with id:"<<rid);
    DOUT("caching texture: " << rid << " -> " << hash2string[rid]);
    
    DataPtr data = mainBundle.load(hash2string[rid]);
    BitmapPtr bmp(new Bitmap(data));
    result.reset(new Texture(bmp));
    hash2texture[rid] = result;
  }
  else
  {
    result = hash2texture[rid];
  }
  
  return result;
}

void ResourceManager::logStats()
{
  DOUT("bitmaps: "<<(u32)hash2bitmap.size());
  DOUT("textures: "<<(u32)hash2texture.size());
  DOUT("shader programs: "<<(u32)hash2shaderprogram.size());
}

}


#include "lost/ResourceManager.h"
#include "lost/Bitmap.h"

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
  DOUT(result << " = '" << resourcePath << "'");
  return result;
}

BitmapPtr ResourceManager::bitmap(const string& resourcePath) 
{
  ResourceId rid = stringToHash(resourcePath);
  if(hash2string.find(rid) == hash2string.end())
  {
    hash2string[rid] = resourcePath;
  }
  ASSERT(hash2string[rid] == resourcePath, "tried to overwrite resource id:"<<rid << " old path:'"<<hash2string[rid] << "new path:" <<resourcePath);

  return bitmap(rid);
}

BitmapPtr ResourceManager::bitmap(ResourceId rid)
{
  BitmapPtr result;
  
  if(hash2bitmap.find(rid) == hash2bitmap.end())
  {
    // hash to string mapping MUST exist if it wasn't loaded yet
    ASSERT(hash2string.find(rid) != hash2string.end(), "couldn't find bitmap resource with id:"<<rid);
    
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

void ResourceManager::logStats()
{
  DOUT("bitmaps: "<<(u32)hash2bitmap.size());
  DOUT("textures: "<<(u32)hash2texture.size());
  DOUT("shader programs: "<<(u32)hash2shaderprogram.size());
}

}


#include "lost/ResourceManager.h"

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
  return djb2Hash(resourcePath.c_str());
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

BitmapPtr ResourceManager::bitmap(u32 bitmapHash)
{
  BitmapPtr result;
  
  return result;
}


}


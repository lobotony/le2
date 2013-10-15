#include "lost/ResourceManager.h"
#include "lost/Bitmap.h"
#include "lost/BitmapIo.h"
#include "lost/Texture.h"
#include "lost/TruetypeFont.h"
#include "lost/StringAdditions.h"

namespace lost
{

#pragma mark - Lifecycle -

ResourceManager::ResourceManager()
{
  useCache = false;
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::useCacheAt(const string& cacheDir)
{
  this->cacheDir = cacheDir;
  create_directories(cacheDir);
  useCache = true;
}

#pragma mark - Hash / ResourceId -

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

#pragma mark - Bitmap -

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

#pragma mark - Texture -

bool ResourceManager::hasTexture(const string& texturePath)
{
  ResourceId rid = hashPath(texturePath);
  return hasTexture(rid);
}

bool ResourceManager::hasTexture(ResourceId rid)
{
  return (hash2texture.find(rid) != hash2texture.end());  
}

TexturePtr ResourceManager::texture(const string& bitmapPath)
{
  return texture(hashPath(bitmapPath));
}

TexturePtr ResourceManager::texture(ResourceId rid)
{
  TexturePtr result;
  
  if(!hasTexture(rid))
  {
    // hash to string mapping MUST exist if it wasn't loaded yet
    ASSERT(hash2string.find(rid) != hash2string.end(), "couldn't find texture resource with id:"<<rid);
    DOUT("caching texture: " << rid << " -> " << hash2string[rid]);
    
    if(useCache)
    {
      Path cachedBitmapPath = cacheDir / Path(hash2string[rid]).file().string()+".raw";
      if(exists(cachedBitmapPath))
      {
        DOUT("reading from memory mapped raw file");
        BitmapPtr mapped = loadMapped(cachedBitmapPath.string());
        result.reset(new Texture(mapped));
      }
      else
      {
        DOUT("creating raw cache file");
        DataPtr data = mainBundle.load(hash2string[rid]);
        BitmapPtr bmp(new Bitmap(data));
        bmp->premultiplyAlpha();
        result.reset(new Texture(bmp));
        writeRawToPath(bmp, cachedBitmapPath.string());
      }
    }
    else
    {
      DataPtr data = mainBundle.load(hash2string[rid]);
      BitmapPtr bmp(new Bitmap(data));
      bmp->premultiplyAlpha();
      result.reset(new Texture(bmp));
    }
    result->filter(GL_LINEAR);
    hash2texture[rid] = result;
  }
  else
  {
    result = hash2texture[rid];
  }
  
  return result;
}

TexturePtr ResourceManager::texture(const string& bitmapPath, const TexturePtr& tex)
{
  TexturePtr result = tex;
  ResourceId rid = hashPath(bitmapPath);
  
  if(!hasTexture(rid))
  {
    hash2texture[rid] = tex;
  }
  
  return result;
}

#pragma mark - Image -

bool ResourceManager::hasImage(const string& path)
{
  ResourceId rid = hashPath(path);
  return hasImage(rid);
}

bool ResourceManager::hasImage(ResourceId rid)
{
  return (hash2image.find(rid) != hash2image.end());
}

ImagePtr ResourceManager::image(const string& bitmapPath)
{
  return image(hashPath(bitmapPath));
}

ImagePtr ResourceManager::image(const string& bitmapPath, const ImagePtr& img)
{
  ImagePtr result = img;
  ResourceId rid = hashPath(bitmapPath);
  
  if(!hasImage(rid))
  {
    hash2image[rid] = img;
  }
  
  return result;
}

ImagePtr ResourceManager::image(ResourceId rid)
{
  ImagePtr result;
  
  if(!hasImage(rid))
  {
    // hash to string mapping MUST exist if it wasn't loaded yet
    ASSERT(hash2string.find(rid) != hash2string.end(), "couldn't find image resource with id:"<<rid);
    DOUT("caching image: " << rid << " -> " << hash2string[rid]);
    
    TexturePtr tex = texture(rid);
    result.reset(new Image(tex));
    result->orientation = ImageOrientationUp;
    hash2image[rid] = result;
  }
  else
  {
    result = hash2image[rid];
  }
  
  return result;
}


ShaderProgramPtr ResourceManager::shader(const string& shaderPath)
{
  return shader(hashPath(shaderPath));  
}

ShaderProgramPtr ResourceManager::shader(ResourceId rid)
{
  ShaderProgramPtr result;
  
  if(hash2shaderprogram.find(rid) == hash2shaderprogram.end())
  {
    // hash to string mapping MUST exist if it wasn't loaded yet
    ASSERT(hash2string.find(rid) != hash2string.end(), "couldn't find shader resource with id:"<<rid);
    DOUT("caching shader: " << rid << " -> " << hash2string[rid]);
    
    result = mainBundle.loadShader(hash2string[rid]);
    hash2shaderprogram[rid] = result;
  }
  else
  {
    result = hash2shaderprogram[rid];
  }

  return result;
}

void ResourceManager::registerFontBundle(const string& fontBundlePath)
{
  // fontBundles are relative to the resource dir, so build a new bundle originating form the resourceBundle
  Bundle fontBundle = mainBundle.subBundle(fontBundlePath);
  Json::Value metadata = fontBundle.loadJson("meta.json");

/*
  // ------------- debug
  Json::StyledWriter writer;
  string w = writer.write(metadata);
  DOUT("loaded font metadata: "<<w);
  // ------------- debug
*/
  
  Json::Value fonts = metadata["fonts"];
  DOUT("fonts found: "<<fonts.size());
  for(u32 i=0; i<fonts.size(); ++i)
  {
    Json::Value font = fonts[i];
    if(font.type() == Json::objectValue)
    {
      string fontName = font["name"].asString();
      Path absoluteFontPath = fontBundle._path / font["ttfile"].asString();
      Path relativeFontPath = absoluteFontPath.relativeTo(mainBundle._path);
    
      ResourceId rid = hashPath(fontName);
      fontId2dataPath[rid] = relativeFontPath;
    
      DOUT(fontName << " -> " << relativeFontPath);
    }
    else
    {
      EOUT("not a dictionary");
    }
  }
}

FontPtr ResourceManager::font(const string& fontName, u32 fontSize)
{
  return font(hashPath(fontName), fontSize);
}

FontPtr ResourceManager::font(ResourceId rid, u32 fontSize)
{
  FontPtr result;
  
  ASSERT(fontId2dataPath.find(rid) != fontId2dataPath.end(), "can't find font data path for font with id:"<<rid<<" -> " << hash2string[rid]);
  pair<ResourceId, u32> fontKey = make_pair(rid, fontSize);
  
  map<pair<ResourceId, u32>, FontPtr>::iterator fontPos = fontIdSize2font.find(fontKey);
  if(fontPos == fontIdSize2font.end())
  {
    DOUT("no font yet for "<<hash2string[rid]<<" in size "<<fontSize<<" , creating");
    // no font instance for this key => check if data was already loaded
    DataPtr data;
    map<ResourceId, DataPtr>::iterator dataPos = fontId2data.find(rid);
    if(dataPos == fontId2data.end())
    {
      DOUT("loading data for font: "<<hash2string[rid]<< " "<<fontId2dataPath[rid]);
      // no data loaded, get it from disk
      data = mainBundle.load(fontId2dataPath[rid]);
      fontId2data[rid] = data;
    }
    else
    {
      data = fontId2data[rid];
    }
    // instantiate font with data and size and put it into map
    result.reset(new TruetypeFont(data, fontSize));
    fontIdSize2font[fontKey] = result;
  }
  else
  {
    result = fontIdSize2font[fontKey];
  }
  return result;
}

void ResourceManager::logStats()
{
  DOUT("bitmaps: "<<(u32)hash2bitmap.size());
  DOUT("textures: "<<(u32)hash2texture.size());
  DOUT("images: "<<(u32)hash2image.size());
  DOUT("shader programs: "<<(u32)hash2shaderprogram.size());
  DOUT("font names: "<<(u32)fontId2dataPath.size());
  DOUT("font data loaded: "<<(u32)fontId2data.size());
  DOUT("fonts instantiated: "<<(u32)fontIdSize2font.size());
  DOUT("hashed resource paths: "<<(u32)hash2string.size());
  for(map<ResourceId, string>::iterator pos = hash2string.begin(); pos != hash2string.end(); ++pos)
  {
    DOUT(pos->first << " -> " << pos->second);
  }
}

}


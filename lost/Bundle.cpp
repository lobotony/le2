#include "lost/Bundle.h"
#include "lost/PlatformPaths.h"
#include "lost/Data.h"
#include "lost/ShaderPreprocessor.h"
#include "lost/Texture.h"

namespace lost
{

Bundle::Bundle()
{
  
}

Bundle::Bundle(const Path& inPath)
: _path(inPath)
{

}

Bundle Bundle::subBundle(const Path& inRelativePath)
{
  return Bundle(_path / inRelativePath);
}

DataPtr Bundle::load(const Path& relativePath) const
{
  DataPtr result(new Data);
  
  Path absolutePath = _path / relativePath;
  string spath = absolutePath.string();
  FILE* file;
  ASSERT(file = fopen(spath.c_str(), "rb"), "couldn't open file "<<spath);
  ASSERT(0 == fseek(file, 0, SEEK_END), "couldn't seek "<<spath);
  s64 size;
  size = ftell(file);
  ASSERT(size != -1, "couldn't get file pos "<<spath);
  result->bytes.reset(new char[size]);
  result->size = size;
  DOUT("'"<<relativePath<<"' [" << size << " bytes]");
  ASSERT(0 == fseek(file, 0, SEEK_SET), "couldn't seek "<<spath);
  fread(result->bytes.get(), size, 1, file);
  ASSERT(0 == ferror(file), "couldn't read "<<spath);
  ASSERT(0 == fclose(file), "couldn't close "<<spath); 
  return result;
}

ShaderProgramPtr Bundle::loadShader(const Path& relativePath) const
{
  return lost::loadShader(*this, relativePath);
}

TexturePtr Bundle::loadTexture(const Path& relativePath) const
{
  return Texture::create(load(relativePath));
}

Json::Value Bundle::loadJson(const Path& relativePath) const
{
  Json::Value result;
  
  DataPtr data = load(relativePath);
  string stringData = data->str();
  
  Json::Reader jsonReader;
  bool parsingSuccessful = jsonReader.parse(stringData, result);
  if (!parsingSuccessful)
  {
    EOUT("!!!!!!!!!!!!!!!");
    EOUT("!!!!!!!!!!!!!!!");
    EOUT("Failed to parse file: "<<relativePath << " error:" << jsonReader.getFormatedErrorMessages());
    EOUT("!!!!!!!!!!!!!!!");
    EOUT("!!!!!!!!!!!!!!!");
  }
  
  return result;
}

ResourceBundle::ResourceBundle()
: Bundle(getResourcePath())
{
}

}
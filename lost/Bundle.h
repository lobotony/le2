#ifndef LOST_BUNDLE_H
#define LOST_BUNDLE_H

#include "lost/Path.h"
#include "json/json.h"

namespace lost
{

/** Read only directory.
 * Might impose/require a certain file system layout and provide helper functions to deal with it.
 */
struct Bundle
{
  Bundle();
  Bundle(const Path& inPath);
  virtual ~Bundle() {};
  
  Bundle subBundle(const Path& inRelativePath);
  
  DataPtr           load(const Path& relativePath) const;
  ShaderProgramPtr  loadShader(const Path& relativePath) const; // loads shader using the shader preprocessor function
  TexturePtr        loadTexture(const Path& relativePath) const; // convenience function that loads an image and instantiates a texture. 
  Json::Value       loadJson(const Path& relativePath) const;
  
  Path _path;
};


/** Application resource Bundle.
 * Points to the applications resources for easy access. 
 * Mac: .app/Contents/Resources
 * iOS: .app/
 * Windows: executable directory
 */
struct ResourceBundle : Bundle
{
  ResourceBundle();
};

}

#endif


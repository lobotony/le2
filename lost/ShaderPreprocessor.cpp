#include "lost/ShaderPreprocessor.h"
#include "lost/StringAdditions.h"
#include "lost/Data.h"
#include "lost/ShaderProgram.h"
#include "lost/Shader.h"

namespace lost
{

string loadData(const Bundle& bundle, const Path& relativePath)
{
  DOUT("loading shader part from: "<<relativePath);
  DataPtr data = bundle.load(relativePath);
  return data->str();
}

bool isDirective(const string& line, const string& name)
{
  bool result = false;
  string trimmed = trim(line);
  if(trimmed.size() > 0)
  {
    if(trimmed[0] == '#')
    {
      if(line.find(name) == 1)
      {
        result = true;
      }
    }
  }
  return result;
}

bool isImportDirective(const string& line)
{
  return isDirective(line, "import");
}


bool isIncludeDirective(const string& line)
{
  return isDirective(line, "include");
}

string extractIncludeFilename(const string& line)
{
  string trimmed = trim(line);
  vector<string> splitLine;
  split(line, splitLine, "\"", false);
  return splitLine[1];
}

string preprocessOnce(const Bundle& bundle,
                    const string& shaderSource,
                    map<string, bool>& imports,
                    uint32_t& numAddedParts)
{
  string result;
  string source = shaderSource;
  normaliseLinebreaks(source);
  vector<string> splitLines;
  split(shaderSource, splitLines, "\n", false);
  numAddedParts = 0;
  
  // FIXME: prepend GLES define if required
  
  for(uint32_t i=0; i<splitLines.size(); ++i)
  {
    string& line = splitLines[i];
    if(isIncludeDirective(line))
    {
      string relativePath = extractIncludeFilename(line);
      string includedFile = loadData(bundle, relativePath);
      result+=includedFile;
      numAddedParts+=1;
    }
    else if(isImportDirective(line))
    {
      string relativePath = extractIncludeFilename(line);
      if(imports.find(relativePath) == imports.end())
      {
        imports[relativePath] = true;
        string includedFile = loadData(bundle, relativePath);
        result += includedFile;
        numAddedParts+=1;
      }
    }
    else
    {
      result.append(line);
      result.append("\n");
    }
  }
  return result;
}

string preprocessShader(const Bundle& bundle, const string& shaderSource)
{
  string result = shaderSource;
  map<string, bool> imports;
  uint32_t numAddedParts;
  do
  {
    numAddedParts = 0;
    result = preprocessOnce(bundle, result, imports, numAddedParts);
  }
  while(numAddedParts > 0);
  
  return result;
}


void logBrokenShader(const string& shaderSource)
{
  vector<string> splitLines;
  string source = shaderSource;
  normaliseLinebreaks(source);
  split(shaderSource, splitLines, "\n", false);
  for(uint32_t i=0; i<splitLines.size(); ++i)
  {
    EOUT(i << "  :  "<<splitLines[i]);
  }
}

ShaderProgramPtr buildShader(const Bundle& bundle,
                             const string& inName,
                             const string& vssource,
                             const string& fssource)
{
//  DOUT("building shader: '"<<inName<<"'");
  ShaderProgramPtr  shaderProgram(new ShaderProgram());
  ShaderPtr         vertexShader(new Shader(GL_VERTEX_SHADER));
  ShaderPtr         fragmentShader(new Shader(GL_FRAGMENT_SHADER));

  string vsname = inName+".vs";
  string fsname = inName+".fs";

  string source = preprocessShader(bundle, vssource);
  vertexShader->source(source);
  vertexShader->compile();
  if(!vertexShader->compiled())
  {
    logBrokenShader(source);
    ASSERT(false, "vertex shader '"<<vsname<<"' compilation failed: "<<vertexShader->log());
  }

  source = preprocessShader(bundle, fssource);
  fragmentShader->source(source);
  fragmentShader->compile();
  if(!fragmentShader->compiled())
  {
    logBrokenShader(source);
    ASSERT(false, "fragment shader '"<<fsname<<"' compilation failed: "<<fragmentShader->log());
  }

  shaderProgram->attach(fragmentShader);
  shaderProgram->attach(vertexShader);
  shaderProgram->link();
  if(!shaderProgram->linked())
  {
    ASSERT(false, "shader program '"<<inName<<"' link failed: "<<shaderProgram->log());
  }

  shaderProgram->enable();
  shaderProgram->buildUniformMap();
  shaderProgram->buildVertexAttributeMap();
  shaderProgram->disable();
  
  return shaderProgram;
}

ShaderProgramPtr loadShader(const Bundle& bundle, Path relativePath)
{
//  DOUT("--- loading shader '"<<inName<<"'");
  Path vspath = relativePath + ".vs";
  string vssource = bundle.load(vspath)->str();
  Path fspath = relativePath + ".fs";
  string fssource = bundle.load(fspath)->str();
  return buildShader(bundle, relativePath.string(), vssource, fssource);
}

}


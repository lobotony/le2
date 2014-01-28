#pragma once

#include "lost/Bundle.h"

namespace lost
{

/** preprocesses a given shader source and returns the result.
 * interprets include and import statements. 
 * adds a symbol to the start of the shader if target OS is using GLES.
 */
string preprocessShader(const Bundle& bundle, const string& source);

/** outputs the shader source line by line as error message, prefixing it with line number, 1-based.
 */
void logBrokenShader(const string& shaderSource);

/** loads and compiles vertex and fragment shader from the given path, assembling them to a shader program.
 * Shader sources must exist in two files, named "<relativePath>.vs" for vertex shader and "<relativePath>.fs" 
 * for fragment shader.
 */
ShaderProgramPtr loadShader(const Bundle& bundle, Path relativePath);


}




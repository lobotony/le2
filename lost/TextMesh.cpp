/*
Copyright (c) 2012 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "TextMesh.h"

namespace lost
{

TextMesh::TextMesh() : Quad()
{
}

TextMesh::TextMesh(const vector<Rect>& rects,
             TexturePtr tex,
             const vector<Rect>& pixelCoords) : Quad(rects, tex, pixelCoords, false)
{
}

TextMesh::~TextMesh()
{
}

TextMeshPtr TextMesh::create() { return TextMeshPtr(new TextMesh); }

MeshPtr TextMesh::clone()
{
  TextMeshPtr result(new TextMesh);
  
  result->material = material;
  result->transform = transform;
  result->vertexBuffer = vertexBuffer;
  result->indexBuffer = indexBuffer;
  result->min = min;
  result->max = max;
  result->size = size;
  result->numLines = numLines;
  
  return result;
}

void TextMesh::resetCharacterMetrics()
{
  characterMetrics.reset(new vector<vector<Rect> >);
}

void TextMesh::pushEmptyCharacterMetricLine()
{
  characterMetrics->push_back(vector<Rect>());
}

void TextMesh::pushCharacterRect(const Rect& cr)
{
  s32 line = (s32)characterMetrics->size();
  line = std::max(line-1, 0);
//  DOUT("// pushing char metrics rect "<<line<<" "<<characterMetrics->at(line).size()<<" "<<cr);
  characterMetrics->at(line).push_back(cr);
}

Rect TextMesh::characterRect(uint32_t lineIndex, uint32_t charIndex)
{
  Rect result;
  
  if(characterMetrics && (lineIndex < characterMetrics->size()))
  {
    if(charIndex < characterMetrics->at(lineIndex).size())
    {
//      DOUT("cr found");
      result = characterMetrics->at(lineIndex).at(charIndex);
    }
  }
  
  return result;
}

}

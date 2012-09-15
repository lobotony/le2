#include "Canvas.h"
#include "Quad.h"
#include "TruetypeFont.h"
#include "TextRender.h"
#include "TextMesh.h"
#include "TextBuffer.h"
#include "Matrix.h"

CanvasObject::CanvasObject() {
  isVisible = true;
  x = 0;
  y = 0;
  xScale = 1;
  yScale = 1;
  rotation = 0;
}

CanvasObject::CanvasObject(const MeshPtr& mesh)
: mesh(mesh) {
  isVisible = true;
  x = 0;
  y = 0;
  xScale = 1;
  yScale = 1;
  rotation = 0;
}

void CanvasObject::process(Context* context) {
  if (isVisible) {
    render(context);
    for (std::list<CanvasObjectPtr>::iterator idx = children.begin(); idx != children.end(); ++idx) {
      (*idx)->process(context);
    }
  }
}

void CanvasObject::render(Context* context) {
  if (mesh) {
    mesh->transform = MatrixTranslation(Vec3(x, y, 0)) * MatrixScaling(Vec3(xScale, yScale, 1)) * MatrixRotation(Vec3(0, 0, rotation));
    context->draw(mesh);
  }
}

void CanvasObject::insert(CanvasObjectPtr& child) {
  children.push_back(child);
}

void CanvasObject::remove(CanvasObjectPtr& child) {
  children.remove(child);
}

Spritesheet::Spritesheet(const TexturePtr& texture, const vector<Rect>& rects) {
  quad.reset(new Quad(rects, texture, rects, false));
  index = 0;
/*
  this->setIndex(indexOffset+0, vertexOffset + 0);
  this->setIndex(indexOffset+1, vertexOffset + 1);
  this->setIndex(indexOffset+2, vertexOffset + 2);
  this->setIndex(indexOffset+3, vertexOffset + 2);
  this->setIndex(indexOffset+4, vertexOffset + 3);
  this->setIndex(indexOffset+5, vertexOffset + 0);
*/
}

void Spritesheet::render(Context* context) {
  if (quad) {
    quad->createIndices(index);
    quad->transform = MatrixTranslation(Vec3(x, y, 0)) * MatrixScaling(Vec3(xScale, yScale, 1)) * MatrixRotation(Vec3(0, 0, rotation));
    context->draw(quad);
  }
}

Canvas::Canvas()
{
  colorShader = bundle.loadShader("color");
  textureShader = bundle.loadShader("texture");
  defaultColor = whiteColor;
}

CanvasObjectPtr Canvas::newRect(float x, float y, float width, float height, CanvasObjectPtr parent) {
  MeshPtr mesh = Quad::create(Rect(0, 0, width, height));
  mesh->material->shader = colorShader;
  mesh->material->color = defaultColor;
  CanvasObjectPtr result(new CanvasObject(mesh));
  result->x = x;
  result->y = y;
  if (parent) {
    parent->insert(result);
  }
  else {
    insert(result);
  }
  return result;
}

CanvasObjectPtr Canvas::newImage(const string& path, float x, float y, CanvasObjectPtr parent) {
	TexturePtr texture = bundle.loadTexture(path);
  MeshPtr texturedQuad = Quad::create(texture);
  texturedQuad->material->shader = textureShader;
  texturedQuad->material->color = defaultColor;
  texturedQuad->material->blendNormal();
  CanvasObjectPtr result(new CanvasObject(texturedQuad));
  result->x = x;
  result->y = y;
  if (parent) {
    parent->insert(result);
  }
  else {
    insert(result);
  }
  return result;
}

CanvasObjectPtr Canvas::newText(const string& text, float x, float y, float width,
                                const string& font, int fontSize, TextAlign align,
                                CanvasObjectPtr parent) {
  TruetypeFontPtr ttf(new TruetypeFont(bundle.load(font), fontSize));
  TextMeshPtr mesh;
  if (width > 0) {
    mesh.reset(new TextMesh());
    TextBuffer tb;
    tb.font(ttf);
    tb.width(width);
    tb.breakMode(BREAKMODE_WORD);
    tb.text(text);
    tb.setAlign(align);
    tb.reset();
    tb.renderAllPhysicalLines(mesh);
  }
  else {
    mesh = lost::render(text, ttf, false);
  }
  mesh->material->shader = textureShader;
  mesh->material->blendPremultiplied();
  CanvasObjectPtr result(new CanvasObject(mesh));
  result->x = x;
  result->y = y;
  if (parent) {
    parent->insert(result);
  }
  else {
    insert(result);
  }
  return result;
}
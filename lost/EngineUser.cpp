#include "lost/Engine.h"
#include "lost/Log.h"
#include "lost/Bundle.h"
#include "lost/Bitmap.h"
#include "lost/Texture.h"
#include "lost/Context.h"
#include "lost/BufferLayout.h"
#include "lost/Mesh.h"
#include "lost/HybridIndexBuffer.h"
#include "lost/HybridVertexBuffer.h"
#include "lost/Camera2D.h"
#include "lost/TruetypeFont.h"
#include "lost/TextRender.h"
#include "lost/TextMesh.h"
#include "lost/Material.h"
#include "lost/TextBuffer.h"
#include <cmath>

namespace lost 
{

using namespace std;

MeshPtr coloredQuad;
MeshPtr texturedQuad;
ShaderProgramPtr colorShader;
ShaderProgramPtr textureShader;
ShaderProgramPtr pointShader;
CameraPtr cam;
TexturePtr ringTexture;
FontPtr font;
TextMeshPtr rt1;
TextMeshPtr rt2;

MeshPtr dot;
MeshPtr dot2;

MeshPtr lines;
MeshPtr spline;


MeshPtr newLineStrip(uint16_t numLines)
{
  MeshPtr result;

  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position);
  result = Mesh::create(layout, ET_u16);
  result->indexBuffer->drawMode = GL_LINE_STRIP;

  uint32_t numVertices = numLines+1;
  uint32_t numIndices = numVertices;
  
  result->vertexBuffer->reset(numVertices);
  result->indexBuffer->reset(numIndices);
  
  for(uint16_t i=0; i<numIndices; ++i)
  {
    result->set(i, UT_index, i);
  }
  
  result->material->color = yellowColor;
  result->material->shader = colorShader;
  
  return result;
}


void updateSplineSegment(vector<Vec2>&        interpolated, // receives the interpolated points
                         uint32_t             pointOffset,  // offset write position into interpolated points. The current segment points will be written at pointOffset onwards
                         uint32_t             numPoints,    // number of points for this segment
                         const vector<Vec2>&  cp,           // control points (all of them)
                         uint32_t             cpoffset)     // offset into control points. Four points after and including cp[0+cpoffset] will be used
{
  Vec2 cp0 = cp[cpoffset+0];
  Vec2 cp1 = cp[cpoffset+1];
  Vec2 cp2 = cp[cpoffset+2];
  Vec2 cp3 = cp[cpoffset+3];

  f32 t = 0;
  f32 dt = 1.0f/(numPoints-1);
  for(uint32_t i=0; i<numPoints; ++i)
  {
    f32 b0 = .5f*(-powf(t,3)+2*powf(t,2)-t);
    f32 b1 = .5f*(3*powf(t,3)-5*powf(t,2)+2);
    f32 b2 = .5f*(-3*powf(t,3)+4*powf(t,2)+t);
    f32 b3 = .5f*(powf(t,3) - powf(t,2));
    Vec2 pt = b0*cp0 + b1*cp1 + b2*cp2 + b3*cp3;
    interpolated[pointOffset+i] = pt;
    t += dt;
  }
}

void updateSpline(const vector<Vec2>& cp, MeshPtr& lineMesh)
{
  // assumes 4 control points
/*  uint32_t numVertices = lineMesh->numVertices();
  f32 stepSize = 1.0f/(numVertices-1);
  f32 t = 0;
  for(uint32_t i=0; i<numVertices; ++i)
  {
    f32 b0 = .5f*(-powf(t,3)+2*powf(t,2)-t);
    f32 b1 = .5f*(3*powf(t,3)-5*powf(t,2)+2);
    f32 b2 = .5f*(-3*powf(t,3)+4*powf(t,2)+t);
    f32 b3 = .5f*(powf(t,3) - powf(t,2));
    Vec2 pt = b0*cp[0] + b1*cp[1] + b2*cp[2] + b3*cp[3];
    t += stepSize;
    lineMesh->set(i, UT_position, pt);
  }*/
  
  uint32_t numVertices = lineMesh->numVertices();
  vector<Vec2> ip;
  ip.reserve(numVertices);
  updateSplineSegment(ip, 0, numVertices, cp, 0);
  for(uint32_t i=0; i<numVertices; ++i)
  {
    lineMesh->set(i, UT_position, ip[i]);
  }
}




void Engine::startup()
{
  ResourceBundle mainBundle;
  colorShader = mainBundle.loadShader("resources/glsl/color");
  textureShader = mainBundle.loadShader("resources/glsl/texture");
	ringTexture = mainBundle.loadTexture("resources/rings.png");
  cam = Camera2D::create(Rect(0,0,1024,768));
  
  DataPtr fontData = mainBundle.load("resources/fonts/vera/Vera.ttf");
  font.reset(new TruetypeFont(fontData, 18));

  rt2.reset(new TextMesh());
  TextBuffer tb;
  tb.font(font);
  tb.width(200);
  tb.breakMode(BREAKMODE_WORD);
  tb.text("This here is a relatively long text, that's hopefully gonna be rendered in multiple lines.");
  tb.setAlign(2);
  tb.reset();
  tb.renderAllPhysicalLines(rt2);
  rt2->material->blendPremultiplied();
  rt2->material->shader = textureShader;
  rt2->transform = MatrixTranslation(Vec3(200,200,0));

  rt1 = render("I vant to drink your blood!", font, false);
  rt1->material->shader = textureShader;
  rt1->material->blendPremultiplied();
  rt1->transform = MatrixTranslation(Vec3(50,50,0));



  coloredQuad = Quad::create(Rect(0,0,50,50));
  coloredQuad->material->shader = colorShader;
  coloredQuad->material->color = redColor;

  texturedQuad = Quad::create(ringTexture);
  texturedQuad->material->shader = textureShader;
  texturedQuad->material->color = whiteColor;
  texturedQuad->material->blendNormal();
  texturedQuad->transform = MatrixTranslation(Vec3(100,100,0));
  
  dot = Quad::create(Rect(0,0,3,3));
  dot->material->shader = colorShader;
  dot->material->color = greenColor;

  dot->transform = MatrixTranslation(Vec3(10,10,0));

  dot2 = dot->clone();
  dot2->transform = MatrixTranslation(Vec3(30,30,0));

  lines = newLineStrip(3);
  lines->set(0, UT_position, Vec2(0,0));
  lines->set(1, UT_position, Vec2(10,10));
  lines->set(2, UT_position, Vec2(20,30));
  lines->set(3, UT_position, Vec2(40,50));
  
  spline = newLineStrip(200);
  vector<Vec2> cp;
  cp.push_back(Vec2(0,0));
  cp.push_back(Vec2(50,400));
  cp.push_back(Vec2(400,30));
  cp.push_back(Vec2(450,30));
  updateSpline(cp, spline);
  
}

void Engine::update()
{
  glContext->clearColor(blackColor);
  glContext->camera(cam);
  glContext->clear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
//  glContext->draw(coloredQuad);
//  glContext->draw(texturedQuad);
//  glContext->draw(rt1);
//  glContext->draw(rt2);
//  glContext->draw(dot);
//  glContext->draw(dot2);
//  glContext->draw(lines);
  glContext->draw(spline);
}

void Engine::shutdown()
{
}

}


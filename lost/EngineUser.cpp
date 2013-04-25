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

#include "guiro/UserInterface.h"
#include "lost/ResourceManager.h"

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
MeshPtr normals;

vector<MeshPtr> cpdots;
vector<MeshPtr> ipdots;

const uint32_t dotsize = 5;


UserInterfacePtr ui;

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

MeshPtr newLineGroup(uint16_t numLines)
{
  MeshPtr result;

  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position);
  result = Mesh::create(layout, ET_u16);
  result->indexBuffer->drawMode = GL_LINES;

  uint32_t numVertices = numLines*2;
  uint32_t numIndices = numVertices;
  
  result->vertexBuffer->reset(numVertices);
  result->indexBuffer->reset(numIndices);
  
  for(uint16_t i=0; i<numIndices; ++i)
  {
    result->set(i, UT_index, i);
  }
  
  result->material->color = greenColor;
  result->material->shader = colorShader;
  
  return result;
}

Vec2 cr(f32 t, const Vec2& cp0, const Vec2& cp1, const Vec2& cp2, const Vec2& cp3)
{
  f32 b0 = .5f*(-powf(t,3)+2*powf(t,2)-t);
  f32 b1 = .5f*(3*powf(t,3)-5*powf(t,2)+2);
  f32 b2 = .5f*(-3*powf(t,3)+4*powf(t,2)+t);
  f32 b3 = .5f*(powf(t,3) - powf(t,2));
  Vec2 pt = b0*cp0 + b1*cp1 + b2*cp2 + b3*cp3;
  return pt;
}

Vec2 crderiv(float t, const Vec2& cp0, const Vec2& cp1, const Vec2& cp2, const Vec2& cp3)
{
  Vec2 result;
  
  result = -(3.0f/2.0f)*cp0*powf(t,2) + 2.0f*cp0*t - (0.5f)*cp0
           + 0.5f*cp1*powf(t,2) - 5*cp1*t
           - 0.5f*cp2 + 4*cp2 + 0.5f*cp2
           + (3.0f/2.0f)*cp3*powf(t,2) - cp3;
  
  return result;
}


void updateSplineSegment(vector<Vec2>&        interpolated, // receives the interpolated points
                         vector<Vec2>&        tangents, // receives tangent vector at this point
                         uint32_t             pointOffset,  // offset write position into interpolated points. The current segment points will be written at pointOffset onwards
                         uint32_t             numPoints,    // number of points for this segment
                         const vector<Vec2>&  cp,           // control points (all of them)
                         uint32_t             cpoffset)     // offset into control points. Four points after and including cp[0+cpoffset] will be used
{
  Vec2 cp0 = cp[cpoffset+0];
  Vec2 cp1 = cp[cpoffset+1];
  Vec2 cp2 = cp[cpoffset+2];
  Vec2 cp3 = cp[cpoffset+3];

  DOUT("seglen "<<len(cp2-cp1));

  f32 t = 0;
  f32 dt = 1.0f/(numPoints-1);
  for(uint32_t i=0; i<numPoints; ++i)
  {
    interpolated[pointOffset+i] = cr(t, cp0, cp1, cp2, cp3);
    tangents[pointOffset+i] = crderiv(t, cp0, cp1, cp2, cp3);
    t += dt;
  }
}

void updateSpline(const vector<Vec2>& cp, MeshPtr& lineMesh, MeshPtr& normalMesh)
{
  uint32_t numVertices = lineMesh->numVertices();
  vector<Vec2> ip; // interpolated points
  vector<Vec2> tv; // tangent vectors
  ip.reserve(numVertices);
  tv.reserve(numVertices);

  bool adaptive = false;
  
  // a minimum of 4 control points is required for the initial segment.
  // Each consecutive is made up by following point + 3 previous.
  uint32_t numSegments = ((uint32_t)cp.size()-4)+1;
  vector<uint32_t> pn; // number of points per segment
  pn.reserve(numSegments);

  if(!adaptive)
  {
    // non-adaptive case: vertices spread evenly between segments
    // leftovers are attached to last segment
    uint32_t nps = numVertices / numSegments;
    uint32_t pbudget = numVertices;
    for(uint32_t i=0; i<numSegments; ++i)
    {
      if(2*nps > pbudget)
      {
        pn[i] = pbudget; // should only be the last one
      }
      else
      {
        pn[i] = nps;
        pbudget -= nps;
      }
      DOUT("seg "<<pn[i]);
    }
  }
  else
  {
    vector<f32> seglen;
    seglen.reserve(numSegments);
    f32 totalLength = 0;
    for(u32 i=0; i<numSegments; ++i)
    {
      seglen[i] = len(cp[i+2]-cp[i+1]);
      DOUT("seglen" << seglen[i]);
      totalLength += seglen[i];
    }
    
    u32 budget = numVertices;
    for(u32 i=0; i<numSegments; ++i)
    {
      if(i != (numSegments-1))
      {
        pn[i] = (seglen[i] / totalLength)*((f32)numVertices);
        budget -= pn[i];
      }
      else
      {
        pn[i] = budget;
      }
    }
  }
  
  uint32_t pointOffset = 0;
  for(uint32_t segnum = 0; segnum < numSegments; ++segnum)
  {
    updateSplineSegment(ip, tv, pointOffset, pn[segnum], cp, segnum);
    pointOffset += pn[segnum];
  }
  
  f32 normalLenght = 20;
  for(uint32_t i=0; i<numVertices; ++i)
  {
    // copy interpolated points into mesh
    lineMesh->set(i, UT_position, ip[i]);
    normalMesh->set(i*2,UT_position, ip[i]);
    
    Vec2 nv;
    nv.x = -tv[i].y;
    nv.y = tv[i].x;
    normalise(nv);
    normalMesh->set(i*2+1, UT_position, ip[i]+nv*normalLenght);
    
    // visualize interpolated points with quads for debugging
    MeshPtr p = dot->clone();
    p->transform = MatrixTranslation(Vec3(ip[i].x-(dotsize/2), ip[i].y-(dotsize/2), 0));
    p->material = dot->material->clone();
    p->material->color = redColor;
    ipdots.push_back(p);
    
    
  }
}


void Engine::startup()
{
  ResourceBundle mainBundle;
  colorShader = resourceManager->shader("resources/glsl/color");
  textureShader = resourceManager->shader("resources/glsl/texture");
	ringTexture = resourceManager->texture("resources/images/rings.png");
  cam = Camera2D::create(Rect(0,0,1024,768));
  
  DataPtr fontData = mainBundle.load("resources/fonts/vera/Vera.ttf");
  font.reset(new TruetypeFont(fontData, 18));
    
  resourceManager->logStats();

  ui.reset(new UserInterface);

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
  
  dot = Quad::create(Rect(0,0,dotsize,dotsize));
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
  
  spline = newLineStrip(60);
  normals = newLineGroup(spline->numVertices());
  vector<Vec2> cp;
  cp.push_back(Vec2(110,110));
  cp.push_back(Vec2(110,110));
  cp.push_back(Vec2(310,310));
  cp.push_back(Vec2(610,110));
  cp.push_back(Vec2(710,510));
  cp.push_back(Vec2(110,510));
  cp.push_back(Vec2(110,510));
/*  cp.push_back(Vec2(200,200));
  cp.push_back(Vec2(20,200));
  cp.push_back(Vec2(110,110));
  cp.push_back(Vec2(110,110));*/
  updateSpline(cp, spline, normals);
  
  for(uint32_t i=0; i<cp.size(); ++i)
  {
    MeshPtr p = dot->clone();
    p->transform = MatrixTranslation(Vec3(cp[i].x-(dotsize/2), cp[i].y-(dotsize/2), 0));
    cpdots.push_back(p);
  }
  
}

void Engine::update()
{
  glContext->clearColor(blackColor);
  glContext->camera(cam);
  glContext->clear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

//  glContext->draw(coloredQuad);
  glContext->draw(texturedQuad);
  glContext->draw(rt1);
//  glContext->draw(rt2);
//  glContext->draw(dot);
//  glContext->draw(dot2);
//  glContext->draw(lines);
  glContext->draw(spline);
  glContext->draw(normals);

  for(uint32_t i=0; i<ipdots.size(); ++i)
  {
    glContext->draw(ipdots[i]);
  }
  
  for(uint32_t i=0; i<cpdots.size(); ++i)
  {
    glContext->draw(cpdots[i]);
  }
  
  ui->update();
  ui->draw(glContext);
}

void Engine::shutdown()
{
}

}


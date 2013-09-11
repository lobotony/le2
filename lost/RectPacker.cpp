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

#include "RectPacker.h"
#include <algorithm>

namespace lost
{
  
RectPacker::Node::Node()
{
  rectid = -1;
  rotated = false;
  tiny = -1;
  large = -1;
}

RectPacker::RectPacker()
{
}

RectPacker::~RectPacker()
{
}

void RectPacker::checkOverlap()
{
  for(uint32_t i=0; i<nodes.size(); ++i)
  {
    Rect& curRect = nodes[i].rect;
    for(uint32_t j=0; j<nodes.size(); ++j)
    {
      if(i!=j)
      {
        if(curRect.intersects(nodes[j].rect))
        {
          DOUT("OVERLAP");
        }
      }
    }    
  }
}

bool compareRects(const RectPacker::SourceRect& r1, const RectPacker::SourceRect& r2)
{
  return (r1.rect.area() > r2.rect.area());
}

void RectPacker::pack(const Rect& targetArea,
          const vector<Rect>& rects,
          bool rotate,
          bool sort)
{
  // clear stats
  sumIter = 0;
  sumNodes = 0;
  
  // clear previous result and init to target area
  nodes.clear();
  Node rootNode;
  rootNode.rect = targetArea;
  nodes.push_back(rootNode);
  
  // create the source vector and preserve incming ids in the progress
  // by wrapping the incoming rects in the SourceRect struct
  sourceRects.clear();
  int32_t numRects = (int32_t)rects.size();
  for(int32_t i=0; i<numRects; ++i)
  {
    sourceRects.push_back(SourceRect(rects[i], i));
  }
  
  // sort the source rects by size for better fitting
  if(sort)
    std::sort(sourceRects.begin(), sourceRects.end(), compareRects);
    
  // pack all source rects into the target area
  for(int32_t i=0; i<numRects; ++i)
  {
    if(!pack(i))
    {
      DOUT("couldn't pack "<<i);
    }
  }
  
  // clean up temporary data
  sourceRects.clear();
  recursionStack.clear();
//  DOUT("searched "<<(u32)((sumIter/sumNodes)*100)<<"% of all nodes");
//  DOUT(" iterations: "<<sumIter<<" nodes:"<<sumNodes);
}

bool RectPacker::exactMatch(const Node& n, const SourceRect& s)
{
  return ((n.rect.width == s.rect.width) && (n.rect.height == s.rect.height));
}

bool RectPacker::fits(const Node& n, const SourceRect& s)
{
  return ((n.rect.width >= s.rect.width) && (n.rect.height >= s.rect.height));
}


bool RectPacker::pack(int32_t sourceRectId)
{
  // try to find an exactly matching node for the given sourceRectId
  // if there is none, we split the current node until we find one
  foundNode = false; // will be set to true if we actually can create a macthing node
  recursionStack.clear();
  recursionStack.push_back(0); // start at the rootNode

  SourceRect& currentSourceRect = sourceRects[sourceRectId];
  // check if current rect is degenerate and abort immediately
  if((currentSourceRect.rect.width == 0) || (currentSourceRect.rect.height == 0))
  {
//    DOUT("rejecting degenerate rect");
    return false;
  }
//  DOUT("------------------ "<<sourceRectId<<" "<<currentSourceRect.rect);


  // loop while we haven't found a match and still have space
  // the last entry in the stack will be the node we have to work with
  uint32_t iterations = 0;
  while(!foundNode && (recursionStack.size() > 0))
  {
    iterations++;
    int32_t currentNodeIndex = recursionStack.back();
    Node currentNode = nodes[currentNodeIndex]; // currentNode needs to be copied since we change the vector and the memory might be moved
//    DOUT("currentNodeIndex: "<<currentNodeIndex<<" stack size: "<<recursionStack.size()<<" rect: "<<currentNode.rect);
    
    // terminate if the currentNode is not in use yet and matches exactly
    if(!currentNode.hasBitmap() && !currentNode.hasChildren() && exactMatch(currentNode, currentSourceRect))
    {
        foundNode = true;
//        currentNode.rect = currentSourceRect.rect;
        currentNode.rectid = currentSourceRect.id;
//        DOUT("///////////////////////////////////////// found match, stack size: "<<recursionStack.size());
        nodes[currentNodeIndex] = currentNode;
    }
    else
    {
      // only split and recurse if node is not in use yet and sourceRect fits into this node
      if((currentNode.rectid == -1) && fits(currentNode, currentSourceRect))
      {
        // make sure currentNode has children
        split(currentNode, currentSourceRect);
        // pop current node and add children if existent
        // add smaller child last so it will get checked first
        recursionStack.pop_back();
        if((currentNode.large != -1) && (nodes[currentNode.large].rectid==-1)) recursionStack.push_back(currentNode.large);
        if((currentNode.tiny != -1) && (nodes[currentNode.tiny].rectid==-1)) recursionStack.push_back(currentNode.tiny);
//        DOUT("stack size after split: "<<recursionStack.size());
//        DOUT("node ids after split: "<<currentNode.tiny << " " << currentNode.large);
        nodes[currentNodeIndex] = currentNode;
      }
      else
      {
        // just pop the node off the stack and continue
//        DOUT("won't split node that's already in use");
        recursionStack.pop_back();
      }
    }
  }  
//  DOUT("iterations: "<<iterations<<" nodes: "<<nodes.size());
  sumIter += iterations;
  sumNodes += nodes.size();
//  checkOverlap();
  return foundNode;
}

int32_t RectPacker::newNode(const Rect& r)
{
//  DOUT("creating node with rect: "<<r);
  nodes.push_back(Node(r));
  int32_t newNodeIndex = (int32_t)nodes.size();
  newNodeIndex -= 1;
  return newNodeIndex;
}

void RectPacker::split(Node& n, const SourceRect& s)
{
  Rect noderect = n.rect;
  Rect bmprect = s.rect;
  
  float vsplitmax, hsplitmax;
  Rect vl, vr, ht, hb;

  if((n.tiny != -1) || (n.large != -1))
  {
//    DOUT("already split");
    return;
  }

  if(noderect.width <= bmprect.width) // don't vsplit if width is already minimum fit for bitmap
  {
    vsplitmax = 0;
  }
  else
  {
    // setup rects for vertical split
    vl = Rect(noderect.x, noderect.y, bmprect.width, noderect.height);  // vertical split left rect
    vr = Rect(noderect.x+bmprect.width, noderect.y, noderect.width-bmprect.width, noderect.height); // vertical split right rect
    // find max child area for vsplit
    vsplitmax = std::max(vl.area(), vr.area());
  }

  if(noderect.height <= bmprect.height) // don't split if height is aready minimum fit for bitmap
  {
    hsplitmax = 0;
  }
  else
  {
    // setup rects for horizontal split
    hb = Rect(noderect.x, noderect.y, noderect.width, bmprect.height);
    ht = Rect(noderect.x, noderect.y+bmprect.height, noderect.width, noderect.height-bmprect.height);
    // find max child area for hsplit
    hsplitmax = std::max(ht.area(), hb.area());
  }

  if((vsplitmax == 0) && (vsplitmax == hsplitmax))
  {
//    DOUT("won't split this node!");
    return;
  }

  if(vsplitmax > hsplitmax)
  {
    if(vl.area() < vr.area())
    {
      n.tiny = newNode(vl);
      n.large = newNode(vr);
    }
    else
    {
      n.tiny = newNode(vr);
      n.large = newNode(vl);
    }
  }
  else
  {
    if(hb.area() < ht.area())
    {
      n.tiny = newNode(hb);
      n.large = newNode(ht);
    }
    else
    {
      n.tiny = newNode(ht);
      n.large = newNode(hb);
    }
  }
  
}
  
}


#include "apps/UiTestApp.h"
#include "lost/Context.h"
#include "guiro/UserInterface.h"

namespace lost
{

void UiTestApp::startup()
{
  ui->enable();

  ui->rootView->layer->backgroundColor = clearColor;
  
  LayerPtr sl1(new Layer);
  sl1->rect(Rect(50,50,200,20));
  sl1->backgroundColor = greenColor;
  
  LayerPtr sl2(new Layer);
  sl2->rect(Rect(75,100,30,40));
  sl2->backgroundColor = yellowColor;

  LayerPtr sl3(new Layer);
  sl3->rect(Rect(0,0,90,10));
  sl3->backgroundColor = blueColor;
  
  ui->rootView->layer->addSublayer(sl1);
  ui->rootView->layer->addSublayer(sl2);
  sl2->addSublayer(sl3);
  
  DOUT("root Z:"<<ui->rootView->layer->z(););
  DOUT("sl1 Z:"<<sl1->z());
  DOUT("sl2 Z:"<<sl2->z());
  DOUT("sl3 Z:"<<sl3->z());
  
  sl3->needsRedraw();
  
  #define SZDOUT(c) DOUT("sizeof("<<#c<<") = "<<u64(sizeof(c)));
  SZDOUT(Layer);
  SZDOUT(LayerPtr);
  SZDOUT(string);
  SZDOUT(vector<LayerPtr>);
  SZDOUT(View);
  SZDOUT(Color);
  SZDOUT(Frame);
}

void UiTestApp::update()
{
  glContext->clearColor(Color(0,0,0,1));
  glContext->clear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
}

void UiTestApp::shutdown()
{
}

}

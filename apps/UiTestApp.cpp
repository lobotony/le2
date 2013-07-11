#include "apps/UiTestApp.h"
#include "lost/Context.h"
#include "lost/UserInterface.h"
#include "lost/layers/TextLayer.h"
#include "lost/ResourceManager.h"
#include "lost/Event.h"

namespace lost
{
UiTestApp::UiTestApp() : Application("resources/le2config.json")
{
}

void UiTestApp::startup()
{
  ui->enable();

  ui->rootView->layer->backgroundColor(clearColor);
  ui->rootView->layer->name = "root";
  
  LayerPtr sl1(new Layer);
  sl1->rect(Rect(50,50,50,50));
  sl1->backgroundColor(greenColor);
  sl1->name = "green";
  sl1->cornerRadius(4);
  
  LayerPtr sl2(new Layer);
  sl2->rect(Rect(75,100,30,40));
  sl2->backgroundColor(yellowColor);
  sl2->name = "yellow";
  sl2->cornerRadius(12);

  LayerPtr sl3(new Layer);
  sl3->rect(Rect(0,0,90,10));
  sl3->backgroundColor(blueColor);
  sl3->name = "blue";
  sl3->cornerRadius(4);

  LayerPtr sl4(new Layer);
  sl4->rect(Rect(100,50,50,50));
  sl4->backgroundColor(whiteColor);
  sl4->name = "reddy";
  sl4->cornerRadius(20);
  sl4->borderColor(blueColor);
  sl4->borderWidth(1);

  LayerPtr sl5(new Layer);
  sl5->rect(Rect(100,100,201,199));
  sl5->backgroundColor(whiteColor);
  sl5->name = "rings";
  sl5->backgroundImage(resourceManager->texture("resources/images/rings.png"));
  
  ui->rootView->layer->addSublayer(sl1);
  ui->rootView->layer->addSublayer(sl2);
  sl2->addSublayer(sl3);
  ui->rootView->layer->addSublayer(sl4);
  ui->rootView->layer->addSublayer(sl5);
  
    

  DOUT("root Z:"<<ui->rootView->layer->z(););
  DOUT("sl1 Z:"<<sl1->z());
  DOUT("sl2 Z:"<<sl2->z());
  DOUT("sl3 Z:"<<sl3->z());

  #define SZDOUT(c) DOUT("sizeof("<<#c<<") = "<<u64(sizeof(c)));
  SZDOUT(Layer);
  SZDOUT(LayerPtr);
  SZDOUT(string);
  SZDOUT(vector<LayerPtr>);
  SZDOUT(View);
  SZDOUT(Color);
  SZDOUT(Frame);
  
  resourceManager->registerFontBundle("resources/fonts/vera.lefont");
  FontPtr font = resourceManager->font("Vera", 12);

  {
    TextLayerPtr tl(new TextLayer);
    tl->font(font);
    tl->text("Hello!");
    tl->name = tl->text();
    tl->rect(10,10,50,20);
    tl->textColor(blackColor);
    tl->backgroundColor(whiteColor);
    tl->cornerRadius(8);
    ui->rootView->layer->addSublayer(tl);
  }
  {
    TextLayerPtr tl(new TextLayer);
    tl->font(font);
    tl->text("Second");
    tl->name = tl->text();
    tl->rect(75,10,50,20);
    tl->textColor(blackColor);
    tl->backgroundColor(whiteColor);
    tl->cornerRadius(8);
    ui->rootView->layer->addSublayer(tl);
  }
  {
    TextLayerPtr tl(new TextLayer);
    tl->font(font);
    tl->text("More text, wrapping?");
    tl->name = tl->text();
    tl->rect(10,100,50,20);
    tl->textColor(blackColor);
    tl->backgroundColor(whiteColor);
    tl->cornerRadius(8);
    ui->rootView->layer->addSublayer(tl);
  }
  {
    TextLayerPtr tl(new TextLayer);
    tl->font(font);
    tl->text("Batman");
    tl->name = tl->text();
    tl->rect(100,100,50,20);
    tl->textColor(blackColor);
    tl->backgroundColor(whiteColor);
    tl->cornerRadius(8);
    ui->rootView->layer->addSublayer(tl);
  }
  first = true;
  logged = false;
  
  
  ViewPtr v1(new View);
  ViewPtr v2(new View);
  v1->addSubview(v2);
  ViewPtr v3(new View);
  ui->rootView->addSubview(v1);
  ui->rootView->addSubview(v3);
  
  v1->name="red";
  v1->rect(0, 500, 200, 100);
  v1->layer->backgroundColor(redColor);
  
  v2->name="green";
  v2->rect(10, 10, 50, 50);
  v2->layer->backgroundColor(greenColor);
  
  v3->name = "yellow";
  v3->rect(300, 500, 140, 80);
  v3->layer->backgroundColor(yellowColor);
  
  cursor.reset(new Layer);
  cursor->pos(Vec2(0,0));
  cursor->size(Vec2(20,20));
  cursor->backgroundColor(whiteColor);
  cursor->name="cursor";
  ui->rootView->layer->addSublayer(cursor);
  
  auto poslogger = [this](Event* ev)
  {
    DOUT("move "<<ev->mouseEvent.x<<" "<<ev->mouseEvent.y);
    cursor->pos(Vec2(ev->mouseEvent.x, ev->mouseEvent.y));
  };
  ui->rootView->addEventHandler(ET_MouseMoveEvent,poslogger,EP_Capture);
  ui->rootView->addEventHandler(ET_MouseMoveEvent,poslogger,EP_Target);
}

void UiTestApp::update()
{
  glContext->clearColor(Color(.57,.57,.57,1));
  glContext->clear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
  if(!first && !logged)
  {
    Application::instance()->resourceManager->logStats();
    logged = true;
  }
  if(first)
  {
    first = false;
  }
}

void UiTestApp::shutdown()
{
}

}

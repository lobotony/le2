#include "apps/UiTestApp.h"
#include "lost/Context.h"
#include "lost/UserInterface.h"
#include "lost/layers/TextLayer.h"
#include "lost/ResourceManager.h"
#include "lost/Event.h"
#include "lost/views/Label.h"
#include "lost/views/ImageView.h"

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
  sl5->rect(Rect(130,130,165,165));
  sl5->backgroundColor(whiteColor);
  sl5->name = "arrow";
  sl5->backgroundImage(resourceManager->image("resources/images/arrow.png"));
  sl5->backgroundImage()->orientation = ImageOrientationUp;
  
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
  resourceManager->registerFontBundle("resources/fonts/antonio.lefont");
  FontPtr font = resourceManager->font("Vera", 12);
  FontPtr largeFont = resourceManager->font("Antonio bold", 18);
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

  {
    TextLayerPtr tl(new TextLayer);
    tl->font(largeFont);
    
    string txt =
"Then throw her in the laundry room, which will hereafter be referred to as 'the brig'. But I know you in the future. I cleaned your poop. This opera's as lousy as it is brilliant! Your lyrics lack subtlety. You can't just have your characters announce how they feel. That makes me feel angry!\n\nOh, I don't have time for this. I have to go and buy a single piece of fruit with a coupon and then return it, making people wait behind me while I complain. I usually try to keep my sadness pent up inside where it can fester quietly as a mental illness. I am Singing Wind, Chief of the Martians.\n\nYou'll have all the Slurm you can drink when you're partying with Slurms McKenzie! Yes, I saw. You were doing well, until everyone died. Bender, we're trying our best.\n\nThat's a popular name today. Little 'e', big 'B'? Fry! Quit doing the right thing, you jerk! Bender, we're trying our best. I'm Santa Claus! Good news, everyone! I've taught the toaster to feel love!\n\nDr. Zoidberg, that doesn't make sense. But, okay! Why, those are the Grunka-Lunkas! They work here in the Slurm factory. Fry! Quit doing the right thing, you jerk! I didn't ask for a completely reasonable excuse! I asked you to get busy! I am Singing Wind, Chief of the Martians.";
    
    tl->text(txt);
    tl->name = tl->text();
    tl->rect(300,40,400,400);
    tl->textColor(blackColor);
    tl->backgroundColor(whiteColor);
    tl->insets(Insets(10));
    tl->cornerRadius(0);
    tl->alignment(TextAlignmentLeft);
    tl->breakmode(BREAKMODE_WORD);
    tl->borderColor(blueColor);
    tl->borderWidth(1);
    
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
  
  v1->name("red");
  v1->rect(0, 500, 200, 100);
  v1->layer->backgroundColor(redColor);
  
  v2->name("green");
  v2->rect(10, 10, 50, 50);
  v2->layer->backgroundColor(greenColor);
  
  v3->name("yellow");
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
//    DOUT("move "<<ev->mouseEvent.x<<" "<<ev->mouseEvent.y);
//    cursor->pos(Vec2(ev->mouseEvent.x, ev->mouseEvent.y));
  };
  
//  auto enterLogger = [this](Event* ev) { DOUT(ev->base.currentTarget->name()<<": enter "<<ev->base.target->name()<<" phase: "<<ev->base.phase); };
//  auto leaveLogger = [this](Event* ev) { DOUT(ev->base.currentTarget->name()<<": leave "<<ev->base.target->name()<<" phase: "<<ev->base.phase); };
  
//  auto mouseDownLogger = [this](Event* ev) { //DOUT(DOUT(ev->base.currentTarget->name()
    
  auto evLogger = [this](Event* ev) {if(!next) {DOUT("==================");next=true;}
                                      DOUT(ev->base.currentTarget->name()<<" "
                                        <<ev->base.target->name()<<" "
                                        <<eventTypeToString(ev->base.type)<<" "
                                        <<eventPhaseToString(ev->base.phase));
                                      if((ev->base.type == ET_FocusGained) || (ev->base.type == ET_FocusLost))
                                      {
                                        View* cf = ui->focusedView();
                                        DOUT("currently focused view: "<<(cf ? cf->name() : "NULL"));
                                      }
                                    };
  
  ui->rootView->addEventHandler(ET_MouseMove,poslogger,EP_Capture);
  ui->rootView->addEventHandler(ET_MouseMove,poslogger,EP_Target);
  
  ui->rootView->addEventHandler(ET_MouseEnter, evLogger, EP_Bubble);
  ui->rootView->addEventHandler(ET_MouseEnter, evLogger, EP_Target);
  ui->rootView->addEventHandler(ET_MouseLeave, evLogger, EP_Bubble);
  ui->rootView->addEventHandler(ET_MouseLeave, evLogger, EP_Target);
  ui->rootView->addEventHandler(ET_FocusGained, evLogger, EP_Bubble);
  ui->rootView->addEventHandler(ET_FocusGained, evLogger, EP_Target);
  ui->rootView->addEventHandler(ET_FocusLost, evLogger, EP_Bubble);
  ui->rootView->addEventHandler(ET_FocusLost, evLogger, EP_Target);

  ui->rootView->addEventHandler(ET_KeyUp, evLogger, EP_Bubble);
  ui->rootView->addEventHandler(ET_KeyUp, evLogger, EP_Target);
  ui->rootView->addEventHandler(ET_KeyDown, evLogger, EP_Bubble);
  ui->rootView->addEventHandler(ET_KeyDown, evLogger, EP_Target);

  v1->addEventHandler(ET_MouseEnter, evLogger, EP_Bubble);
  v1->addEventHandler(ET_MouseEnter, evLogger, EP_Target);
  v1->addEventHandler(ET_MouseLeave, evLogger, EP_Bubble);
  v1->addEventHandler(ET_MouseLeave, evLogger, EP_Target);

  v2->addEventHandler(ET_MouseEnter, evLogger, EP_Bubble);
  v2->addEventHandler(ET_MouseEnter, evLogger, EP_Target);
  v2->addEventHandler(ET_MouseLeave, evLogger, EP_Bubble);
  v2->addEventHandler(ET_MouseLeave, evLogger, EP_Target);

  v3->addEventHandler(ET_MouseEnter, evLogger, EP_Bubble);
  v3->addEventHandler(ET_MouseEnter, evLogger, EP_Target);
  v3->addEventHandler(ET_MouseLeave, evLogger, EP_Bubble);
  v3->addEventHandler(ET_MouseLeave, evLogger, EP_Target);

  LabelPtr label(new Label);
  label->font(largeFont);
  label->text("TESTING!!!!!");
  label->rect(200,200,100,100);
  label->addEventHandler(ET_MouseUpInside, [](Event* event){ DOUT("mouse up inside label"); }, EP_Target);
  ui->rootView->addSubview(label);

  ImageViewPtr iv(new ImageView(resourceManager->image("resources/images/rings.png")));
  iv->pos(200,300);
  ui->rootView->addSubview(iv);

  DOUT("current focus: "<<ui->focusedView()->name());
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
  next = false;
}

void UiTestApp::shutdown()
{
}

}

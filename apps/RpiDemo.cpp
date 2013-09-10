#include "apps/RpiDemo.h"
#include "lost/UserInterface.h"
#include "lost/ResourceManager.h"
#include "lost/views/ImageView.h"
#include "lost/views/Label.h"
#include "lost/views/Button.h"
#include "lost/Context.h"
#include "lost/ResourceManager.h"

namespace lost
{

RpiDemo::RpiDemo() : Application("resources/rpiDemoConfig.json")
{
  
}

void RpiDemo::startup()
{
  resourceManager->registerFontBundle("resources/fonts/weeblysleek.lefont");

  FontPtr headerFont = resourceManager->font("Weebly Sleek Ui Light", 94);
  FontPtr descFont = resourceManager->font("Weebly Sleek Ui Bold", 24);

  ui->enable();

  ui->rootView->layer->backgroundColor(clearColor);
  ui->rootView->layer->name = "root";
  
  ImagePtr image = resourceManager->image("resources/images/sunflowers.png");
  ImageViewPtr iv(new ImageView);
  iv->image(image);
  ui->rootView->addSubview(iv);
  
  f32 w = 400;
  
  ViewPtr rightBar(new View);
  ui->rootView->addSubview(rightBar);
  
  Color transBlackColor(0,0,0,.75);
  
  rightBar->backgroundColor(transBlackColor);
  rightBar->rect(windowSize.width-w, 0, w, windowSize.height);
 
  LabelPtr label(new Label);
  rightBar->addSubview(label);
  f32 lh=140;
  label->font(headerFont);
  label->text("Raspberry Pi");
  label->rect(0,windowSize.height-lh, w, lh);
  label->textColor(Color(1,1,1,1));
  label->backgroundColor(Color(0,0,0,0));
  
  descHeight = 220;
  f32 descWidth = windowSize.width-w;
  f32 is = 10;
  descContainer.reset(new View);
  ui->rootView->addSubview(descContainer);
  descContainer->backgroundColor(transBlackColor);
  descContainer->rect(0,0,descWidth, descHeight);
  LabelPtr descLabel(new Label);
  descContainer->addSubview(descLabel);
  descLabel->rect(is,is,descWidth-2*is, descHeight-2*is);
  descLabel->font(descFont);
  descLabel->backgroundColor(clearColor);
  descLabel->alignment(TextAlignmentLeft);
  descLabel->textColor(whiteColor);
  descLabel->text("Broadcom BCM2835 (CPU + GPU. SDRAM is a separate chip stacked on top), 700 MHz ARM11 ARM1176JZF-S core, Broadcom VideoCore IV,OpenGL ES 2.0,OpenVG 1080p30 H.264 high-profile encode/decode, RAM:  512 MiB, USB Ports: 2, Video Output: Composite video, Composite RCA, HDMI (not at the same time), Audio Outputs: TRS connector, 3.5 mm jack, HDMI, Audio Inputs: None, Oboard Storage: Secure Digital|SD / MMC / SDIO card slot, Onboard Network: 10/100 wired Ethernet RJ45, Low-level Peripherals: General Purpose Input/Output (GPIO) pins, Serial Peripheral Interface Bus (SPI), I²C, I²S[2], Universal asynchronous receiver/transmitter (UART), Real-time clock: None, Power ratings (provisional, from alpha board): 700 mA, (3.5 W), Power Source: 5 V (DC) via Micro USB type B or GPIO header, Size: 85.0 x 56.0 mm");
  
  f32 buttonWidth = w/2;
  
  Color buttonBbCol(1,1,1,.1);
  ButtonPtr touchButton(new Button);
  rightBar->addSubview(touchButton);
  touchButton->rect(0,0,buttonWidth,descHeight);
  touchButton->backgroundColor(buttonBbCol);
  touchButton->titleLabel->backgroundColor(clearColor);
  touchButton->titleLabel->textColor(whiteColor);
  touchButton->titleLabel->font(descFont);
  touchButton->imageView->backgroundColor(clearColor);
  touchButton->title("Touch", ButtonStateReleased);
  touchButton->title("Touch", ButtonStatePressed);
  touchButton->titleColor(whiteColor, ButtonStateReleased);
  touchButton->titleColor(blackColor, ButtonStatePressed);

  ButtonPtr descButton(new Button);
  rightBar->addSubview(descButton);
  descButton->rect(buttonWidth,0,buttonWidth,descHeight);
  descButton->backgroundColor(buttonBbCol);
  descButton->titleLabel->backgroundColor(clearColor);
  descButton->titleLabel->textColor(whiteColor);
  descButton->titleLabel->font(descFont);
  descButton->imageView->backgroundColor(clearColor);
  descButton->title("Specs", ButtonStateReleased);
  descButton->title("Specs", ButtonStatePressed);
  descButton->titleColor(whiteColor, ButtonStateReleased);
  descButton->titleColor(blackColor, ButtonStatePressed);

  touchButton->addAction([this](Button* b) {toggleTouchDemo();});
  descButton->addAction([this](Button* b) {toggleSpecs();});
  
  slidingIn = false;
  descContainer->pos(0,-descHeight);
  
  psize = 80;
  touchView.reset(new View);
  touchView->rect(200,600,psize,psize);
  touchView->layer->cornerRadius(8);
  touchView->layer->borderWidth(2);
  touchView->backgroundColor(transBlackColor);
  touchView->borderColor(whiteColor);
  
  ui->rootView->addSubview(touchView);
  
  offset = Vec2(0,descHeight);
  areaSize = Vec2(windowSize.width-w, windowSize.height);
  
  clickedView = false;
  downHandler = [this](Event* event)
  {
    clickedView = true;
//    DOUT(event->mouseEvent.x << " "<<event->mouseEvent.y);
  };

  moveHandler = [this](Event* event)
  {
    if(clickedView)
    {
      f32 x = event->mouseEvent.x;
      f32 y = event->mouseEvent.y;
      
      x = std::min(x,areaSize.x+offset.x);
      x = std::max(x,offset.x);
      x = floorf(x-(psize/2));
      y = std::min(y,areaSize.y+offset.y);
      y = std::max(y,offset.y);
      y = floorf(y-(psize/2));
      
      touchView->pos(Vec2(x, y));
    }
  };

  upHandler = [this](Event* event)
  {
    clickedView = NULL;
//    DOUT(eventTypeToString(event->base.type)<<" "<<eventPhaseToString(event->base.phase));
  };

  ui->rootView->addEventHandler(ET_MouseMove, moveHandler, EP_Target);
  ui->rootView->addEventHandler(ET_MouseMove, moveHandler, EP_Bubble);
  touchView->addEventHandler(ET_MouseDown, downHandler, EP_Target);
  ui->rootView->addEventHandler(ET_MouseUp, upHandler, EP_Target);
  ui->rootView->addEventHandler(ET_MouseUp, upHandler, EP_Bubble);
  touchView->visible(true);
}

void RpiDemo::toggleSpecs()
{
  DOUT("");
  
  bool animated = true;
  
  if(!slidingIn)
  {
    slidingIn = true;
    descContainer->layer->setValue("pos", Variant(Vec2(0,0)), animated);
  }
  else
  {
    slidingIn = false;
    descContainer->layer->setValue("pos", Variant(Vec2(0,-descHeight)), animated);
  }
}

void RpiDemo::toggleTouchDemo()
{
  DOUT("");
}


void RpiDemo::update()
{
  glContext->clearColor(Color(.57,.57,.57,1));
  glContext->clear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
}

void RpiDemo::shutdown()
{
}

}
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
  
  f32 descHeight = 220;
  f32 descWidth = windowSize.width-w;
  f32 is = 10;
  ViewPtr descContainer(new View);
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

}

void RpiDemo::toggleSpecs()
{
  
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
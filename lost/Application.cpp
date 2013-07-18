#include "lost/Application.h"
#include "lost/EventPool.h"
#include "lost/EventQueue.h"
#include "lost/Context.h"
#include "lost/ResourceManager.h"
#include "lost/UserInterface.h"

extern lost::Application* _appInstance; // must be set by runner implementation

namespace lost
{

Application* Application::instance()
{
  return _appInstance;
}

Application::Application(const char* configPath)
{
  if(configPath)
  {
    DOUT("loading config from: '"<<configPath<<"'");
    ResourceBundle mainBundle;
    config = mainBundle.loadJson(configPath);
  }
  
  eventPool = new EventPool;
  eventQueue = new EventQueue;
  glContext = NULL; // created in startup, after OS specific GLcontext was created
  resourceManager = NULL;
}

Application::~Application()
{
}

void Application::startup()
{
}

void Application::update()
{
}

void Application::shutdown()
{
}

void Application::doStartup()
{
  // Application startup
  glContext = new Context;
  resourceManager = new ResourceManager;
  ui = new UserInterface;
  // user startup
  startup();
}

void Application::doUpdate()
{
  clock.update();
  update();
  ui->processEvents(eventQueue->getCurrentQueue());
  ui->draw();
  eventQueue->swap();
}

void Application::doShutdown()
{
  // user shutdown
  shutdown();
  ui->disable();
  // Application shutdown
  delete glContext;
}


}
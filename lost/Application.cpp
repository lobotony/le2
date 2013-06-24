#include "lost/Application.h"
#include "lost/EventPool.h"
#include "lost/EventQueue.h"
#include "lost/Context.h"
#include "lost/ResourceManager.h"
#include "guiro/UserInterface.h"

extern lost::Application* _appInstance; // must be set by runner implementation

namespace lost
{

Application* Application::instance()
{
  return _appInstance;
}

Application::Application()
{
  eventPool = new EventPool;
  eventQueue = new EventQueue;
  glContext = NULL; // created in startup, after OS specific GLcontext was created
  resourceManager = NULL;// created in startup, after glContext was created, because it will use GL resources
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
  ui->update(eventQueue->getCurrentQueue());
  ui->draw();
  eventQueue->swap();
}

void Application::doShutdown()
{
  // user shutdown
  shutdown();
  // Application shutdown
  delete glContext;
}


}
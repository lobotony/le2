#include "lost/Engine.h"
#include "lost/EventPool.h"
#include "lost/EventQueue.h"
#include "lost/Context.h"
#include "lost/ResourceManager.h"

namespace lost
{

Engine::Engine()
{
  eventPool = new EventPool;
  eventQueue = new EventQueue;
  glContext = NULL; // created in startup, after OS specific GLcontext was created
  resourceManager = NULL;// created in startup, after glContext was created, because it will use GL resources
}

Engine::~Engine()
{
}

void Engine::startup()
{
}

void Engine::update()
{
}

void Engine::shutdown()
{
}

void Engine::doStartup()
{
  // engine startup
  glContext = new Context;
  resourceManager = new ResourceManager;
  // user startup
  startup();
}

void Engine::doUpdate()
{
  update();
  eventQueue->returnAllEvents();
}

void Engine::doShutdown()
{
  // user shutdown
  shutdown();
  // engine shutdown
  delete glContext;
}


}
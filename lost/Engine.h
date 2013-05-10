#ifndef LOST_ENGINE_H
#define LOST_ENGINE_H

namespace lost
{

struct EventPool;
struct EventQueue;
struct ResourceManager;
struct Context;
struct UserInterface;

struct Engine
{
  Engine();
  ~Engine();

  static Engine* instance();

  void startup();  // use provided, called by doStartup
  void update();   // user provided, called by doUpdate
  void shutdown(); // user provided, called by doShutdown
  
  void doStartup(); // called by OS specific code, performs engine and user startup
  void doUpdate(); // called by OS specific code, performs user update and housekeeping
  void doShutdown(); // called by OS specific code, performs user and engine shutdown
  
  EventPool*    eventPool; // global event pool, thread safe
  EventQueue*   eventQueue; // global event queue, fed by OS specific part, thread safe, user code reads events
  Context*      glContext;
  ResourceManager*  resourceManager;
  UserInterface* ui;
};

}

#endif


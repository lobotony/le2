#pragma once

namespace lost
{

struct EventPool;
struct EventQueue;
struct ResourceManager;
struct Context;
struct UserInterface;

struct Application
{
  Application(const char* configPath = NULL);
  ~Application();

  virtual void startup();  // user provided, called by doStartup
  virtual void update();   // user provided, called by doUpdate
  virtual void shutdown(); // user provided, called by doShutdown
  
  void doStartup(); // called by OS specific code, performs engine and user startup
  void doUpdate(); // called by OS specific code, performs user update and housekeeping
  void doShutdown(); // called by OS specific code, performs user and engine shutdown
  
  static Application* instance(); 
  
  Json::Value       config;
  
  Vec2              windowSize; // the current size of the applications operating system window. This will be updated before startup and after windowResize events.
  Clock             clock;
  EventPool*        eventPool; // global event pool, thread safe
  EventQueue*       eventQueue; // global event queue, fed by OS specific part, thread safe, user code reads events
  Context*          glContext;
  ResourceManager*  resourceManager;
  UserInterface*    ui;
};

}



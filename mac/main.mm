#import <Cocoa/Cocoa.h>
#import "LEAppDelegate.h"
#import "LEGLView.h"
#import <OpenGL/gl.h>
#import "lost/Engine.h"
#import "LEWindow.h"
#include "lost/Log.h"

NSOpenGLContext* glcontext = nil;

int main(int argc, char *argv[])
{
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init]; // first global one, required or we leak 
  NSApplication* app = [NSApplication sharedApplication];     // singleton
  LEAppDelegate* appDelegate = [[LEAppDelegate alloc] init];  // required for some important callbacks

  // setup very basic menu so we can at least CMD-Q quit
  NSMenu *menu;
  NSMenuItem *menu_item, *temp_item;
  NSString* title = nil;
  NSDictionary* app_dictionary = [[NSBundle mainBundle] infoDictionary];
  if (app_dictionary) 
  {
    title = [app_dictionary objectForKey: @"CFBundleName"];
  }
  if (title == nil) 
  {
    title = [[NSProcessInfo processInfo] processName];
  }
  NSMenu *main_menu = [[NSMenu allocWithZone: [NSMenu menuZone]] initWithTitle: @"temp"];
  [app setMainMenu: main_menu];
  menu = [[NSMenu allocWithZone: [NSMenu menuZone]] initWithTitle: @"temp"];
  temp_item = [[NSMenuItem allocWithZone: [NSMenu menuZone]]
               initWithTitle: @"temp"
               action: NULL
               keyEquivalent: @""];
  [[app mainMenu] addItem: temp_item];
  [[app mainMenu] setSubmenu: menu forItem: temp_item];
  NSString *quit = [@"Quit " stringByAppendingString: title];
  menu_item = [[NSMenuItem allocWithZone: [NSMenu menuZone]]
               initWithTitle: quit
               action: @selector(quitAction:)
               keyEquivalent: @"q"];
  [menu_item setKeyEquivalentModifierMask: NSCommandKeyMask];
  [menu_item setTarget: appDelegate];
  [menu addItem: menu_item];
  
  [menu_item release];
  [temp_item release];
  [menu release];
  [main_menu release];
  [app setDelegate: appDelegate];

  // setup window and GL view
  CGRect fr = CGRectMake(0, 0, 800, 600);
  const NSOpenGLPixelFormatAttribute windowedAttributes[] =
  {
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFAColorSize, [[[NSUserDefaults standardUserDefaults] objectForKey:@"colorDepth"] unsignedIntValue],
    NSOpenGLPFAAlphaSize, 8,
    NSOpenGLPFADepthSize, 32,
    0
  };  
  NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes: windowedAttributes];
  

  NSRect r;
  r.size.width = fr.size.width;
  r.size.height = fr.size.height;
  r.origin.y = fr.origin.y;
  r.origin.x = fr.origin.x;
  LEGLView* glview = [[[LEGLView alloc] initWithFrame:r pixelFormat:pixelFormat] autorelease];
  glcontext = [glview openGLContext];

  // make context current for this thread to be able to safely call startup
  [[glview openGLContext] makeCurrentContext];
  lost::Engine::instance()->doStartup();
  [NSOpenGLContext clearCurrentContext]; // then clear the context for this thread, so it's later only active on the render thread
  
  LEWindow* window = [[LEWindow alloc]
                      initWithContentRect: r
                      styleMask: NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask
                      backing: NSBackingStoreBuffered
                      defer: NO];
  [window setTitle: @"LostEngine 2"];
  [window setAcceptsMouseMovedEvents: YES];
  [window setContentView: glview];
  [window setDelegate: appDelegate];
  [window setReleasedWhenClosed: NO];
  [window makeKeyAndOrderFront:nil]; // shows and focuses the window
  appDelegate.window = window;
  [app run]; // runs the actual app, doesn't return
  
  [pool release];
  return EXIT_SUCCESS;
}

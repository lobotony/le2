#import <Cocoa/Cocoa.h>

@interface LEAppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>

@property (assign) IBOutlet NSWindow *window;

-(void)sendWindowResizeEvent;

@end

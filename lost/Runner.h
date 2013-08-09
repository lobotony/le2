#ifndef LOST_RUNNER_H
#define LOST_RUNNER_H


namespace lost
{

struct Application;

/** This is basically a cross plattform main() method that starts the provided application instance.
 * 
 * Implementations of the functions below must be provided at link time, i.e. you have to add the
 * correct platform-specific files to your project or makefile.
 *
 * Since the application can read an optional json configuration file, the runner can use the 
 * applications config file to e.g. configure the OpenGL context. 
 *
 * TODO: pass in command line arguments
 */

void run(Application* app); 

}

#endif


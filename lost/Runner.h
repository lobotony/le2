#ifndef LOST_RUNNER_H
#define LOST_RUNNER_H


namespace lost
{

struct Application;

/** This is basically a cross plattform main() method that either creates a demo app to run
 * if called with no parameters, or uses the provided app to startup a new application.
 * 
 * Implementations of the functions below must be provided at link time, i.e. you have to add the
 * correct platform-specific files to your project or makefile.
 *
 * TODO: pass in command line arguments and probably a configuration file as well to tweak 
 *       aspects of OpenGL context
 */

void run(Application* app); // uses provided app. Subclass Application and override as required.

}

#endif


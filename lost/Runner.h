#ifndef LOST_RUNNER_H
#define LOST_RUNNER_H


namespace lost
{

struct Engine;

/** This is basically a cross plattform main() method that either creates a demo engine to run
 * if called with no parameters, or uses the provided engine to startup a new application.
 * 
 * Implementations of the functions below must be provided at link time, i.e. you have to add the
 * correct platform-specific files to your project or makefile.
 *
 * TODO: pass in command line arguments and probably a configuration file as well to tweak 
 *       aspects of OpenGL context
 */

void run(); // creates and runs a demo engine
void run(Engine* engine); // uses provided engine. Subclass Engine and override as required.

}

#endif


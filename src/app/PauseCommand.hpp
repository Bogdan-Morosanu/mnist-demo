#ifndef DEMO_APP_PAUSE_COMMAND
#define DEMO_APP_PAUSE_COMMAND

#include "threads/ThreadRunner.hpp"

namespace app {

    class PauseCommand {
    public:
	
	explicit
	PauseCommand(thr::ThreadRunner &th_runner)
	    : runner(&th_runner)
	{ }
	
	std::string name() const { return "pause"; }
	
	void issue(int id)
	{
	    runner->pause(id - 1); // we map our application's ids [1 -> nb_threads]
	                           // into the thread runner's "natural" C++ ids
	}
	
    private:
	// this is a pointer (and not a reference) because we want
	// to be able to move and copy assign this object
	thr::ThreadRunner *runner;
    };
    
}

#endif

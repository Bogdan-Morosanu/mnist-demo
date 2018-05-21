#ifndef DEMO_APP_PAUSE_COMMAND
#define DEMO_APP_PAUSE_COMMAND

#include <string>
#include <cstdlib>

#include "AppDefs.hpp"

#include "threads/ThreadRunner.hpp"

namespace app {

    class PauseCommand {
    public:
	
	explicit
	PauseCommand(thr::ThreadRunner &th_runner)
	    : runner(&th_runner)
	{ }
	
	std::string name() const { return "pause"; }

	std::string pattern() const { return AppDefs::number_pattern(); }
	
	void issue(std::string::const_iterator it)
	{
	    int id = std::atoi(&*it) - 1;  // we map our application's ids [1 -> nb_threads]
                                           // into the thread runner's "natural" C++ ids

	    if (id < 0 || id > runner->size()) {
		std::cerr << ("invalid id " + std::to_string(id+1) + " passed to " + name() + " command!\n");
		return;
	    }
	    
	    if (runner->status(id) == thr::Status::RUNNING) {

		runner->pause(id);
		std::string msg = "paused thread " + std::to_string(id+1) + ".\n";
		std::cout << msg;

	    } else {
		std::cerr << "error: trying to pause thread that is not running!\n";
	    }

	}
	
    private:
	// this is a pointer (and not a reference) because we want
	// to be able to move and copy assign this object
	thr::ThreadRunner *runner;
    };
    
}

#endif

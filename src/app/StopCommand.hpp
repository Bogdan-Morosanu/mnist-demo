#ifndef DEMO_APP_STOP_COMMAND
#define DEMO_APP_STOP_COMMAND

#include <string>
#include <cstdlib>
#include <iostream>

#include "AppDefs.hpp"

#include "threads/ThreadRunner.hpp"

namespace app {

    class StopCommand {
    public:
	
	explicit
        StopCommand(thr::ThreadRunner &th_runner)
	    : runner(&th_runner)
	{ }
	
	std::string name() const { return "stop"; }

	std::string pattern() const { return AppDefs::number_pattern(); }
	
	void issue(std::string::const_iterator it)
	{
	    int id = std::atoi(&*it);
	    runner->stop(id - 1); // we map our application's ids [1 -> nb_threads]
	                          // into the thread runner's "natural" C++ ids

	    std::cout << "stopped thread " << id << ".\n";
	}
	
    private:
	// this is a pointer (and not a reference) because we want
	// to be able to move and copy assign this object
	thr::ThreadRunner *runner;
    };
    
}

#endif
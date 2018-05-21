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
	    int id = std::atoi(&*it) - 1;  // we map our application's ids [1 -> nb_threads]
                                           // into the thread runner's "natural" C++ ids

	    auto sts = runner->status(id);
	    if (sts == thr::Status::RUNNING || sts == thr::Status::PAUSED) {

		runner->stop(id);
		std::string msg = "stopped thread " + std::to_string(id+1) + ".\n";
		std::cout << msg;

	    } else {
		std::cerr << "error: trying to stop thread that is neither running nor paused!\n";
	    }
	}
	
    private:
	// this is a pointer (and not a reference) because we want
	// to be able to move and copy assign this object
	thr::ThreadRunner *runner;
    };
    
}

#endif

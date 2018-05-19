#ifndef DEMO_APP_RESTART_COMMAND
#define DEMO_APP_RESTART_COMMAND

#include <string>
#include <cstdlib>

#include "AppDefs.hpp"

#include "threads/ThreadRunner.hpp"

namespace app {

    class RestartCommand {
    public:
	
	explicit
	RestartCommand(thr::ThreadRunner &th_runner)
	    : runner(&th_runner)
	{ }
	
	std::string name() const { return "restart"; }

	std::string pattern() const { return AppDefs::number_pattern(); }
	
	void issue(std::string::const_iterator it)
	{
	    int id = std::atoi(&*it);
	    runner->restart(id - 1); // we map our application's ids [1 -> nb_threads]
	                             // into the thread runner's "natural" C++ ids

	    std::string msg = "restarted thread " + std::to_string(id) + ".\n";
	    std::cout << msg;
	}
	
    private:
	// this is a pointer (and not a reference) because we want
	// to be able to move and copy assign this object
	thr::ThreadRunner *runner;
    };
    
}

#endif

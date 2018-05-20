#ifndef DEMO_APP_STATUS_COMMAND
#define DEMO_APP_STATUS_COMMAND

#include <string>
#include <cassert>

#include "threads/ThreadRunner.hpp"

namespace app {

    class StatusCommand {
    public:

	explicit
	StatusCommand(thr::ThreadRunner &th_runner)
	    : runner(&th_runner)
	{ }
	
	std::string name() { return "status"; }

	void issue()
	{
	    for (std::size_t i = 0; i < runner->size(); ++i) {
		std::string msg = "thread " +
		                   std::to_string(i+1) +
		                   " - " +
		                   status_string(runner->status(i)) +
                                   " [ " + runner->info(i) + " ]\n";

		std::cout << msg;
	    }
	}

    private:

	static std::string status_string(thr::Status sts)
	{
	    switch(sts) {
	    case thr::Status::RUNNING:
		return "running";

	    case thr::Status::PAUSED:
		return "paused";

	    case thr::Status::STOPPED:
		return "stopped";

	    case thr::Status::FINISHED:
		return "finished";

	    default:
		assert(false && "enum has changed but member function processing it has not! please update");
		return "malformed"; // just to please compiler warning
	    }	
	}
	
	// this is a pointer (and not a reference) because we want
	// to be able to move and copy assign this object
	thr::ThreadRunner *runner;
    };
}

#endif

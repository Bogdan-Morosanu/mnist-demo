#ifndef DEMO_APP_APPLICATION
#define DEMO_APP_APPLICATION

#include "threads/ThreadRunner.hpp"

#include "parser/Parser.hpp"

#include "WarpWorkManager.hpp"

namespace app {

    /// @brief main class managing our application.
    class Application {
    public:
	
	Application();

	/// @brief run application on th_num threads.
	void run(int th_num);
	
    private:
	
	/// object managing threads
	thr::ThreadRunner th_runner;

	/// object parsing input from the user
	psr::CaseParser parser;

	/// object handling the warp work sequence creation
	app::WarpWorkManager warp_mgr;
    };
    
}

#endif

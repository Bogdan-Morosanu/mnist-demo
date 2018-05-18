#ifndef DEMO_APP_APPLICATION
#define DEMO_APP_APPLICATION

#include "threads/ThreadRunner.hpp"

#include "parser/Parser.hpp"

namespace app {

    
    class Application {
    public:
	
	Application();

	void run(int th_num);
	
    private:
	/// object managing threads
	thr::ThreadRunner th_runner;

	psr::CaseParser parser;
    };
    
}

#endif

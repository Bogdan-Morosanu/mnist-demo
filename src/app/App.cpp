#include "App.hpp"

#include "threads/ResThread.hpp"
#include "threads/CoutWorkSequence.hpp"

#include "parser/CommandParser.hpp"
#include "parser/EchoCommand.hpp"

#include "PauseCommand.hpp"
#include "ResumeCommand.hpp"

namespace app {

    Application::Application()
	: th_runner()
	, parser()
    { }

    void Application::run(int th_num)
    {
	for (int i = 0 ; i < th_num; i++) {
	    thr::ResThread<thr::CoutWorkSequence> res_thread({"thread " + std::to_string(i+1) + "\n", 10 });
	    th_runner.push_back(std::move(res_thread));
	}

	parser.push_back(psr::CommandParser<psr::EchoCommand>());
	parser.push_back(psr::CommandParser<app::PauseCommand>(app::PauseCommand(th_runner)));
	parser.push_back(psr::CommandParser<app::ResumeCommand>(app::ResumeCommand(th_runner)));
	
	parser.parse_stream(std::cin, std::cerr);

	// TODO insert code to cleanup, such that we can call .run() more than once
    }
}

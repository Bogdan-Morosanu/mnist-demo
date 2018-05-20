#include "App.hpp"

#include "threads/ResThread.hpp"
#include "threads/CoutWorkSequence.hpp"

#include "parser/CommandParser.hpp"
#include "parser/EchoCommand.hpp"

#include "PauseCommand.hpp"
#include "RestartCommand.hpp"
#include "StopCommand.hpp"
#include "StatusCommand.hpp"

namespace app {

    Application::Application()
	: th_runner()
	, parser()
    {
	parser.push_back(psr::UnaryCommandParser<psr::EchoCommand>());
	parser.push_back(psr::UnaryCommandParser<app::PauseCommand>(app::PauseCommand(th_runner)));
	parser.push_back(psr::UnaryCommandParser<app::RestartCommand>(app::RestartCommand(th_runner)));
	parser.push_back(psr::UnaryCommandParser<app::StopCommand>(app::StopCommand(th_runner)));
	parser.push_back(psr::CommandParser<app::StatusCommand>(app::StatusCommand(th_runner)));
    }

    void Application::run(int th_num)
    {
	for (int i = 0 ; i < th_num; i++) {
	    thr::ResThread<thr::CoutWorkSequence> res_thread({"thread " + std::to_string(i+1) + "\n", 10 });
	    th_runner.push_back(std::move(res_thread));
	}
	
	std::thread parse_thread([&]()
				 {
				   parser.parse_stream(std::cin, std::cerr);
				 });
	
	th_runner.join_all();

	std::cout << "All threads have finished their work.\nType 'exit' to finish parsing.\n" << std::endl;

	parse_thread.join();

	th_runner.finish();
    }
}

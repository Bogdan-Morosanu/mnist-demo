#include "App.hpp"

#include "threads/ResThread.hpp"
#include "threads/CoutWorkSequence.hpp"

#include "parser/CommandParser.hpp"
#include "parser/EchoCommand.hpp"

#include "PauseCommand.hpp"
#include "RestartCommand.hpp"
#include "StopCommand.hpp"
#include "StatusCommand.hpp"
#include "WarpWorkSequence.hpp"
#include "WarpWorkManager.hpp"

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
	auto to_warp = th_num / 2;
	auto to_int = th_num - to_warp;
	
	for (int i = 0 ; i < to_warp; i++) {
	    thr::ResThread<app::WarpWorkManager::WorkSequence> res_thread(warp_mgr.generate_work());
	    th_runner.push_back(std::move(res_thread));
	}

	for (int i = 0; i < to_int; i++) {
	    thr::ResThread<app::IntegrateWorkManager::WorkSequence> res_thread(int_mgr.generate_work());
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

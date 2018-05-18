#include <iostream>
#include <exception>
#include <mutex>

#include <boost/program_options.hpp>

#include "threads/ThreadRunner.hpp"
#include "threads/CoutWorkSequence.hpp"
#include "threads/ResThread.hpp"

namespace po = boost::program_options;

int main(int argc, char **argv)
{
    try {

	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help", "print help message")
	    ("thread-num", po::value<int>(), "number of threads to run");
    
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
	    std::cout << desc << std::endl;
	    std::exit(0);
	}

	if (vm.count("thread-num")) {
	    int th_num = vm["thread-num"].as<int>();
	    std::cout << "starting " << th_num << " threads...\n";

	    thr::ThreadRunner runner;

	    for (int i = 0; i < th_num; ++i) {
	    	thr::ResThread<thr::CoutWorkSequence> res_thread({"thread " + std::to_string(i) + "\n", 10 });
	    	runner.push_back(std::move(res_thread));
	    }

	    std::this_thread::sleep_for(std::chrono::seconds(5));

	    runner.pause(0);
	    runner.pause(1);

	    std::this_thread::sleep_for(std::chrono::seconds(10));

	    runner.resume(0);
	    runner.resume(1);
	    
	    runner.join_all();

	    std::cout << "done!" << std::endl;
	
	} else {
	    std::cout << "no thread num specified!" << std::endl;
	    std::exit(EXIT_FAILURE);
	}

    } catch(const std::exception &e) {
	std::cout << e.what() << std::endl;
	std::exit(EXIT_FAILURE);
    }

    return 0;
}

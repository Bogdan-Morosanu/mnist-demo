#include <iostream>
#include <exception>
#include <mutex>

#include <boost/program_options.hpp>

#include "threads/ThreadRunner.hpp"

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

	    std::mutex mtx;
	    
	    thr::ThreadRunner runner(th_num,
				     [&]()
				     {
					 std::lock_guard<std::mutex> lock(mtx);
					 std::cout << "I'm here!" << std::endl;
				     });

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

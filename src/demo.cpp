#include <iostream>
#include <exception>
#include <mutex>

#include <boost/program_options.hpp>

#include "app/App.hpp"

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

	    app::Application demo_app;
	    demo_app.run(th_num);
	    
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

#include <iostream>
#include <exception>
#include <mutex>

#include <boost/program_options.hpp>

#include "app/App.hpp"

#include "point-cloud/UniformPointStream.hpp"

namespace po = boost::program_options;

int main(int argc, char **argv)
{
    try {

	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help", "print help message")
	    ("threads", po::value<int>(), "number of threads to run");
    
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
	    std::cout << desc << std::endl;
	    std::exit(0);
	}
	
	if (vm.count("threads")) {
	    int th_num = vm["threads"].as<int>();

	    std::cout << "starting " << th_num << " threads...\n";

	    app::Application demo_app;
	    demo_app.run(th_num);
	    	
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

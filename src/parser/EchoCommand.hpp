#ifndef PSR_ECHO_COMMAND
#define PSR_ECHO_COMMAND

#include <string>
#include <iostream>

namespace psr {

    class EchoCommand {
    public:

	std::string name() { return "echo"; }

	void issue(int i)
	{
	    std::cout << "echo " << i << std::endl;
	}
	
    };
}

#endif

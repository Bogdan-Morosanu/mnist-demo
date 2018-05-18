#ifndef PSR_ECHO_COMMAND
#define PSR_ECHO_COMMAND

#include <string>
#include <iostream>

namespace psr {

    class EchoCommand {
    public:

	std::string name() const { return "echo"; }
	
	std::string pattern() const { return ".*"; }
	
	void issue(std::string::const_iterator it)
	{
	    std::cout << &*it << std::endl;
	}	
    };
}

#endif

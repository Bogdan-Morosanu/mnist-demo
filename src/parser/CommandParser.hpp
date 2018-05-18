#ifndef PSR_COMMAND_PARSER
#define PSR_COMMAND_PARSER

#include <string>
#include <regex>
#include <cstdlib>

namespace psr {

    /// @brief adapts a Command object to be included in a Case parser.
    ///        This parses command in the form of <name> <positive-integer-argument>.
    ///        You need to provide a move-constructible type with two member functions
    ///              1) std::string name(); - that gives the name of the command
    ///              2) void issue(int i);  - that will be called when the command is
    ///                                       detected by the parser in the input.
    template < typename Command >
    class CommandParser {
    public:

	explicit
	CommandParser(Command c = Command())
	    : rgx("^\\s*" + c.name() + "\\s+([1-9][0-9]*)$")
	    , command(std::move(c))
	{ }

	bool accept(const std::string &s)
	{
	    std::smatch matches;

	    if (std::regex_match(s, matches, rgx)) {
		command.issue(std::atoi(&*matches[1].first)); // get the char * of the integer (\n is ignored)
		return true;

	    } else {
		return false;
	    }
	}
	
    private:

	std::regex rgx;
	
	Command command;
    };
    
}

#endif

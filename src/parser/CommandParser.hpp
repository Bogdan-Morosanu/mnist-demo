#ifndef PSR_COMMAND_PARSER
#define PSR_COMMAND_PARSER

#include <string>
#include <regex>
#include <cstdlib>

namespace psr {

    /// @brief adapts a Unary Command object to be included in a Case parser.
    ///        This parses a command in the form of "<command-name> <mandatory-argument>".
    ///        You need to provide a move-constructible type with the member functions:
    ///              1) std::string name()       - gives the name of the command
    ///
    ///              2) std::string pattern()    - gives the regex pattern of strings you want to
    ///                                            accept as arguments.
    ///
    ///              3) void issue(iter it)      - that will be called when the command is
    ///                                            detected by the parser in the input.
    ///                                            iter is defined as:
    ///                                               using iter = std::string::const_iterator;
    ///                                            it points to the first character in the argument. 
    template < typename Command >
    class UnaryCommandParser {
    public:

	explicit
	UnaryCommandParser(Command c = Command())
	    : rgx("^\\s*" + c.name() + "\\s+(" + c.pattern() + ")$")
	    , command(std::move(c))
	{ }

	bool accept(const std::string &s)
	{
	    std::smatch matches;

	    if (std::regex_match(s, matches, rgx)) {
		command.issue(matches[1].first);
		return true;

	    } else {
		return false;
	    }
	}
	
    private:

	std::regex rgx;
	
	Command command;
    };

    /// @brief adapts a Nullary (with zero args) Command object to be included in a Case parser.
    ///        This parses a command in the form of "<command-name>".
    ///        You need to provide a move-constructible type with the member functions:
    ///              1) std::string name()       - gives the name of the command
    ///
    ///              2) void issue()             - that will be called when the command is
    ///                                            detected by the parser in the input.
    template < typename Command >
    class CommandParser {
    public:
	
	explicit
	CommandParser(Command c = Command())
	    : rgx("^\\s*" + c.name() + "\\s*$")
	    , command(std::move(c))
	{ }

	bool accept(const std::string &s)
	{
	    std::smatch matches;

	    if (std::regex_match(s, matches, rgx)) {
		command.issue();
		return true;

	    } else {
		return false;
	    }
	}
	
    private:

	std::regex rgx;
	
	Command command;
    };

    /// @brief special exit command parser, detects pattern that will end parsing.
    class ExitCommandParser {
    public:
	
	explicit
	ExitCommandParser(std::string name = "exit")
	    : rgx(std::move(name))
	{ }

	bool accept(const std::string &s) const
	{
	    std::smatch matches;

	    return std::regex_match(s, matches, rgx);
	}

    private:

	std::regex rgx;
    };
}

#endif


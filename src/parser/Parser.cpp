#include "Parser.hpp"


namespace prs {
    
    /// parse input stream in and log parsing errors to err
    void CaseParser::parse_stream(std::istream &in, std::ostream &err)
    {
	while (!in.eof()) {
	    std::string s;
	    std::getline(in, s, '\n');

	    bool success = false;
	    for (auto &c : cases) {
		if (c->accept(s)) {
		    success = true;
		    break; // found case accepting, got to next string
		}
	    }

	    if (exit_parser.accept(s)) {
		break; // end parsing
	    }
		
	    if (!success) { // command not recognised
		err << "command not recognised \"" << s << "\"" << std::endl;		    
	    }
	}
    }

}

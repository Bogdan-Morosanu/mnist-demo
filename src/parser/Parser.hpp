#ifndef PSR_CASE_PARSER
#define PSR_CASE_PARSER

#include "CommandParser.hpp"

namespace psr {

    class CaseParser {
    public:

	CaseParser()
	    : cases()
	    , exit_parser()
	{ }

	template < typename Case >
	void push_back(Case c)
	{
	    using ValueType = typename std::decay<Case>::type;
	    cases.push_back(std::make_unique<CaseModel<ValueType>>(std::move(c)));
	}

	void set_exit_parser(psr::ExitCommandParser &&ep)
	{
	    this->exit_parser = ep;
	}
        
	/// parse input stream in and log parsing errors to err
	void parse_stream(std::istream &in, std::ostream &err);
	
    private:

	// we use the runtime concept idiom to implement polymorphic behaviour in our parser.
	//
	// our users define their own types and we can add them to our composite without
	// requiring intrusive intervention like inheriting from some base class (which may be
	// undesirable).
	//
	// all our users need to do is provide *any* type with a given interface.
	//
	struct CaseConcept {

	    virtual bool accept(const std::string &str) = 0;

	    virtual ~CaseConcept() = default;
	};

	template < typename Case >
	struct CaseModel final : public CaseConcept {

	    CaseModel(Case &&cs)
		: c(cs)
	    { }
	    
	    bool accept(const std::string &str)
	    {
		return c.accept(str);
	    }

	    Case c;
	};

	std::vector<std::unique_ptr<CaseConcept>> cases;

	psr::ExitCommandParser exit_parser;
    };
}

#endif

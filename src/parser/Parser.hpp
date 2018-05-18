#ifndef PSR_CASE_PARSER
#define PSR_CASE_PARSER

namespace psr {


    class CaseParser {
    public:

	CaseParser()
	    : cases()
	{ }

	template < typename Case >
	void push_back(Case c)
	{
	    using ValueType = typename std::decay<Case>::type;
	    cases.push_back(std::make_unique<CaseModel<ValueType>>(std::move(c)));
	}

	/// parse input stream in and log parsing errors to err
	void parse_stream(std::istream &in, std::ostream &err)
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

		if (!success) {
		    err << "error in input \"" << s << "\"" << std::endl;		    
		}
	    }
	}
	
    private:

	struct CaseConcept {
	    virtual bool accept(const std::string &str) = 0;
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
    };
}

#endif

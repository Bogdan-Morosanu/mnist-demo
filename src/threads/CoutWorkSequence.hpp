#include <iostream>
#include <string>
#include <cassert>

namespace thr {

    class CoutWorkSequence {
    public:

	CoutWorkSequence(std::string &&str, int times)
	    : str(str)
	    , times(times)
	{
	    assert(times > 0);
	}

	CoutWorkSequence(const std::string &str, int times)
	    : str(str)
	    , times(times)
	{
	    assert(times > 0);
	}
	
	bool finised()
	{
	    return times == 0;
	}

	void step()
	{
	    assert(!finished());
	    
	    times--;
	    std::cout << str;
	}
	
    private:
	
	std::string str;

	int times;
    };
}

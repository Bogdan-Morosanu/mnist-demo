#ifndef THREADS_COUT_WORK_SEQUENCE
#define THREADS_COUT_WORK_SEQUENCE

#include <iostream>
#include <string>
#include <cassert>
#include <thread>
#include <chrono>
#include <mutex>

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
	
	bool finished()
	{
	    return times == 0;
	}

	void step()
	{
	    assert(!finished());

	    times--;
	    std::cout << (str + "\n"); // we do this so the .info() returns a nice string
	                               // given more time we would escape the \n or something similar
	    
	    std::this_thread::sleep_for(std::chrono::seconds(3));
	}


	std::string info() const
	{
	    return "printing \"" + str + "\" to cout...";
	}
	
    private:

	std::string str;

	int times;
    };
}



#endif

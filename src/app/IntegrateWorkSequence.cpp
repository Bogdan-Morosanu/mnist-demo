#include "IntegrateWorkSequence.hpp"

namespace app {
    
    void IntegrateWorkSequence::step()
    {
	double fr = fraction_done(); // no synch necesary here since only this thread is advancing evals_done
	double point = lower_bound + fr * (upper_bound - lower_bound);

	static const double rect_length = (upper_bound - lower_bound) / total_due;

	double rect_height = std::exp(point * point);

	{
	    std::lock_guard<std::mutex> lock(*mtx);
	    evals_done++;
	    sum += rect_height * rect_length;
	}
    }


    std::string IntegrateWorkSequence::info() const
    {
	// we must synch with other thread advancing evals_done
	std::lock_guard<std::mutex> lock(*mtx); 
	    
	return std::string("integrating e^x^2 over [" +
			   std::to_string(lower_bound) + ", " +
			   std::to_string(upper_bound) + "] current est. " +
			   std::to_string(sum) + " (" +
			   std::to_string(100.0 * fraction_done()) +
			   "% done). ");
    }
	
}

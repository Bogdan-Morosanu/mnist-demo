#ifndef DEMO_APP_INTEGRATE_WORK_SEQUENCE
#define DEMO_APP_INTEGRATE_WORK_SEQUENCE

#include <mutex>
#include <memory>
#include <iostream>
#include <string>
#include <cassert>
#include <cmath>

namespace app {

    /// @brief numerically integrate e^x^2 on a given interval.
    class IntegrateWorkSequence {
    public:

	explicit
	IntegrateWorkSequence(int evals = 1e7, double low = 0.5, double high = 0.5, int evals_per_step = 16)
	    : step_num(evals_per_step)
	    , total_due(evals)
	    , evals_done(0)
	    , lower_bound(low)
	    , upper_bound(high)
	    , sum(0.0)
	    , mtx(std::make_unique<std::mutex>())
	{
	    assert(high > low);
	}
	
	bool finished()	{ return evals_done == total_due; }

	void step();

	std::string info() const;
	
    private:

	double fraction_done() const
	{
	    return evals_done / double(total_due);
	}
	
	/// number of evaluations of the integrand per work step.
	int step_num;

	/// number of evaluations of the integrand to do.
	int total_due;

	/// evaluations done so far.
	int evals_done;
	
	/// lower bound of the integral.
	double lower_bound;

	/// upper bound of the integral.
	double upper_bound;

	/// estimate of the integral so far.
	double sum;
	
	/// mutex to synchronise access between threads.
	std::unique_ptr<std::mutex> mtx;
    };
}

#endif

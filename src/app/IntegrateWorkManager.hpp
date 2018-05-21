#ifndef DEMO_APP_INTEGRATE_WORK_MANAGER
#define DEMO_APP_INTEGRATE_WORK_MANAGER

#include "point-cloud/UniformValueSource.hpp"
#include "IntegrateWorkSequence.hpp"

namespace app {

    class IntegrateWorkManager {
    public:

	static const double MINIMUM_INTERVAL_LENGTH;

	static const double STEP_RESOLUTION;
	
	IntegrateWorkManager()
	    : random_gen()
	{ }

	using WorkSequence = app::IntegrateWorkSequence;

	WorkSequence generate_work();
	
    private:
	ptc::UniformValueSource random_gen;
    };
    
}

#endif

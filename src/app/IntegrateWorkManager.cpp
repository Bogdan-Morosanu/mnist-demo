#include "IntegrateWorkManager.hpp"

#include <cstdlib>

namespace app {

    const double IntegrateWorkManager::MINIMUM_INTERVAL_LENGTH = 0.1;
    
    const double IntegrateWorkManager::STEP_RESOLUTION = 1e-8;
	
    IntegrateWorkManager::WorkSequence IntegrateWorkManager::generate_work()
    {
	auto a = random_gen.generate();
	auto b = random_gen.generate();

	auto low = std::min(a, b);
	auto high = std::max(a, b) + MINIMUM_INTERVAL_LENGTH;
	auto step_num = (high - low) / STEP_RESOLUTION;
		
	return WorkSequence(step_num, low, high);
    }    
}

#ifndef DEMO_APP_WORK_MANAGER
#define DEMO_APP_WORK_MANAGER

#include <list>

#include "WarpWorkSequence.hpp"
#include "ValueSink.hpp"

#include "point-cloud/UniformPointSources.hpp"
#include "point-cloud/UniformValueSource.hpp"

namespace app {

    /// @brief class manages warp work sequences.
    class WarpWorkManager {
    public:

	static const int POINT_CLOUD_SIZE;
	
	using WorkSequence = app::WarpWorkSequence<ptc::UniformPointSequence, app::ValueSink>;
	
	WorkSequence generate_work()
	{
	    seqs.emplace_back(POINT_CLOUD_SIZE);
	    
	    WorkSequence::AffineWarpMatrix mat;
	    rand_generator.write_to(mat);
	    
	    return WorkSequence(seqs.back(), point_sink, mat);
	}
	
    private:

	/// random generator used to generate the affine matrix for each transform
	ptc::UniformValueSource rand_generator;

	/// random 3d point sequences for each worker thread
	// this is a list and not a std::vector since we need object adresses to persist
	// when we .push_back and we do not iterate over them, we only need to keep
	// them alive.
	std::list<ptc::UniformPointSequence> seqs;

	/// simulated data sink consuming output of worker threads
	app::ValueSink point_sink;
    };
}

#endif

#ifndef DEMO_APP_WARP_WORK_SEQUENCE
#define DEMO_APP_WARP_WORK_SEQUENCE

#include <iostream>
#include <string>
#include <cassert>

#include <Eigen/Core>

namespace app {

    template < typename InputSequence, typename OutputSink >
    class WarpWorkSequence {
    public:

	using AffineWarpMatrix = Eigen::Matrix<double, 4, 4>;

	WarpWorkSequence(InputSequence &in_seq,
			 OutputSink &out_sink,
			 const AffineWarpMatrix &m,
			 int warps_per_step = 16)
	    : in_src(&in_seq)
	    , out_dst(&out_sink)
	    , mat(m)
	    , step_num(warps_per_step)
	{ }
	
	bool finished()
	{
	    return in_src.empty();
	}

	void step()
	{
	    assert(!finished());

	    for (int i = 0; i < step_num; ++i, in_src.advance) {
		out_dst->accept(mat * in_src->value());		
	    }
	}


	std::string info() const
	{
	    return "warping point clouds."
	}
	
    private:

	/// @brief sequence containing input data
	// this is a pointer and not reference because we need move semantics.
	InputSequence *in_src;

	/// @brief sink accepting output data
	// this is a pointer and not reference because we need move semantics.
	OutputSink *out_dst;

	/// affine transform to apply
	AffineWarpMatrix mat;

	/// number of transformations per work step
	int step_num;
    };
}

#endif

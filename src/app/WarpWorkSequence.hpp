#ifndef DEMO_APP_WARP_WORK_SEQUENCE
#define DEMO_APP_WARP_WORK_SEQUENCE

#include <mutex>
#include <memory>
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
	    , total_due(in_seq.remaining())
	    , mtx(std::make_unique<std::mutex>())
	{ }
	
	bool finished()
	{
	    return in_src->empty();
	}

	void step()
	{
	    assert(!finished());

	    for (int i = 0; i < step_num; ++i) {

		Eigen::Matrix<double, 4, 1> pt_hom; // point in homogeneous coords

		auto &pt = in_src->value();

		pt_hom(0, 0) = pt(0, 0);
		pt_hom(1, 0) = pt(1, 0);
		pt_hom(2, 0) = pt(2, 0);
		pt_hom(3, 0) = 1.0;
		
		auto hom_result = mat * pt_hom;

		Eigen::Matrix<double, 3, 1> ret;

		ret(0, 0) = hom_result(0, 0);		
		ret(1, 0) = hom_result(1, 0);
		ret(2, 0) = hom_result(2, 0);

		{
		    // note that we need sinchronise when we mutate shared object.
		    // we can read in parallel with no synch, as long as others do not
		    // modify them. here we assume the info() member function is
		    // called from another thread, so we need to synchronise the
		    // .remaining() and .advance() member function calls for our
		    // in_src object
		    std::lock_guard<std::mutex> lock(*mtx);
		    in_src->advance();		    
		}

		out_dst->accept(ret); // send our output value to the data sink
	    }
	}


	std::string info() const
	{
	    std::lock_guard<std::mutex> lock(*mtx);
	    
	    return std::string("warping point clouds (" +
			       std::to_string(100.0 * (1.0 - in_src->remaining() / double(total_due))) +
			       "% done).");
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

	/// numer of transformations to do
	int total_due;

	/// mutex to synchronise accesing in_src
	std::unique_ptr<std::mutex> mtx;
    };
}

#endif

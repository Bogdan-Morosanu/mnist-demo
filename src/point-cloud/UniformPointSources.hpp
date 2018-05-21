#ifndef PTC_UNIFORM_POINT_STREAM
#define PTC_UNIFORM_POINT_STREAM

#include "UniformValueSource.hpp"

namespace ptc {

    /// @brief class used to generate an endless ammount of
    ///        uniform-distributed 3D points.
    class UniformPointStream {
    public:

	UniformPointStream()
	    : val()
	    , val_src()
	{
	    advance();
	}

	/// @brief get current element
	const Eigen::Matrix<double, 3, 1> &value() const
	{
	    return val;
	}

	/// @brief advance stream
	void advance()
	{
	    val_src.write_to(val);
	}

	/// @brief iostream-like interface
	UniformPointStream& operator >> (Eigen::Matrix<double, 3, 1> &out)
	{
	    out = value();
	    advance();
	    return *this;
	}
	
    private:

	Eigen::Matrix<double, 3, 1> val;
	
	ptc::UniformValueSource val_src;
    };

    /// @brief class adapts a uniform point stream to yield a finite length sequence
    //
    // Remarks: I thought a lot about how to do this with iterators so our
    //          work sequences could also work with standard containers out of the box.
    //          However, writing an end iterator for these uniform point sequences
    //          was a bit tricky to do when we would allow users to copy iterators,
    //          since advancing one iterator would have the effect of advancing the
    //          whole sequence. Maybe I should have made move-only iterators, but
    //          I gave up on the whole ideea as it seemed to complex to implement
    //          correctly in the time I had remaining.
    //
    class UniformPointSequence {
    public:
	explicit
	UniformPointSequence(int point_num)
	    : pt_num(point_num)
	    , stream()
	{
	    assert(point_num >= 0); // maybe there is some use in creating an empty sequence
	}

	const Eigen::Matrix<double, 3, 1> &value() const
	{
	    return stream.value();
	}

	void advance()
	{
	    assert(!empty());
	    
	    stream.advance();
	    pt_num--;
	}

	bool empty() const
	{
	    return pt_num == 0;
	}

	int remaining() const
	{
	    return pt_num;
	}

    private:

	/// points left in the sequence
	int pt_num;

	/// underlying stream of points
	UniformPointStream stream;
    };
}

#endif

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

}

#endif

#ifndef PTC_RANDOM_POINT_SOURCE
#define PTC_RANDOM_POINT_SOURCE

#include <random>

#include <Eigen/Core>

namespace ptc {

    /// @brief class is used to generate uniform random variables.
    class UniformValueSource {
    public:

	explicit
        UniformValueSource(double low = -1.0, double high = 1.0)
	    : rd()
	    , gen()
	    , dist(low, high)
	{
	    gen = std::mt19937(rd()); // do it like this so re-ordering members does not create
	                            // undefined behaviour
	}

	template < int Rows, int Cols >
	Eigen::Matrix<double, Rows, Cols> generate()
	{
	    Eigen::Matrix<double, Rows, Cols> ret;

	    write_to(ret);
	    
	    return ret;
	}

	template < int Rows, int Cols >
	void write_to(Eigen::Matrix<double, Rows, Cols> &out)
	{
	    for (int r = 0; r < Rows; ++r) {
		for (int c = 0; c < Cols; ++c) {
		    out(r, c) = dist(gen);
		}
	    }
	}
	
    private:
	
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<double> dist;
    };
    
}


#endif

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
	    : gen(std::random_device()())
	    , dist(low, high)
	{
	}

	double generate()
	{
	    return dist(gen);
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

	void write_to(double &d)
	{
	    d = dist(gen);
	}
	
    private:
	
	std::mt19937 gen;

	std::uniform_real_distribution<double> dist;
    };
    
}


#endif

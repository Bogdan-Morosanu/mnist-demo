
#include <cmath>

namespace mnet {

    namespace act {

	struct ReLu {

	    double operator() (double d)
	    {
		return std::max(0, d);
	    }

	    double deriv(double d)
	    {
		return d > 0.0 ? 1.0 : 0.0;
	    }
	};

    }
}

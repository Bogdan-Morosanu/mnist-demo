#include <memory>

#include <Eigen/Core>

namespace mnet {

    template < typename ActFunction, int InDim, int OutDim >
    class FcLayer {
    public:

	using WeightType = Eigen::Matrix<double, OutDim, InDim>;

	using BiasType = Eigen::Matrix<double, OutDim, 1>;
	
	using InType = Eigen::Matrix<double, InDim, 1>;

	using OutType = Eigen::Matrix<double, OutDim, 1>;
	
	struct Gradients {

	    std::unique_ptr<WeightType> weight_grad;

	    std::unique_ptr<BiasType> bias_grad;
	};

	
	explicit
	FcLayer(ActFunction fn = ActFunction())
	    : fn(fn)
	    , weights(std::make_unique<decltype(*weights)>())
	    , bias(std::make_unique<decltype(*bias)>()
	{
	    
	}

	std::unique_ptr<OutType> forward(const InType &in) const
	{
	    auto ret = std::make_unique<OutType>();

	    *ret = ((*this->weights) * in).transpose();

	    *ret += *bias;

	    for (auto &x : *ret) {
		x = fn(x);
	    }
	    
	    return ret;
	}

	Gradients backward(const InType &in) const
	{
	    Gradients ret = { std::make_unique<WeightType>(),
			      std::make_unique<BiasType>() };

	    // d / dw f (w*x + b)
	    // == df / du * du / dw with u = w*x + b
	    // df / du * x
	    auto u = std::make_unique<OutType>();

	    *u = ((*this->weights) * in).transpose();

	    *u += *bias;

	    for (int r = 0; r < weights->rows(); ++r) {
		for (int c = 0; c < weights->cols(); ++c) {
		    (*ret.weight_grad)(r, c) = in(c) * fn.deriv(u(r));
		}
	    }

	    for (int i = 0; i < ret.bias_grad->rows(); ++i) {
		(*ret.bias_grad)(i) = fn.deriv(u(i));
	    }
	    
	    return ret;
	}

	void update_params(const WeightType &delta_w, const BiasType &delta_b)
	{
	    *weights += delta_w;
	    *bias += delta_b;
	}
	
    private:

	/// activation function for this layer
	ActFunction fn;

	/// weights for the linear transformation of the input
	std::unique_ptr<WeightType> weights;

	/// bias term for offsetting the output
	std::unique_ptr<BiasType> bias;
    };

}

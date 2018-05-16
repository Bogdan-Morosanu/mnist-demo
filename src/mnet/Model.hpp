
#include <tuple>

namespace mnet {

    template < typename... Layers >
    class Model {
    public:

	using LayerList = std::tuple<Layers...>;

	using GradList = std::tuple<Layers::Gradients...>;

	using InType = typename std::tuple_element<0, LayerList>::type::InType;

	using OutType = typename std::tuple_element<std::tuple_size<LayerList>::value - 1, LayerList>::type::OutType;

	std::unique_ptr<OutType> forward(const InType &in) const;

	GradList backward(const InType &in) const;

	void update_params(const GradList &deltas);
	
    private:
	
	LayerList layers;
    };
}

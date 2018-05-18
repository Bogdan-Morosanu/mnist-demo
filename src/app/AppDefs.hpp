#ifndef DEMO_APP_DEFS
#define DEMO_APP_DEFS

namespace app {

    /// @brief global application definitions
    struct AppDefs {

	/// @brief regex pattern to accept positive integers.
	static std::string number_pattern() { return "[1-9][0-9]*"; }
    };
}
#endif

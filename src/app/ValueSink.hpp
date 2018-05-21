#ifndef DEMO_APP_POINT_SINK
#define DEMO_APP_POINT_SINK

namespace app {

    /// @brief dummy class accepting objects from our worker threads.
    class ValueSink {
    public:

	template < typename T >
	void accept(T &&t)
	{
	    // in a "real" program we would move this value to its useful destination
	    // in this program we just need to prevent the compiler from optimising this function call away
	    asm volatile ("" :: "g" (t));
	}
    };
}

#endif

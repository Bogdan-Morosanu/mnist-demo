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
	    // (and thus deciding that all code in the warp work sequence is dead code, except the increment,
	    //  which is read when we print info about the thread). If the compiler decides this
	    // on higher optimisation settings, it will throw out all code dead code and leave us with just
	    // a thread incrementing a single integer.
	    //
	    // we have this ugly ifdef here because this solution is a gcc and clang only solution. I've
	    // looked up some MSCV options but they seem to be based on reinterpret_cast to char* and over-writing
	    // the t argument with itself, which seems a bit like undefined behavior to me. The option bellow
	    // is a no-op.
#ifndef _MSC_VER
	    asm volatile ("" :: "g" (t));
#endif
	}
    };
}

#endif

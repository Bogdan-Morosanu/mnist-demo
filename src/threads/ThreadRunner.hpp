#include <thread>
#include <vector>

namespace thr {

    class ThreadRunner {
    public:

	template < typename ThreadFunction >
	ThreadRunner(int num_th, ThreadFunction &&th)
	    : threads()
	{
	    for ( ; num_th > 0; --num_th) {
		threads.emplace_back(std::forward<ThreadFunction>(th));
	    }
	}

	void join_all()
	{
	    for (auto &th : threads) {
		if (th.joinable()) {
		    th.join();
		}
	    }
	}
	
    private:
	std::vector<std::thread> threads;
    };
}

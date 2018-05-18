#ifndef THREADS_THREAD_RUNNER
#define THREADS_THREAD_RUNNER

#include <thread>
#include <vector>
#include "ResThread.hpp"

namespace thr {

    /// @brief class manages a number of different resumable threads.
    ///        A resumable thread is a function that executes some work in a
    ///        different thread, must be move constructible and have the
    ///        following public member functions:
    ///	          1) void pause()         - pause execution of the thread (with
    ///                                     some implementation defined delay).
    ///
    ///           2) bool is_paused()     - inform the caller if the thread
    ///                                     is currently paused.
    ///
    ///           3) void resume()        - resume thread execution.
    ///
    ///           4) bool joinable()      - inform the caller if the thread is
    ///                                     joinable (ie it still has work to do).
    ///
    ///           5) void join()          - join the thread: returns when the thread
    ///                                     has stopped execution.
    class ThreadRunner {
    public:

	ThreadRunner()
	    : threads()
	{ }

	void join_all()
	{
	    for (auto &th : threads) {
		if (th->joinable()) {
		    th->join();
		}
	    }
	}

	template < typename ResThread >
	void push_back(ResThread th)
	{
	    using ValType = typename std::decay<ResThread>::type;
	    
	    threads.push_back(std::make_unique<ResThreadModel<ValType>>(std::move(th)));
	}

	~ThreadRunner() { join_all(); }

	void pause(int id)
	{
	    threads[id]->pause();
	}

	void resume(int id)
	{
	    threads[id]->resume();
	}

	/// @brief joins all threads and then destroys them once they are done executing
	void finish()
	{
	    join_all();
	    threads.clear();
	}
	
    private:

	struct ResThreadConcept {

	    virtual void pause() = 0;

	    virtual bool is_paused() = 0;

	    virtual void resume() = 0;

	    virtual bool joinable() = 0;

	    virtual void join() = 0;
	};

	template < typename ResThread >
	struct ResThreadModel final : public ResThreadConcept {

	    ResThreadModel(ResThread t)
		: th(std::move(t)) { }

	    void pause() override
	    {
		th.pause();
	    }

	    bool is_paused() override
	    {
		return th.is_paused();
	    }

	    void resume() override
	    {
		th.resume();
	    }

	    bool joinable() override
	    {
		return th.joinable();
	    }

	    void join() override
	    {
		th.join();
	    }

	    ResThread th;
	};

	std::vector<std::unique_ptr<ResThreadConcept>> threads;
    };
}

#endif

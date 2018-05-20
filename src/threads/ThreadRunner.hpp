#ifndef THREADS_THREAD_RUNNER
#define THREADS_THREAD_RUNNER

#include <thread>
#include <vector>
#include "ResThread.hpp"
#include "ThreadDefs.hpp"

namespace thr {

    /// @brief class manages a number of different resumable threads.
    ///        A resumable thread is a function that executes some work in a
    ///        different thread, must be move constructible and have the
    ///        following public member functions:
    ///	          1) void pause()               - pause execution of the thread (with
    ///                                           some implementation defined delay).
    ///
    ///           2) thr::Status status() const - inform the caller of the thread's current
    ///                                           status (running, paused, stopped, finished).
    ///           3) std::string info() const   - info string to print to the console
    ///                                           when describing the thread
    ///
    ///           4) void restart()             - restart thread execution.
    ///
    ///           5) void stop()                - stop thread execution.
    ///
    ///           6) bool joinable() const      - inform the caller if the thread is
    ///                                           joinable (ie it still has work to do).
    ///
    ///           7) void join()                - join the thread: returns when the thread
    ///                                           has stopped execution.
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

	void restart(int id)
	{
	    threads[id]->restart();
	}

	void stop(int id)
	{
	    threads[id]->stop();
	}

	Status status(int id) const
	{
	    return threads[id]->status();
	}

	std::string info(int id) const
	{
	    return threads[id]->info();
	}
		
	
	/// @brief joins all threads and then destroys them once they are done executing
	void finish()
	{
	    join_all();
	    threads.clear();
	}

	std::size_t size() const
	{
	    return threads.size();
	}
	
    private:

	struct ResThreadConcept {

	    virtual void pause() = 0;

	    virtual Status status() const = 0;

	    virtual std::string info() const = 0;

	    virtual void restart() = 0;

	    virtual void stop() = 0;
	    
	    virtual bool joinable() const = 0;

	    virtual void join() = 0;

	    virtual ~ResThreadConcept() = default;
	};

	template < typename ResThread >
	struct ResThreadModel final : public ResThreadConcept {

	    ResThreadModel(ResThread t)
		: th(std::move(t)) { }

	    void pause() override
	    {
		th.pause();
	    }

	    Status status() const override
	    {
		return th.status();
	    }

	    std::string info() const override
	    {
		return th.info();
	    }
	    
	    void restart() override
	    {
		th.restart();
	    }

	    void stop() override
	    {
		th.stop();
	    }
	    
	    bool joinable() const override
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

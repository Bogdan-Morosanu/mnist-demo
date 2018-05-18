#ifndef THREADS_RES_THREAD
#define THREADS_RES_THREAD

#include <thread>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <iostream>

namespace thr {

    /// @brief Resumable thread, similar to std::thread, but with pause and resume functionality.
    ///        Takes as input a Work Sequences, i.e. a class that can be move constructed and has
    ///        the following member functions:
    ///           1) void step()     - execute an atomic piece of work that cannot be paused
    ///           2) bool finished() - inform the caller if all the work steps have been completed
    ///
    ///        This class takes ownership of that work sequence and runs it in a separate thread
    ///        of execution.
    template < typename WorkSeq >
    class ResThread {	
    public:

	explicit
	ResThread(WorkSeq work_seq)
	    : sync(std::make_unique<Sync>(std::move(work_seq)))
	    , thread() // we don't start the thread right away since that could be undefined
	               // behavior if someone re-ordered members in this class
	{
	    thread = std::thread(ThreadFunctionProxy(*sync)); // start once all members have been constructed
	}

	// expose some std::thread functionality
	
	bool joinable() { return thread.joinable(); }

	void join() { thread.join(); }

	/// pause execution until resume() is called
	void pause()
	{
	    std::lock_guard<std::mutex> lock(sync->pause_mtx);

	    sync->paused = true;
	}

	/// resume thread if paused before-hand
	void resume()
	{
	    {
		std::lock_guard<std::mutex> lock(sync->pause_mtx);

		sync->paused = false;
	    }
	    
	    sync->wakeup_cv.notify_one();
	}
	
	bool is_paused()
	{
	    std::lock_guard<std::mutex> lock(sync->pause_mtx);
	    
	    return sync->paused;
	}
	
    private:

	/// get the underlying value type of our work sequence
	using WorkSeqVal = typename std::decay<WorkSeq>::type;
	
	/// @brief Synchronisation data to be allocated on the heap.
	/// @detail Gives our thread function proxy a permanent location to
	///         read synchronisation data from, even if the resthread object
	///         handle has been moved arroud the code.
	struct Sync {

	    Sync(WorkSeqVal &&work)
		: work(work)
		, paused(false)
		, pause_mtx()
		, wakeup_cv()
	    { }
	    
	    WorkSeqVal work;

	    bool paused;

	    std::mutex pause_mtx;

	    std::condition_variable wakeup_cv;
	};
	
	// we don't pass *this to std::thread, because that would copy
	// the current object into a new one, which is not what we want.
	// we just need a lightweight proxy.
	class ThreadFunctionProxy {
	public:    
	    explicit
	    ThreadFunctionProxy(Sync &res_thread_sync)
		: sync(res_thread_sync)
	    { }

	    void operator()()
	    {
		while (!sync.work.finished()) {
		    try {
			sync.work.step();

		    } catch(std::exception &e) {
			std::cerr << "exception thrown in work sequence: " << e.what() << std::endl;
			return;
		    }

		    wait_if_needed();
		}
	    }
	    
	private:

	    /// pause thread if pause() has been called, until resume() is called
	    void wait_if_needed()
	    {
		std::unique_lock<std::mutex> lock(sync.pause_mtx);

		if (sync.paused) {
		    sync.wakeup_cv.wait(lock, [&]() { return !sync.paused; });
		}
	    }

	    Sync &sync;
	};
	
	// the resthread object is the sole owner of both the thread and the data it
	// operates on. Since we cannot legally destroy the std::thread object while it is running,
	// and when we move this object, we move both the std::unique_ptr and the std::thread
	// together, we conclude that, at the moment of destruction, the std::thread has exited,
	// and it is thus safe to destroy the synchronisation data sync.
	
	std::unique_ptr<Sync> sync;
	
	std::thread thread;
    };
}

#endif

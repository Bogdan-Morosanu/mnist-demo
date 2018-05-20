#ifndef THREADS_RES_THREAD
#define THREADS_RES_THREAD

#include <thread>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <iostream>

#include "ThreadDefs.hpp"

namespace thr {
    
    /// @brief Resumable thread, similar to std::thread, but with pause, restart and stop functionality.
    ///        Takes as input a Work Sequences, i.e. a class that can be move constructed and has
    ///        the following member functions:
    ///           1) void step()              - execute an atomic piece of work that cannot be paused
    ///
    ///           2) bool finished() const    - inform the caller if all the work steps have been completed
    ///
    ///           3) std::string info() const - info string to print to the console when describing the thread
    ///
    ///        This class takes ownership of that work sequence and runs it in a separate thread
    ///        of execution.
    template < typename WorkSeq >
    class ResThread {	

       	static_assert(std::is_same<WorkSeq, typename std::decay<WorkSeq>::type>::value,
		      "Please create threads with value work sequence types only!");
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
	bool joinable() const { return thread.joinable(); }

	void join() { thread.join(); }

	/// pause execution until restart() is called
	void pause()
	{
	    std::lock_guard<std::mutex> lock(sync->mtx);

	    sync->paused = true;
	}
		
	/// restart thread if paused before-hand
	void restart()
	{
	    {
		std::lock_guard<std::mutex> lock(sync->mtx);

		sync->paused = false;
	    }
	    
	    sync->wakeup_cv.notify_one();
	}

	/// stop thread execution
	void stop()
	{
	    std::lock_guard<std::mutex> lock(sync->mtx);

	    sync->stopped = true;
	}

	Status status() const
	{
	    std::lock_guard<std::mutex> lock(sync->mtx);

	    if (sync->stopped) {
		return Status::STOPPED;

	    } else if (sync->paused) {
		return Status::PAUSED;

	    } else if (!sync->finished) {
		return Status::RUNNING;

	    } else {
		return Status::FINISHED;
	    }
	}
	
	std::string info() const
	{
	    return sync->work.info();
	}
	
    private:

	/// get the underlying value type of our work sequence
	using WorkSeqVal = typename std::decay<WorkSeq>::type;
	
	/// @brief Synchronisation data to be allocated on the heap.
	/// @detail Gives our thread function proxy a permanent location to
	///         read synchronisation data from, even if the resthread object
	///         handle has been moved arround the code.
	struct Sync {

	    Sync(WorkSeqVal &&work)
		: work(work)
		, paused(false)
		, stopped(false)
		, finished(false)
		, mtx()
		, wakeup_cv()
	    { }
	    
	    WorkSeqVal work;

	    bool paused;

	    bool stopped;

	    bool finished;
	    
	    std::mutex mtx;

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
		while (!sync.work.finished() && !is_stopped()) {

		    try {
			sync.work.step();

		    } catch(std::exception &e) {
			std::cerr << "exception thrown in work sequence: " << e.what() << std::endl;
			return;
		    }

		    // yes, we acquire the lock twice, but I expect that would
		    // not be a performance problem, since we almost always are
		    // the only thread waiting on it.
		    wait_if_needed();
		}

		std::lock_guard<std::mutex> lock(sync.mtx);
		sync.finished = true;
	    }
	    
	private:

	    /// pause thread if pause() has been called, until restart() is called
	    void wait_if_needed()
	    {
		std::unique_lock<std::mutex> lock(sync.mtx);

		if (sync.paused) {
		    sync.wakeup_cv.wait(lock, [&]() { return !sync.paused; });
		} 
	    }

	    bool is_stopped()
	    {
		bool ret;
		
		{
		    std::lock_guard<std::mutex> lock(sync.mtx);
		    ret = sync.stopped;
		}
		
		return ret;		
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

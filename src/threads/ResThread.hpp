#include <thread>
#include <mutex>
#include <condition_variable>

namespace thr {

    /// resumable thread
    template < typename WorkSeq >
    class ResThread {	
    public:
	ResThread(WorkSeq work_seq)
	    : work(std::move(work_seq))
	    , is_paused(false)
	    , pause_mtx()
	    , wakeup_cv()
	    , thread() // we don't start the thread right away since that could be undefined
	               // behavior if someone re-ordered members in this class
	{
	    thread = std::thread(ThreadFunctionProxy(*this)); // start once all members have been constructed
	}

	// expose some std::thread functionality
	
	bool joinable() { return thread.joinable(); }

	void join() { thread.join(); }

	void detach() { thread.detach(); }

	/// pause execution until resume() is called
	void pause()
	{
	    std::lock_guard<std::mutex> lock(pause_mtx);

	    is_paused = true;
	}

	/// resume thread if paused before-hand
	void resume()
	{
	    std::lock_guard<std::mutex> lock(pause_mtx);

	    is_paused = false;

	    wakeup_cv.notify_one();
	}

	bool paused() { return is_paused; }
	
    private:

	// we don't pass *this to std::thread, because that would copy
	// the current object into a new one, which is not what we want.
	// we just need a lightweight proxy.
	class ThreadFunctionProxy {
	public:    
	    explicit
	    ThreadFunctionProxy(ResThread &res_th)
		: res_thread(res_th)
	    { }

	    void operator()()
	    {
		// dispatch to resumable thread implementation
		res_thread();
	    }
	    
	private:
	    ResThread &res_thread;
	};
	
	/// "main" function to be run inside the std::thread
	void operator()()
	{
	    while (!work.finished()) {
		work.step();
		wait_if_needed();
	    }
	}

	/// pause thread if pause() has been called, until resume is called
	void wait_if_needed()
	{
	    std::unique_lock<std::mutex> lock(pause_mtx);

	    if (is_paused) {
		wakeup_cv.wait(lock, [&]() { return !is_paused; });
	    }
	}

	/// get the underlying value type of our work sequence
	using WorkSeqVal = typename std::decay<WorkSeq>::type;
	
	WorkSeqVal work;

	bool is_paused;
	
	std::mutex pause_mtx;

	std::condition_variable wakeup_cv;

	std::thread thread;
    };
}

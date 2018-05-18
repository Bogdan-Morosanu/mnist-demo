#ifndef THREADS_THREAD_RUNNER
#define THREADS_THREAD_RUNNER

#include <thread>
#include <vector>
#include "ResThread.hpp"

namespace thr {

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
	
    private:

	struct ResThreadConcept {

	    virtual void pause() = 0;

	    virtual bool is_paused() = 0;

	    virtual void resume() = 0;

	    virtual bool joinable() = 0;

	    virtual void join() = 0;

	    virtual void detach() = 0;
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

	    void detach() override
	    {
		th.detach();
	    }
	    
	    ResThread th;
	};

	std::vector<std::unique_ptr<ResThreadConcept>> threads;
    };
}

#endif

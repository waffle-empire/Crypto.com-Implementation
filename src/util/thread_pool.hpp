#pragma once
#include "common.hpp"

namespace crypto_com_impl
{
	class ThreadPool
	{
        // atomic variable == thread safe
		std::atomic<bool> m_accept_jobs;
        // 
		std::condition_variable m_data_condition;

        // stack (list) with functions with return type void
		std::stack<std::function<void()>> m_job_stack;
        // mutex => thread lock prevent access violation errors
		std::mutex m_lock;

        int m_thread_count;
        // vector (fancy list) with all the threads
		std::vector<std::thread> m_thread_pool;

        // main thread to launch thread pool
		std::thread m_managing_thread;
	public:
        // constructor of class
		ThreadPool(int thread_count = std::thread::hardware_concurrency());
        // destructor
		~ThreadPool();

        // destroy thread pool
		void destroy();
		bool has_jobs();
        // push function / lambda on stack
		void push(std::function<void()> func);
	private:
        // create thread pool
		void create();
        // tell the thread pool we're done using it and kill of all threads
		void done();
        // infinite running function waiting for new jobs to be pushed on stack
		void run();
	};

    // inline == global
    // thread_pool == instance
    // * == pointer
    // g_thread_pool == variable name
    // {} == initiate class object
	inline ThreadPool* g_thread_pool{};
}
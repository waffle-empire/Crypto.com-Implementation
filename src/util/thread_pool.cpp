#include "util/logger.hpp"
#include "thread_pool.hpp"

namespace crypto_com_impl
{
	ThreadPool::ThreadPool(int thread_count)
        : m_accept_jobs(true), m_data_condition(), m_job_stack(), m_lock(), m_thread_count(thread_count)
	{
		this->m_managing_thread = std::thread(&ThreadPool::create, this);

		g_thread_pool = this;
	}

	ThreadPool::~ThreadPool()
	{
		g_thread_pool = nullptr;
	}

	void ThreadPool::create()
	{
		g_log->info("THREAD_POOL", "Allocated %d threads in pool.", m_thread_count);
		this->m_thread_pool.reserve(m_thread_count);

		for (int i = 0; i < m_thread_count; i++)
			this->m_thread_pool.push_back(std::thread(&ThreadPool::run, this));
	}

	void ThreadPool::destroy()
	{
		this->m_managing_thread.join();

		this->done();

		for (int i = 0; i < this->m_thread_pool.size(); i++)
			this->m_thread_pool.at(i).join();
	}

	void ThreadPool::done()
	{
		std::unique_lock<std::mutex> lock(this->m_lock);
		this->m_accept_jobs = false;

		lock.unlock();
		this->m_data_condition.notify_all();
	}

	bool ThreadPool::has_jobs()
	{
		std::unique_lock<std::mutex> lock(this->m_lock);

		return !this->m_job_stack.empty();
	}

	void ThreadPool::push(std::function<void()> func)
	{
		if (func)
		{
			std::unique_lock<std::mutex> lock(this->m_lock);
			this->m_job_stack.push(std::move(func));

			lock.unlock();
			this->m_data_condition.notify_all();
		}
	}

	void ThreadPool::run()
	{
		for (;;)
		{
			std::unique_lock<std::mutex> lock(this->m_lock);

			this->m_data_condition.wait(lock, [this]()
			{
				return !this->m_job_stack.empty() || !this->m_accept_jobs;
			});

			if (!this->m_accept_jobs) return;
			if (this->m_job_stack.empty()) continue;

			auto job = std::move(this->m_job_stack.top());
			this->m_job_stack.pop();
			lock.unlock();

			try
			{
				std::invoke(std::move(job));
			}
			catch (const std::exception& e)
			{
				g_log->warning("THREAD", "Exception thrown while executing job in thread: %s", e.what());
			}
		}

		g_log->info("THREAD", "Thread %d exiting...", std::this_thread::get_id());
	}
}
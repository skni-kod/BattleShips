#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <utility>

namespace network
{

template <typename T> class Queue
{
public:
	Queue() = default;
	Queue(const Queue<T> &) = delete;
	virtual ~Queue() { clear(); }

	const T &front()
	{
		std::scoped_lock lock(mux);
		return deque.front();
	}

	const T &back()
	{
		std::scoped_lock lock(mux);
		return deque.back();
	}

	void push_front(const T &item)
	{
		std::scoped_lock lock(mux);
		deque.emplace_front(std::move(item));

		std::unique_lock<std::mutex> ul(mux_block);
		block.notify_one();
	}

	void push_back(const T &item)
	{
		std::scoped_lock lock(mux);
		deque.emplace_back(std::move(item));

		std::unique_lock<std::mutex> ul(mux_block);
		block.notify_one();
	}

	T pop_front()
	{
		std::scoped_lock lock(mux);
		auto t = std::move(deque.front());
		deque.pop_front();
		return t;
	}

	T pop_back()
	{
		std::scoped_lock lock(mux);
		auto t = std::move(deque.back());
		deque.pop_back();
		return t;
	}

	bool empty()
	{
		std::scoped_lock lock(mux);
		return deque.empty();
	}

	size_t count()
	{
		std::scoped_lock lock(mux);
		return deque.size();
	}

	void clear()
	{
		std::scoped_lock lock(mux);
		deque.clear();
	}

	void wait()
	{
		while (empty())
		{
			std::unique_lock<std::mutex> ul(mux_block);
			block.wait(ul);
		}
	}

protected:
	std::mutex mux;
	std::deque<T> deque;
	std::condition_variable block;
	std::mutex mux_block;
};

} // namespace network

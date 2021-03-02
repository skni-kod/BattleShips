#pragma once

#include "common.hpp"

namespace net
{
template <typename T> class tsqueue
{
public:
	tsqueue() = default;
	tsqueue(const tsqueue<T> &) = delete;
	virtual ~tsqueue() { clear(); }

public:
	const T &front()
	{
		std::scoped_lock lock(queue_mux);
		return dequeue.front();
	}

	const T &back()
	{
		std::scoped_lock lock(queue_mux);
		return dequeue.back();
	}

	T pop_front()
	{
		std::scoped_lock lock(queue_mux);
		auto t = std::move(dequeue.front());
		dequeue.pop_front();
		return t;
	}

	T pop_back()
	{
		std::scoped_lock lock(queue_mux);
		auto t = std::move(dequeue.back());
		dequeue.pop_back();
		return t;
	}

	void push_back(const T &item)
	{
		std::scoped_lock lock(queue_mux);
		dequeue.emplace_back(std::move(item));

		std::unique_lock<std::mutex> ul(blocking_mux);
		cv.notify_one();
	}

	void push_front(const T &item)
	{
		std::scoped_lock lock(queue_mux);
		dequeue.emplace_front(std::move(item));

		std::unique_lock<std::mutex> ul(blocking_mux);
		cv.notify_one();
	}

	bool empty()
	{
		std::scoped_lock lock(queue_mux);
		return dequeue.empty();
	}

	size_t count()
	{
		std::scoped_lock lock(queue_mux);
		return dequeue.size();
	}

	void clear()
	{
		std::scoped_lock lock(queue_mux);
		dequeue.clear();
	}

	void wait()
	{
		while (empty()) {
			std::unique_lock<std::mutex> ul(blocking_mux);
			cv.wait(ul);
		}
	}

protected:
	std::mutex queue_mux;
	std::deque<T> dequeue;
	std::condition_variable cv;
	std::mutex blocking_mux;
};
} // namespace net
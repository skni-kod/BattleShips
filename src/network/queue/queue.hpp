#pragma once

#include <deque>
#include <mutex>
#include <utility>

namespace network
{

template <typename T> class Queue
{
public:
	Queue() = default;
	Queue(const T &) = delete;
	virtual ~Queue() { clear(); }

	const T& front() {
		std::scoped_lock lock(mux);
		return deque.front();
	}

	const T& back() {
		std::scoped_lock lock(mux);
		return deque.back();
	}

	void push_front(const T& item) {
		std::scoped_lock lock(mux);
		deque.emplace_front(std::move(item));
	}

	void push_back(const T& item) {
		std::scoped_lock lock(mux);
		deque.emplace_back(std::move(item));
	}

	T pop_front() {
		std::scoped_lock lock(mux);
		auto t = std::move(deque.front());
		deque.pop_front();
		return t;
	}

	T pop_back() {
		std::scoped_lock lock(mux);
		auto t = std::move(deque.back());
		deque.pop_back();
		return t;
	}

	bool empty() {
		std::scoped_lock lock(mux);
		return deque.empty();
	}

	size_t count() {
		std::scoped_lock lock(mux);
		return deque.count();
	}

	void clear() {
		std::scoped_lock lock(mux);
		deque.clear();
	}

protected:
	std::mutex mux;
	std::deque<T> deque;
};

} // namespace network

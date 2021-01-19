#pragma once

#include "common.hpp"

namespace net
{
/**
 * \brief Klasa kolejki zabezpieczona wątkowo.
 */
template <typename T> class tsqueue
{
public:
	tsqueue() = default;
	tsqueue(const tsqueue<T> &) = delete;
	virtual ~tsqueue() { clear(); }

public:
	/**
	 * \brief Zwraca element na przodzie kolejki.
	 */
	const T &front()
	{
		std::scoped_lock lock(queue_mux);
		return dequeue.front();
	}

	/**
	 * \brief Zwraca element na tyle kolejki.
	 */
	const T &back()
	{
		std::scoped_lock lock(queue_mux);
		return dequeue.back();
	}

	/**
	 * \brief Usuwa i zwraca element na przodzie kolejki.
	 */
	T pop_front()
	{
		std::scoped_lock lock(queue_mux);
		auto t = std::move(dequeue.front());
		dequeue.pop_front();
		return t;
	}

	/**
	 * \brief Usuwa i zwraca element na tyle kolejki.
	 */
	T pop_back()
	{
		std::scoped_lock lock(queue_mux);
		auto t = std::move(dequeue.back());
		dequeue.pop_back();
		return t;
	}

	/**
	 * \brief Dodaje element do tyłu kolejki.
	 */
	void push_back(const T &item)
	{
		std::scoped_lock lock(queue_mux);
		dequeue.emplace_back(std::move(item));

		std::unique_lock<std::mutex> ul(blocking_mux);
		cv.notify_one();
	}

	/**
	 * \brief Dodaje element do przodu kolejki.
	 */
	void push_front(const T &item)
	{
		std::scoped_lock lock(queue_mux);
		dequeue.emplace_front(std::move(item));

		std::unique_lock<std::mutex> ul(blocking_mux);
		cv.notify_one();
	}

	/**
	 * \brief Zwraca true jeśli kolejka jest pusta.
	 */
	bool empty()
	{
		std::scoped_lock lock(queue_mux);
		return dequeue.empty();
	}

	/**
	 * \brief Zwraca liczbę elementów kolejki.
	 */
	size_t count()
	{
		std::scoped_lock lock(queue_mux);
		return dequeue.size();
	}

	/**
	 * \brief Usuwa wszystkie elementy kolejki.
	 */
	void clear()
	{
		std::scoped_lock lock(queue_mux);
		dequeue.clear();
	}

	/**
	 * \brief Metoda blokująca.
	 */
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
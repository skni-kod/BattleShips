#pragma once

#define ASIO_STANDALONE

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <iostream>

namespace network
{

enum class MessageType {
	Connection,
	Shots,
};

struct MessageHeader {
	MessageType type;
	size_t size;
};

class Message
{
	MessageHeader header;
	std::vector<uint8_t> body;

	size_t size() const { return sizeof(header) + body.size(); }

public:
	template <typename T> void add(const T &data)
	{
		static_assert(std::is_standard_layout<T>::value, "Data cannot be copied!");

		size_t last_size = body.size();

		body.resize(body.size() + sizeof(T));

		std::memcpy(body.data() + last_size, &data, sizeof(T));

		header.size = size();
	}

	template <typename T> void pop(T &data)
	{
		static_assert(std::is_standard_layout<T>::value, "Data cannot be copied!");

		size_t size_after = body.size() - sizeof(T);

		std::memcpy(&data, body.data() + size_after, sizeof(T));

		body.resize(size_after);

		header.size = size();
	}
};

} // namespace network

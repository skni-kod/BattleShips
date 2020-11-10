#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <ostream>
#include <vector>

namespace network
{

template <typename MessageType> struct MessageHeader {
	MessageType type;
	size_t size;
};

template <typename MessageType> struct Message {
	MessageHeader<MessageType> header;
	std::vector<std::uint8_t> body;

	size_t size() const { return sizeof(header) + body.size(); }

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

template <typename MessageType> class Connection;

template <typename MessageType> struct OwnedMessage {
	std::shared_ptr<Connection<MessageType>> remote = nullptr;
	Message<MessageType> message;

	friend std::ostream &operator<<(std::ostream &os, const OwnedMessage<MessageType> &msg)
	{
		os << msg.message;
		return os;
	}
};

} // namespace network

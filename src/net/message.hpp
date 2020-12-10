#pragma once

#include "common.hpp"

namespace net
{
// Message Header is sent at start of all messages. The template allows us
// to use "enum class" to ensure that the messages are valid at compile time
template <typename T> struct message_header {
	T id{};
	uint32_t size = 0;
};

// Message Body contains a header and a std::vector, containing raw bytes
// of infomation. This way the message can be variable length, but the size
// in the header must be updated.
template <typename T> struct message {
	message_header<T> header{};
	std::vector<uint8_t> body;

	size_t size() const { return body.size(); }

	// Produces description of message
	friend std::ostream &operator<<(std::ostream &os, const message<T> &msg)
	{
		os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
		return os;
	}

	// Pushes any data into the message buffer
	template <typename DataType> friend message<T> &operator<<(message<T> &msg, const DataType &data)
	{
		// DataType has to be trivally copyiable
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

		size_t i = msg.body.size();

		msg.body.resize(msg.body.size() + sizeof(DataType));

		std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

		msg.header.size = static_cast<uint32_t>(msg.size());

		return msg;
	}

	// Pulls any data form the message buffer
	template <typename DataType> friend message<T> &operator>>(message<T> &msg, DataType &data)
	{
		// DataType has to be trivally copyiable
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

		size_t i = msg.body.size() - sizeof(DataType);

		std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

		msg.body.resize(i);

		msg.header.size = static_cast<uint32_t>(msg.size());

		return msg;
	}
};

// The owned_message is a regular message, but it is associated with a connection.
// On a server, the owner would be the client that sent the message, on a client the server.

template <typename T> class connection;

template <typename T> struct owned_message {
	std::shared_ptr<connection<T>> remote = nullptr;
	message<T> msg;

	friend std::ostream &operator<<(std::ostream &os, const owned_message<T> &message)
	{
		os << message.msg;
		return os;
	}
};
} // namespace net


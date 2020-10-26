#pragma once

#define ASIO_STANDALONE

#include <iostream>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

namespace network {
	void test() {
		asio::error_code e;
		asio::io_context context;

		asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34"), 80);

		asio::ip::tcp::socket socket(context);

		socket.connect(endpoint, e);

		if(!e)
			std::cout << "Connected!\n";
		else
			std::cout << "Failed to connect to address:\n" << e.message();
	}
}

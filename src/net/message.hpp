#pragma once

#include "common.hpp"

namespace net
{
/**
 * \brief Struktura nagłówka wiadomości.
 * Przechowuje typ oraz rozmiar wiadomości.
 */
template <typename T> struct message_header {
	T id{};
	uint32_t size = 0;
};

/**
 * \brief Struktura wiadomości.
 * Ciało wiadomości przechowywane jest w wektorze, jeżeli zmieniamy jego długość
 * to trzeba zaktualizować nagłówek.
 */
template <typename T> struct message {
	message_header<T> header{};
	std::vector<uint8_t> body;

	/**
	 * \brief Metoda zwracająca rozmiar ciała wiadomości.
	 */
	size_t size() const { return body.size(); }

	/**
	 * \brief Funkcja wyświetlajaca wiadomość.
	 */
	friend std::ostream &operator<<(std::ostream &os, const message<T> &msg)
	{
		os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
		return os;
	}

	/**
	 * \brief Metoda dodająca dane do wiadomości.
	 * Dane muszą być linearnymi strukturami danych.
	 */
	template <typename DataType> friend message<T> &operator<<(message<T> &msg, const DataType &data)
	{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

		size_t i = msg.body.size();

		msg.body.resize(msg.body.size() + sizeof(DataType));

		std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

		msg.header.size = static_cast<uint32_t>(msg.size());

		return msg;
	}

	/**
	 * \brief Metoda wyciągająca dane z wiadomości.
	 * Dane muszą być linearnymi strukturami danych.
	 */
	template <typename DataType> friend message<T> &operator>>(message<T> &msg, DataType &data)
	{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

		size_t i = msg.body.size() - sizeof(DataType);

		std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

		msg.body.resize(i);

		msg.header.size = static_cast<uint32_t>(msg.size());

		return msg;
	}
};

template <typename T> class connection;

/**
 * \brief Struktura wiadomości z właścicielem.
 * Zawiera wskaźnik współdzielony do serwera jeśli jest to wiadomość serwera.
 */
template <typename T> struct owned_message {
	std::shared_ptr<connection<T>> remote = nullptr;
	message<T> msg;

	/**
	 * \brief Metoda wywołująca opertor wypisania wiadomości.
	 */
	friend std::ostream &operator<<(std::ostream &os, const owned_message<T> &message)
	{
		os << message.msg;
		return os;
	}
};
} // namespace net


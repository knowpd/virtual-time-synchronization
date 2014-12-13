/**
 * Copyright (c) 2014 Hee Won Lee <knowpd@gmail.com>
 *
 * This file is part of TdfAgent.
 *
 * TdfAgent is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TdfAgent is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UDPASYNCSERVER_HPP_
#define UDPASYNCSERVER_HPP_

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <log4cxx/logger.h>
#include "../MemoryDatabase.hpp"
#include "../MessageProcessor.hpp"

using boost::asio::ip::udp;

class UdpAsyncServer {
public:
	UdpAsyncServer(boost::asio::io_service& io_service, string& port, MemoryDatabase& memDb);
	void startReceiveFrom();
	void handleReceiveFrom(const boost::system::error_code& error,
			size_t bytes_recvd);
	void handleSendTo(const boost::system::error_code& /*error*/,
			size_t /*bytes_sent*/);

private:
	static log4cxx::LoggerPtr logger;
	boost::asio::io_service& ioService_;
	udp::socket socket_;
	udp::endpoint remoteEndpoint_;
	enum {
		MAX_LENGTH = 1024
	};
	char data_[MAX_LENGTH];
	MessageProcessor msgProcessor_;
};

#endif /* UDPASYNCSERVER_HPP_ */

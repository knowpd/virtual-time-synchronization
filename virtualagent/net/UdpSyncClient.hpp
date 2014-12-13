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

#ifndef UDPSYNCCLIENT_HPP_
#define UDPSYNCCLIENT_HPP_

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::udp;

class UdpSyncClient {
public:
	UdpSyncClient(boost::asio::io_service& ioService, const std::string& host,
			const std::string& service);
	void sendTo(std::string msg);
	void sendTo(char* msg, size_t length);
	void close();

private:
	boost::asio::io_service& ioService_;
	udp::socket socket_;
	udp::endpoint remoteEndpoint_;
};

#endif /* UDPSYNCCLIENT_HPP_ */

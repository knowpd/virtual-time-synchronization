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

#include "UdpSyncClient.hpp"

UdpSyncClient::UdpSyncClient(boost::asio::io_service& ioService,
		const std::string& host, const std::string& service) :
		ioService_(ioService), socket_(ioService, udp::endpoint(udp::v4(), 0)) {
	boost::asio::socket_base::broadcast option(true);
	socket_.set_option(option);

	udp::resolver resolver(ioService);
	udp::resolver::query query(udp::v4(), host, service);
	udp::resolver::iterator iterator = resolver.resolve(query);
	remoteEndpoint_ = *iterator;
}

void UdpSyncClient::sendTo(std::string msg) {
	socket_.send_to(boost::asio::buffer(msg), remoteEndpoint_);
}

void UdpSyncClient::sendTo(char* msg, size_t length) {
	socket_.send_to(boost::asio::buffer(msg, length), remoteEndpoint_);
}

void UdpSyncClient::close() {
	socket_.close();
}

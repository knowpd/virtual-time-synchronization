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

#include "UdpAsyncServer.hpp"

log4cxx::LoggerPtr UdpAsyncServer::logger(log4cxx::Logger::getLogger("UdpAsyncServer"));

UdpAsyncServer::UdpAsyncServer(boost::asio::io_service& io_service, string& port,
		MemoryDatabase& memDb) :
		ioService_(io_service), socket_(io_service,
				udp::endpoint(udp::v4(), atoi(port.c_str()))), msgProcessor_(memDb) {
	startReceiveFrom();
}

void UdpAsyncServer::startReceiveFrom() {
	socket_.async_receive_from(boost::asio::buffer(data_, MAX_LENGTH),
			remoteEndpoint_,
			boost::bind(&UdpAsyncServer::handleReceiveFrom, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

void UdpAsyncServer::handleReceiveFrom(const boost::system::error_code& error,
		size_t bytes_recvd) {
	if (!error && bytes_recvd > 0) {
		strcpy(data_ + bytes_recvd, "");
		LOG4CXX_DEBUG(logger, "data = " << data_);
		msgProcessor_.processUdpMsg(data_);
		/*
		 socket_.async_send_to(
		 boost::asio::buffer(data_, bytes_recvd), remoteEndpoint_,
		 boost::bind(&UdpAsyncServer::handleSendTo, this,
		 boost::asio::placeholders::error,
		 boost::asio::placeholders::bytes_transferred));
		 */
	}
	startReceiveFrom();
}

void UdpAsyncServer::handleSendTo(const boost::system::error_code& /*error*/,
		size_t /*bytes_sent*/) {
	startReceiveFrom();
}

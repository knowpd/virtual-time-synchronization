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

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "../MemoryDatabase.hpp"
#include "../MessageProcessor.hpp"

using boost::asio::ip::udp;

std::string make_daytime_string() {
	using namespace std;
	// For time_t, time and ctime;
	time_t now = time(0);
	return ctime(&now);
}

class UdpAsyncServer {
public:
	enum { MAX_BUFFER_SIZE = 512 };

	UdpAsyncServer(boost::asio::io_service& io_service,
			const udp::endpoint& endpoint, MemoryDatabase& memDb) :
			ioService_(io_service),
			socket_(io_service, udp::endpoint(udp::v4(), 13)),
			msgProcessor_(memDb){
		startReceive();
	}

private:
	void startReceive() {
		socket_.async_receive_from(boost::asio::buffer(recvBuffer_),
				remoteEndpoint_,
				boost::bind(&UdpAsyncServer::handleReceive, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
	}

	void handleReceive(const boost::system::error_code& error,
			std::size_t /*bytes_transferred*/) {
		//msgProcessor_.parse(recvBuffer_);

		std::cout << "recvBuffer_ = " << recvBuffer_ << std::endl;

		if (!error || error == boost::asio::error::message_size) {
			boost::shared_ptr<std::string> message(
					new std::string(make_daytime_string()));

//			socket_.async_send_to(boost::asio::buffer(*message),
//					remoteEndpoint_,
//					boost::bind(&UdpAsyncServer::handleSend, this, message,
//							boost::asio::placeholders::error,
//							boost::asio::placeholders::bytes_transferred));

			startReceive();
		}
	}

	void handleSend(boost::shared_ptr<std::string> /*message*/,
			const boost::system::error_code& /*error*/,
			std::size_t /*bytes_transferred*/) {
	}

	boost::asio::io_service& ioService_;
	udp::socket socket_;
	udp::endpoint remoteEndpoint_;
	//boost::array<char, MAX_BUFFER_SIZE> recvBuffer_;
	char recvBuffer_[MAX_BUFFER_SIZE];
	MessageProcessor msgProcessor_;
};

typedef boost::shared_ptr<UdpAsyncServer> udpAsyncServerPtr;

int main() {
	MemoryDatabase memDb;
	try {
		unsigned short udpPort = 9997;
		boost::asio::io_service io_service;
		udp::endpoint endpoint(udp::v4(), udpPort);
		udpAsyncServerPtr udpAsyncServer(
				new UdpAsyncServer(io_service, endpoint, memDb));
		io_service.run();
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}

#endif /* UDPASYNCSERVER_HPP_ */

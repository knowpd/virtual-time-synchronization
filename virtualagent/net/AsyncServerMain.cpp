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

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "TcpAsyncServer.hpp"
#include "UdpAsyncServer.hpp"
#include "UdpSyncClient.hpp"
#include "RawMessage.hpp"
#include "../MemoryDatabase.hpp"
#include "../SharedMemory.hpp"

using boost::asio::ip::tcp;
using namespace std;

MemoryDatabase memDb;

enum {
	MAX_LENGTH = 1024
};

int main(int argc, char* argv[]) {

	try {
		string tcpPort = Configuration::tcpPort;
		string udpPort = Configuration::udpPort;
		string broadcastIp = "255.255.255.255";

		boost::asio::io_service ioService;
		TcpAsyncServer tcpAsyncServer(ioService, tcpPort, memDb);
		UdpAsyncServer udpAsyncServer(ioService, udpPort, memDb);
		boost::thread thrdIoService(
				boost::bind(&boost::asio::io_service::run, &ioService));
		UdpSyncClient udpClient(ioService, broadcastIp, udpPort);

		for (;;) {
			std::cout << "Enter message: ";
			char request[MAX_LENGTH]; //="0002|hightemplar|3.6";
			std::cin.getline(request, MAX_LENGTH);
			size_t requestLength = std::strlen(request);
			udpClient.sendTo(request, requestLength);
		}

		thrdIoService.join();

	} catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}


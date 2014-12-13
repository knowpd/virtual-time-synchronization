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

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "UdpSyncClient.hpp"

using boost::asio::ip::udp;

enum {
	MAX_LENGTH = 1024
};

int main(int argc, char* argv[]) {
	try {
		if (argc != 3) {
			std::cerr << "Usage: UdpSyncClientMain <host> <port>\n";
			return 1;
		}

//    argv[1] = "localhost";
//    argv[2] = "9997";

		boost::asio::io_service ioService;
		UdpSyncClient udpClient(ioService, argv[1], argv[2]);

		for (;;) {
			std::cout << "Enter message: ";
			char request[MAX_LENGTH]; //="0002|hightemplar|3.6";
			std::cin.getline(request, MAX_LENGTH);
			size_t requestLength = std::strlen(request);
			udpClient.sendTo(request, requestLength);
		}

		//udpClient.sendTo("0002|terran|2.7");

	} catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

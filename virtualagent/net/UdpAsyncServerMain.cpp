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

// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "../MemoryDatabase.hpp"
#include "../MessageProcessor.hpp"
#include "UdpAsyncServer.hpp"

using boost::asio::ip::udp;

MemoryDatabase memDb;

void printDb() {
	for (;;) {
		static int count = 0;
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		cout << "----->count = " << count++ << endl;
		memDb.tdfTable.print();
	}
}

int main(int argc, char* argv[]) {
	boost::thread thrd(&printDb);

	try {
		string udpPort = "9997";
		boost::asio::io_service ioService;
		UdpAsyncServer udpAsyncServer(ioService, udpPort, memDb);
		ioService.run();
	} catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	thrd.join();

	return 0;
}

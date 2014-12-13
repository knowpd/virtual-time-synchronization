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
#include "RawMessage.hpp"
#include "../MemoryDatabase.hpp"
#include "../SharedMemory.hpp"

using boost::asio::ip::tcp;
using namespace std;

MemoryDatabase memDb;

void printDb()
{
	for(;;){
		static int count = 0;
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		cout << "----->count = " << count++ << endl;
		memDb.ifaceAttrTable.print();
	}
}

typedef boost::shared_ptr<TcpAsyncServer> tcpAsyncServerPtr;

int main(int argc, char* argv[]) {


    boost::thread thrd(&printDb);

	try {
		string tcpPort = "9998";
		boost::asio::io_service ioService;
		tcpAsyncServerPtr tcpAsyncServer(new TcpAsyncServer(ioService, tcpPort, memDb));
		ioService.run();

	} catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

    thrd.join();

	return 0;
}


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
#include "TcpAsyncServer.hpp"
#include "RawMessage.hpp"
#include "../MemoryDatabase.hpp"
#include "../MessageProcessor.hpp"

using boost::asio::ip::tcp;
using namespace std;

int main(int argc, char* argv[]) {
	MemoryDatabase myDb;

//	myDb.tdfTable.set("terran", 3.0);
//	myDb.tdfTable.set("hightemplar", 5.0);
//	myDb.tdfTable.set("terran", 4.0);
//	myDb.tdfTable.print();
//
//	//InterfaceAttributeTable ifaceAttrTable;
//	myDb.ifaceAttrTable.set("eth0", 1000, 2000);
//	myDb.ifaceAttrTable.set("eth1", 3000, 4000);
//	myDb.ifaceAttrTable.print();

	//string str1("0001|eth2|50000|60000");
	RawMessage msg;
	strcpy(msg.data(), "250|0001|eth2|50000|60000");
	msg.encode_header();
	//printf("%s", msg.data());
	MessageProcessor msgPro(myDb);
	msgPro.parse(msg);
	myDb.ifaceAttrTable.print();

	//cout << "terran = " << ifaceAttrTable.get("eth0")->rate << endl;
	//cout << "hightemplar = " << ifaceAttrTable.get("eth0")->delay << endl;

	return 0;
}

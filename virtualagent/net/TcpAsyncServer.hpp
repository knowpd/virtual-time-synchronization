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

#ifndef TCPASYNCSERVER_HPP_
#define TCPASYNCSERVER_HPP_

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
#include <log4cxx/logger.h>
#include "RawMessage.hpp"
#include "../MessageProcessor.hpp"
#include "../MemoryDatabase.hpp"
#include "TcpSession.hpp"

using boost::asio::ip::tcp;

typedef boost::shared_ptr<TcpSession> tcp_session_ptr;

class TcpAsyncServer {
public:
	TcpAsyncServer(boost::asio::io_service& ioService, string& port,
			MemoryDatabase& memDb);
	void startAccept();
	void handleAccept(tcp_session_ptr session,
			const boost::system::error_code& error);

private:
	static log4cxx::LoggerPtr logger;
	boost::asio::io_service& ioService_;
	tcp::acceptor acceptor_;
	TcpConnectionManager connMgr_;
	MemoryDatabase& memDb_;
};

#endif // TCPASYNCSERVER_HPP_

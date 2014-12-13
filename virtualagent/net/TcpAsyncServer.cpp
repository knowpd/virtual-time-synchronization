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

#include "TcpAsyncServer.hpp"

using boost::asio::ip::tcp;

log4cxx::LoggerPtr TcpAsyncServer::logger(log4cxx::Logger::getLogger("TcpAsyncServer"));

TcpAsyncServer::TcpAsyncServer(boost::asio::io_service& ioService,
		string& port, MemoryDatabase& memDb)
	: ioService_(ioService),
	  acceptor_(ioService, tcp::endpoint(tcp::v4(), atoi(port.c_str()))),
	  memDb_(memDb){
	startAccept();
}

void TcpAsyncServer::startAccept() {
	tcp_session_ptr new_session(new TcpSession(ioService_, connMgr_, memDb_));
	acceptor_.async_accept(new_session->socket(),
			boost::bind(&TcpAsyncServer::handleAccept, this, new_session,
					boost::asio::placeholders::error));
}

void TcpAsyncServer::handleAccept(tcp_session_ptr session,
		const boost::system::error_code& error) {
	if (!error) {
		session->start();
	}

	startAccept();
}


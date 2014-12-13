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

#include "TcpSession.hpp"

TcpSession::TcpSession(boost::asio::io_service& io_service,
		TcpConnectionManager& conn_mgr, MemoryDatabase& memDb) :
		socket_(io_service), tcpConnMgr_(conn_mgr), msgProcessor_(memDb) {
}

tcp::socket& TcpSession::socket() {
	return socket_;
}

void TcpSession::start() {
	tcpConnMgr_.join(shared_from_this());
	boost::asio::async_read(socket_,
			boost::asio::buffer(readMsg_.data(), RawMessage::HEADER_LENGTH),
			boost::bind(&TcpSession::handleReadHeader, shared_from_this(),
					boost::asio::placeholders::error));
}

void TcpSession::deliver(const RawMessage& msg) {
	bool write_in_progress = !writeMsgs_.empty();
	writeMsgs_.push_back(msg);
	if (!write_in_progress) {
		boost::asio::async_write(socket_,
				boost::asio::buffer(writeMsgs_.front().data(),
						writeMsgs_.front().length()),
				boost::bind(&TcpSession::handleWrite, shared_from_this(),
						boost::asio::placeholders::error));
	}
}

void TcpSession::handleReadHeader(const boost::system::error_code& error) {
	if (!error && readMsg_.decode_header()) {
		boost::asio::async_read(socket_,
				boost::asio::buffer(readMsg_.body(), readMsg_.body_length()),
				boost::bind(&TcpSession::handleReadBody, shared_from_this(),
						boost::asio::placeholders::error));
	} else {
		tcpConnMgr_.leave(shared_from_this());
	}
}

void TcpSession::handleReadBody(const boost::system::error_code& error) {
	if (!error) {
		//ConnMgr_.deliver(readMsg_);
		strcpy(readMsg_.body() + readMsg_.body_length(), "");
		msgProcessor_.processTcpMsg(readMsg_.body());

		boost::asio::async_read(socket_,
				boost::asio::buffer(readMsg_.data(), RawMessage::HEADER_LENGTH),
				boost::bind(&TcpSession::handleReadHeader, shared_from_this(),
						boost::asio::placeholders::error));
	} else {
		tcpConnMgr_.leave(shared_from_this());
	}
}

void TcpSession::handleWrite(const boost::system::error_code& error) {
	if (!error) {
		writeMsgs_.pop_front();
		if (!writeMsgs_.empty()) {
			boost::asio::async_write(socket_,
					boost::asio::buffer(writeMsgs_.front().data(),
							writeMsgs_.front().length()),
					boost::bind(&TcpSession::handleWrite, shared_from_this(),
							boost::asio::placeholders::error));
		}
	} else {
		tcpConnMgr_.leave(shared_from_this());
	}
}

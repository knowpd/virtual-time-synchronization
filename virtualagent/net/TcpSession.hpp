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

#ifndef TCPSESSION_HPP_
#define TCPSESSION_HPP_

#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "RawMessage.hpp"
#include "../MessageProcessor.hpp"
#include "../MemoryDatabase.hpp"
#include "TcpConnectionManager.hpp"

using boost::asio::ip::tcp;

typedef std::deque<RawMessage> rawMessageQueue;

class TcpSession
  : public ConnParticipant,
    public boost::enable_shared_from_this<TcpSession>
{
public:
  TcpSession(boost::asio::io_service& io_service, TcpConnectionManager& conn_mgr, MemoryDatabase& memDb);
  tcp::socket& socket();
  void start();
  void deliver(const RawMessage& msg);
  void handleReadHeader(const boost::system::error_code& error);
  void handleReadBody(const boost::system::error_code& error);
  void handleWrite(const boost::system::error_code& error);

private:
  tcp::socket socket_;
  TcpConnectionManager& tcpConnMgr_;
  RawMessage readMsg_;
  rawMessageQueue writeMsgs_;
  MessageProcessor msgProcessor_;
  //MemoryDatabase& memDb_;
};


#endif /* TCPSESSION_HPP_ */

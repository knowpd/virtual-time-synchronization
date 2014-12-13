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

#ifndef TCPCONNECTIONMANAGER_HPP_
#define TCPCONNECTIONMANAGER_HPP_

#include <deque>
#include <iostream>
#include <set>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "RawMessage.hpp"

typedef std::deque<RawMessage> rawMessageQueue;

//-----------------------------------------------------------------
class ConnParticipant
{
public:
  virtual ~ConnParticipant() {}
  virtual void deliver(const RawMessage& msg) = 0;
};


//-----------------------------------------------------------------
typedef boost::shared_ptr<ConnParticipant> connParticipantPtr;

class TcpConnectionManager
{
public:
  void join(connParticipantPtr participant);
  void leave(connParticipantPtr participant);
  void deliver(const RawMessage& msg);

private:
  std::set<connParticipantPtr> participants_;
  enum { MAX_RECENT_MSGS = 100 };
  rawMessageQueue recentMsgs_;
};


#endif /* TCPCONNECTIONMANAGER_HPP_ */

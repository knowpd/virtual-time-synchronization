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

// reference: http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html


#ifndef VIRTUALAGENT_HPP_
#define VIRTUALAGENT_HPP_

#include <sys/types.h>
#include <cstdio>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
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
#include <log4cxx/logger.h>
#include "net/TcpAsyncServer.hpp"
#include "net/UdpAsyncServer.hpp"
#include "net/UdpSyncClient.hpp"
#include "net/RawMessage.hpp"
#include "MemoryDatabase.hpp"
#include "VirtualTime.hpp"
#include "SharedMemory.hpp"
#include "CpuMonitor.hpp"
#include "SignalHandler.hpp"

//#include "SignalHandler.hpp"

class VirtualAgent {
public:

	VirtualAgent();
	void start();

private:
	static log4cxx::LoggerPtr logger;
	string tcpPort_;
	string udpPort_;
	string broadcastIp_;
};

#endif /* VIRTUALAGENT_HPP_ */


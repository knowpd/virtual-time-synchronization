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

#ifndef MESSAGE_PROCESSOR_HPP
#define MESSAGE_PROCESSOR_HPP

#include "InterfaceConfigurator.hpp"
#include <boost/algorithm/string/classification.hpp> // for boost::is_any_of()
#include <boost/algorithm/string/split.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <log4cxx/logger.h>
#include "MemoryDatabase.hpp"
#include "net/RawMessage.hpp"
#include "Configuration.hpp"
#include "shared/bin_sem.h"

using namespace std;

class MessageProcessor {

public:
	enum {
		MAX_LENGTH = 1024
	};

	MessageProcessor(MemoryDatabase& memDb);
	void processUdpMsg(const char* udpMsg);
	void processTcpMsg(const char* tcpMsg);

private:
	void parseUdpMsg(string msg);
	void parseTcpMsg(string msg);

	static log4cxx::LoggerPtr logger;
	MemoryDatabase& memDb_;
};

#endif // MESSAGE_PROCESSOR_HPP

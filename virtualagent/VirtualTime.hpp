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

#ifndef VIRTUALTIME_HPP_
#define VIRTUALTIME_HPP_

#include <iostream>
#include <boost/thread.hpp>
#include <log4cxx/logger.h>
//#include "shared/qemu-timer.h"
#include "shared/bin_sem.h"
#include "SharedMemory.hpp"
#include "MemoryDatabase.hpp"
#include "SignalHandler.hpp"
#include "InterfaceConfigurator.hpp"
#include "CpuMonitor.hpp"
#include "TimeGetter.hpp"

class VirtualTime {
public:
	VirtualTime(MemoryDatabase& memDb);
	void operator ()();
	void start();
	void startAdaptiveVirtualTime(void);
	void __testIfVirtualTimeProceeds(void); // Code for testing

private:
	static log4cxx::LoggerPtr logger;
	MemoryDatabase& memDb_;
	//InterfaceConfigurator ifaceConfig_;
	//struct timespec time_slice;
	//struct timespec time_sleep;
	//struct timespec sem_timeout;
};

#endif /* VIRTUALTIME_HPP_ */

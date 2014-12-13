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

#ifndef CPUMONITOR_HPP_
#define CPUMONITOR_HPP_

#include <iostream>
#include <locale.h>
#include <libintl.h>
#include <math.h>
#include <cstdio>
#include <unistd.h>
#include <cmath>
#include <glibtop.h>
#include <glibtop/cpu.h>
#include <sys/time.h>
#include <log4cxx/logger.h>
#include "net/UdpSyncClient.hpp"
#include "MemoryDatabase.hpp"
#include "SignalHandler.hpp"
#include "Configuration.hpp"

using namespace std;

class CpuMonitor {
public:
	CpuMonitor(UdpSyncClient& udpClient, MemoryDatabase& memDb);
	void operator ()();
	void start();
	double getCpuIdleRatio();
	double getMinXCpuIdleRatio();
	double getAvgMinXCpuIdleRatio();

	static double avgMinXCpuIdleRatio; // For exponential weighted moving average
	static double curMinXCpuIdleRatio;

	static int64_t avgSimStateOverheadDelay; // For exponential weighted moving average

private:
	double sign(double t);

	static log4cxx::LoggerPtr logger;
	UdpSyncClient& udpClient_;
	MemoryDatabase& memDb_;
	unsigned long frequency_;
	double total_;
	double idle_;
	vector<double> xcpu_total_;
	vector<double> xcpu_idle_;
};

#endif /* CPUMONITOR_HPP_ */

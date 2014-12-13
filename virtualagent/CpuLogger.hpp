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

#ifndef CPULOGGER_HPP_
#define CPULOGGER_HPP_

#include <iostream>
#include <locale.h>
#include <libintl.h>
#include <math.h>
#include <cstdio>
#include <unistd.h>
#include <vector>
#include <glibtop.h>
#include <glibtop/cpu.h>
#include <sys/time.h>
#include <log4cxx/logger.h>

using namespace std;

class CpuLogger {
public:
	CpuLogger();
	double getCpuIdleRatio();
	double getMinXCpuIdleRatio();

private:
	static log4cxx::LoggerPtr logger;
	unsigned long frequency_;
	double total_;
	double idle_;
	vector<double> xcpu_total_;
	vector<double> xcpu_idle_;
};

#endif /* CPULOGGER_HPP_ */

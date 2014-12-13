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

#include "CpuLogger.hpp"

log4cxx::LoggerPtr CpuLogger::logger(log4cxx::Logger::getLogger("CpuLogger"));

CpuLogger::CpuLogger() {
	LOG4CXX_TRACE(logger,
			"CpuLogger::CpuLogger()");

	glibtop_cpu cpu;
	glibtop_get_cpu(&cpu);
	frequency_ = (unsigned long) cpu.frequency;
	total_ = ((unsigned long) cpu.total) ? ((double) cpu.total) : 1.0;
	idle_ = ((unsigned long) cpu.idle) ? ((double) cpu.idle) : 1.0;

	for (int i = 0; i <= glibtop_global_server->ncpu; i++) {
		double xcpu_total =
				((unsigned long) cpu.xcpu_total[i]) ?
						((double) cpu.xcpu_total[i]) : 1.0;
		double xcpu_idle =
				((unsigned long) cpu.xcpu_idle[i]) ?
						((double) cpu.xcpu_idle[i]) : 1.0;
		xcpu_total_.push_back(xcpu_total);
		xcpu_idle_.push_back(xcpu_idle);
	}
}

double CpuLogger::getMinXCpuIdleRatio() {
	LOG4CXX_TRACE(logger, "CpuLogger::getMinXCpuIdleRatio()");

//	vector<double> xcpu_total, xcpu_idle;
//	vector<double> diff_xcpu_total, diff_xcpu_idle;

	double min_xcpu_idle_ratio = 1.0;
	glibtop_cpu cpu;
	glibtop_get_cpu(&cpu);

	for (int i = 0; i <= glibtop_global_server->ncpu; i++) {

		double xcpu_total =
				((unsigned long) cpu.xcpu_total[i]) ?
						((double) cpu.xcpu_total[i]) : 1.0;
		double xcpu_idle =
				((unsigned long) cpu.xcpu_idle[i]) ?
						((double) cpu.xcpu_idle[i]) : 1.0;

		double diff_xcpu_total = xcpu_total - xcpu_total_[i];
		double diff_xcpu_idle = xcpu_idle - xcpu_idle_[i];

		//cout << " " << xcpu_total_[0];
		//for(int i=0; i< xcpu_total_.size(); i++){
		//	cout << " " << xcpu_total_[i];
		//}

		xcpu_total_[i] = xcpu_total;
		xcpu_idle_[i] = xcpu_idle;

		double xcpu_idle_ratio = diff_xcpu_idle / diff_xcpu_total;

		if (isnan(xcpu_idle_ratio))
			xcpu_idle_ratio = xcpu_idle / xcpu_total;

//		cout << "diff_xcpu_total[" << i << "] = " << diff_xcpu_total;
//		cout << ", diff_xcpu_idle[" << i << "] = " << diff_xcpu_idle << endl;
//		cout << "xcpu_idle_ratio[" << i << "] = " << xcpu_idle_ratio << endl;

		if (i == 0)
			min_xcpu_idle_ratio = xcpu_idle_ratio;
		else if (min_xcpu_idle_ratio > xcpu_idle_ratio)
			min_xcpu_idle_ratio = xcpu_idle_ratio;

	}

	LOG4CXX_DEBUG(logger, "min_xcpu_idle_ratio = " << min_xcpu_idle_ratio);
	return min_xcpu_idle_ratio;
}


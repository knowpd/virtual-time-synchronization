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

#include "CpuMonitor.hpp"

log4cxx::LoggerPtr CpuMonitor::logger(log4cxx::Logger::getLogger("CpuMonitor"));

double CpuMonitor::avgMinXCpuIdleRatio = 1.0;
double CpuMonitor::curMinXCpuIdleRatio = 1.0;

int64_t CpuMonitor::avgSimStateOverheadDelay = Configuration::targetDelay;


CpuMonitor::CpuMonitor(UdpSyncClient& udpClient, MemoryDatabase& memDb) :
		udpClient_(udpClient), memDb_(memDb) {
	LOG4CXX_TRACE(logger,
			"CpuMonitor::CpuMonitor(UdpSyncClient& udpClient, MemoryDatabase& memDb)");
	// Initialize alpha of exponential weighted moving average

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

void CpuMonitor::operator ()() {
	LOG4CXX_TRACE(logger, "CpuMonitor::operator ()()");

	start();
}

void CpuMonitor::start() {
	LOG4CXX_TRACE(logger, "CpuMonitor::start()");

	// Get hostname
	char hostname[256];
	size_t len = 255;
	gethostname(hostname, len);

	// Define TDF_load
	long double tdfLoad = Configuration::tdfMin;
	long double tdfSim = Configuration::tdfMin;

	LOG4CXX_TRACE(logger, "CpuMonitor::start(): CK1001");

	// Main loop
	while (SignalHandler::isRunning) {
		if(Configuration::isLoadMonitored){
			if (Configuration::monitorMode == 0 || Configuration::monitorMode == 2){
				// Set a target CPU Idle ratio
				double targCpuIdleRatio = 1.0 - Configuration::targetLoad;
				LOG4CXX_INFO(logger, "Configuration::targetLoad = " << Configuration::targetLoad);

				// Get exponential weighted moving average of minimum XCpuIdle ratio
				double curAvgMinXCpuIdleRatio = getAvgMinXCpuIdleRatio();

				// Get latest TDF
				//long double currTdf = memDb_.tdfTable.getLatestTdf();		// currTdf won't be used here

				// Calculate required TDF
				double linearIncrement = (curAvgMinXCpuIdleRatio - targCpuIdleRatio)
										/ (1.0 - targCpuIdleRatio);
				double absOflinearIncrement = fabs(linearIncrement);
				double signOflinearIncrement = sign(linearIncrement);
				double nonlinearIncrement = pow(absOflinearIncrement, Configuration::nonlinearFactor);
				double TwoPowOfNonlinearFactor = pow(2, Configuration::nonlinearFactor);

				tdfLoad = tdfLoad
						- Configuration::changeSlope * TwoPowOfNonlinearFactor * signOflinearIncrement * nonlinearIncrement;

				LOG4CXX_DEBUG(logger, "tdfLoad = " << tdfLoad);

				// Set a boundary
				if (tdfLoad < Configuration::tdfMin)
					tdfLoad = Configuration::tdfMin;
				else if (tdfLoad > Configuration::tdfMax)
					tdfLoad = Configuration::tdfMax;
			}

			if(Configuration::monitorMode == 1 || Configuration::monitorMode == 2){
				uint64_t simStateTime = memDb_.sharedMemory.simulatorsStatePtr_->time;
				uint64_t simStateOverheadDelay = memDb_.sharedMemory.simulatorsStatePtr_->overhead_delay;
				uint64_t currentTime = TimeGetter::getTimeUs();
				uint64_t timeDiff;

				// Code for filtering old simulators_state in shared memory
				if (simStateTime > currentTime)
					timeDiff = simStateTime - currentTime;
				else
					timeDiff = currentTime -simStateTime;

				//LOG4CXX_TRACE(logger, "CpuMonitor::start(): CK3: " << simStateTime << " " << currentTime << " " << timeDiff);
				// Main control formula logic
				if ( timeDiff < 10000000){ // 10000000us = 10s
					//LOG4CXX_TRACE(logger, "CpuMonitor::start(): CK4");

					// Get exponential weighted moving average
					//alpha = 0.125; // 0.125, 0.0625, 0.0312, 0.0156
					avgSimStateOverheadDelay = (1.0-Configuration::alpha)*avgSimStateOverheadDelay+Configuration::alpha*simStateOverheadDelay;
					LOG4CXX_DEBUG(logger, "avgSimulatorsOverheadDelay = " << avgSimStateOverheadDelay);

					tdfSim = tdfSim + Configuration::tdfSimGain * ((long double) avgSimStateOverheadDelay - (long double) Configuration::targetDelay) / (long double) Configuration::targetDelay;
					LOG4CXX_DEBUG(logger, "tdfSim = " << tdfSim);

					// Set a boundary
					if (tdfSim < Configuration::tdfMin)
						tdfSim = Configuration::tdfMin;
					else if (tdfSim > Configuration::tdfMax)
						tdfSim = Configuration::tdfMax;

				} else{
					tdfSim = Configuration::tdfMin;
				}

				//tdfSim = memDb_.sharedMemory.getTdfSim();
			}

			long double rTdf;

			switch(Configuration::monitorMode){
			case 0:
				rTdf = tdfLoad;
				break;
			case 1:
				rTdf = tdfSim;
				break;
			case 2:
				if (tdfSim > tdfLoad)
					rTdf = tdfSim;
				else
					rTdf = tdfLoad;
				break;
			default:
				rTdf = Configuration::tdfMin;
			}

			// Send UDP packet of required TDF
			std::string msg;
			msg.append("0010|");	// Protocol-10
			msg.append(hostname);
			msg.append("|");
			std::ostringstream ss;
			ss << rTdf;
			msg.append(ss.str());
			LOG4CXX_DEBUG(logger, "UDP Msg: " << msg);
			udpClient_.sendTo(msg); // ex) message "0010|hightemplar|3.6"
		}

		// Sleep
		int tdfTableSize = memDb_.tdfTable.getSize();
		unsigned int sleepUs = Configuration::packetHandlePeriod * tdfTableSize;
		if(usleep(sleepUs) < 0)
			LOG4CXX_ERROR(logger, "usleep error");
		//pselect(0, NULL, NULL, NULL, &timeSpec, NULL);
	}
}

double CpuMonitor::getCpuIdleRatio() {
	LOG4CXX_TRACE(logger, "CpuMonitor::getCpuIdleRatio()");

	double total, idle, ret;
	glibtop_cpu cpu;
	glibtop_get_cpu(&cpu);
	total = ((unsigned long) cpu.total) ? ((double) cpu.total) : 1.0;
	idle = ((unsigned long) cpu.idle) ? ((double) cpu.idle) : 1.0;
	ret = (idle - idle_) / (total - total_);
	total_ = total;
	idle_ = idle;

	if (isnan(ret))
		return idle_ / total_;
	else
		return ret;
}

double CpuMonitor::getMinXCpuIdleRatio() {
	LOG4CXX_TRACE(logger, "CpuMonitor::getMinXCpuIdleRatio()");

	vector<double> xcpu_total, xcpu_idle;
	vector<double> diff_xcpu_total, diff_xcpu_idle;

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

double CpuMonitor::getAvgMinXCpuIdleRatio() {
	// Get minimum XCpuIdle ratio
	curMinXCpuIdleRatio = getMinXCpuIdleRatio();
	LOG4CXX_DEBUG(logger, "currCpuIdleRatio = " << curMinXCpuIdleRatio);

	// Get exponential weighted moving average
	//alpha = 0.125; // 0.125, 0.0625, 0.0312, 0.0156
	avgMinXCpuIdleRatio = (1.0-Configuration::alpha)*avgMinXCpuIdleRatio+Configuration::alpha*curMinXCpuIdleRatio;

	LOG4CXX_DEBUG(logger, "avgMinXCpuIdleRatio = " << avgMinXCpuIdleRatio);

	return avgMinXCpuIdleRatio;
}

double CpuMonitor::sign(double t){
    if( t == 0 )
        return double(0);
    else
        return (t < 0)? double(-1) : double(1);
}


// obsolete code

//		if (currCpuIdle < 0.2 || currCpuIdle > 0.8) {
//			diffCpuIdle = prevCpuIdle - currCpuIdle;
//			if (currCpuIdle < 0.2 && fabs(diffCpuIdle) <= 0.1)
//				diffCpuIdle = 0.1;
//			else if (currCpuIdle > 0.8 && fabs(diffCpuIdle) <= 0.1)
//				diffCpuIdle = -0.1;

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

#include "VirtualTime.hpp"

log4cxx::LoggerPtr VirtualTime::logger(
		log4cxx::Logger::getLogger("VirtualTime"));

VirtualTime::VirtualTime(MemoryDatabase& memDb) :
		memDb_(memDb) {
	LOG4CXX_TRACE(logger,
			"Entering VirtualTime::VirtualTime(MemoryDatabase& memDb)");

//		time_sleep.tv_sec = 0;
//		time_sleep.tv_nsec = *time_sleep_shmin;
//		time_slice.tv_sec = 0;
//		time_slice.tv_nsec = *time_slice_shmin;
//		sem_timeout.tv_sec = 0;
//		sem_timeout.tv_nsec = SEMWAIT_DEFAULT;
}

void VirtualTime::operator ()() {
	LOG4CXX_TRACE(logger, "VirtualTime::operator ()()");

	start();
}

void VirtualTime::start() {
	LOG4CXX_TRACE(logger, "Entering VirtualTime::start()");

	// Initialize clocks
	init_clocks();

	// Enable virtual time
	cpu_enable_ticks();
	semaphore_setval(semid_gostop, VIRHOST, SEMGO);

	// Start adaptive virtual time
	startAdaptiveVirtualTime();
}

/* main loop 7: adaptive TDF */
void VirtualTime::startAdaptiveVirtualTime(void) {
	LOG4CXX_TRACE(logger, "Entering mainloop_adaptivetdf()");

	//CpuLogger cpuLogger;

	InterfaceConfigurator ifaceConfig(memDb_);

	// Count looping
	static int mainloop_cnt = 0;

	while (SignalHandler::isRunning) {
		LOG4CXX_TRACE(logger, "while (SignalHandler::isRunning)");

		// Code for testing
		//__testIfVirtualTimeProceeds();

		// Wait
		boost::unique_lock<boost::mutex> lock(memDb_.tdfTable.mutex);
		memDb_.tdfTable.cond.wait(memDb_.tdfTable.mutex);

		// Suspend VHs
		cpu_disable_ticks();
		if(Configuration::isVMSuspensionsEnabled)
			semaphore_setval(semid_gostop, VIRHOST, SEMSTOP);

		// Log new tdf
		long double newTdf = memDb_.tdfTable.getLatestTdf();
		LOG4CXX_DEBUG(logger,
				"[" << mainloop_cnt++ << "] SM proceeds: tdf = " << newTdf);

		// Set new tdf to shared memory
		memDb_.sharedMemory.setTdf(newTdf);

		// Update interface attributes
		ifaceConfig.updateInterfaceAttribute();

		// log for a graph regarding TDF, CPU Utilization, and time
		//double cpuIdleRatio = cpuLogger.getMinXCpuIdleRatio();
		LOG4CXX_INFO(logger,  TimeGetter::getTimeUs() << "	"
				<< newTdf << "	"
				<< 1.0 - CpuMonitor::curMinXCpuIdleRatio);	// or avgMinXCpuIdleRatio

		// Resume VHs
		cpu_enable_ticks();
		if(Configuration::isVMSuspensionsEnabled)
			semaphore_setval(semid_gostop, VIRHOST, SEMGO);
	}
}

// Test if virtual time proceeds
void VirtualTime::__testIfVirtualTimeProceeds() {
	for(;;) {
		std::cout << "cpu_get_clock() =  " << cpu_get_clock() << std::endl;
		sleep(1);
	}
}


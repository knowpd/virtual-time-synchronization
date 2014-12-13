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

#include "SharedMemory.hpp"

log4cxx::LoggerPtr SharedMemory::logger(log4cxx::Logger::getLogger("SharedMemory"));

SharedMemory::SharedMemory() {
	LOG4CXX_TRACE(logger, "Entering SharedMemory::SharedMemory()");
}

SharedMemory::~SharedMemory() {
	LOG4CXX_TRACE(logger, "Entering SharedMemory::~SharedMemory()");

	// Remove timers_state: Remove shared memory and semaphore
	timers_state_remove();
}

void SharedMemory::create() {
	LOG4CXX_TRACE(logger, "Entering SharedMemory::create()");

//	TimersStateKey tskey =
//			{ SHMKEY_TIMERS_STATE, SHMKEY_TDF, SEMKEY_READERSWRITERS,
//					SHMKEY_READCOUNT, SHMKEY_WRITECOUNT, SEMKEY_GOSTOP,
//					SHMKEY_FLAG_BASE, SHMKEY_FLAG_TRANSIT, SEMKEY_BINARY };

	TimersStateKey tskey = { SHMKEY_TIMERS_STATE, SHMKEY_TDF, SHMKEY_SIMULATORS_STATE, SEMKEY_GOSTOP };
	timers_state_create(tskey);
	*tdf = 1.0; // Initialization
	tdfPtr_ = tdf;

	// For TDF_sim
	simulators_state->overhead_delay = 0; // Initialization
	simulators_state->time = 0; // Initializaiton
	simulatorsStatePtr_ = simulators_state;

}

long double SharedMemory::getTdf() {
	LOG4CXX_TRACE(logger, "Entering SharedMemory::getTdf()");

	return *tdfPtr_;
}

void SharedMemory::setTdf(long double tdfVal) {
	LOG4CXX_TRACE(logger, "Entering SharedMemory::setTdf");

	*tdfPtr_ = tdfVal;
}

//long double SharedMemory::getTdfSim() {
//	LOG4CXX_TRACE(logger, "Entering SharedMemory::getTdfSim()");
//
//	uint64_t tdfSimTime = simulatorsStatePtr_->time;
//	uint64_t currentTime = TimeGetter::getTimeUs();
//	uint64_t timeDiff;
//
//	if (tdfSimTime > currentTime)
//		timeDiff = tdfSimTime - currentTime;
//	else
//		timeDiff = currentTime -tdfSimTime;
//
//	if ( timeDiff > 10000000)
//		return Configuration::tdfMin;
//	else{
//		// TDF_sim
//		long double tdfSim = (long double) simulatorsStatePtr_->overhead_delay / (long double) Configuration::targetDelay;
//		return tdfSim;
//	}
//}

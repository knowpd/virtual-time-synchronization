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

#ifndef SHAREDMEMORY_HPP_
#define SHAREDMEMORY_HPP_

#include <sys/shm.h>
#include <log4cxx/logger.h>
#include "shared/qemu-timer.h"
#include "TimeGetter.hpp"
#include "Configuration.hpp"

//#define SHMKEY_TDF1_IN (720)	// shared memory key for time_ratio_shmin
//#define SHMKEY_TDF2_IN (719)	// shared memory key for time_ratio2_shmin
//#define SHMKEY_TIME_SLEEP_IN (721)	// shared memory key for time_sleep_shmin
//#define SHMKEY_TIME_SLICE_IN (722)	// shared memory key for time_slice_shmin
//#define SHMKEY_USE_TIME_SLICE_IN (723) // shared memory key for use_time_slice_shmin
//#define SHMKEY_USE_QEMUPKT_FEED_IN (724) // shared memory key for use_qemupkt_feed_shmin

#define SHMKEY_TIMERS_STATE (528)	// shared memory key for timers_state
#define SHMKEY_TDF (529)			// shared memory key for TDF
#define SHMKEY_SIMULATORS_STATE (527)		// shared memory key for simulators_state
#define SEMKEY_GOSTOP (331)			// semaphore key for resume/suspend of SyncMgr
//#define SHMKEY_READCOUNT (530)		// semaphore key for readcount
//#define SHMKEY_WRITECOUNT (531)		// semaphore key for writecount
//#define SEMKEY_READERSWRITERS (330)	// semaphore key for readers/writers
//#define SHMKEY_FLAG_BASE (332)		// shared memory key of flag for base TDF
//#define SHMKEY_FLAG_TRANSIT (333)	// shared memory key of flag for base TDF
//#define SEMKEY_BINARY (334)			// binary semaphore key

extern long double *tdf;
extern SimulatorsState *simulators_state;

class SharedMemory {
public:
	SharedMemory();
	~SharedMemory();
	void create();
	long double getTdf();
	long double getTdfSim();
	void setTdf(long double tdfVal);

	SimulatorsState* simulatorsStatePtr_;

private:
	static log4cxx::LoggerPtr logger;
	TimersState* timers_state;
	long double* tdfPtr_;
};

#endif /* SHAREDMEMORY_HPP_ */

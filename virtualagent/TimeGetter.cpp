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

#include "TimeGetter.hpp"

uint64_t TimeGetter::getTimeUs(void) {
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		cerr << "gettimeofday() error" << endl;

	return convertTimevalToUs(&tv);
}

uint64_t TimeGetter::getTimeNs(void) {
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		cerr << "gettimeofday() error" << endl;

	return convertTimevalToNs(&tv);
}

uint64_t TimeGetter::getClockRealTime(void) {
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) < 0)
		cerr << "clock_gettime() error" << endl;

	return convertTimespecToNs(&ts);
}

uint64_t TimeGetter::getClockMonotonic(void) {
	struct timespec ts;

	// CLOCK_MONOTONIC: Clock  that  cannot  be  set and represents
	//  monotonic time since some unspecified starting point.
	if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
		cerr << "clock_gettime() error" << endl;

	return convertTimespecToNs(&ts);
}

uint64_t TimeGetter::convertTimevalToUs(struct timeval *tv) {
	uint64_t usResult = tv->tv_sec * US_PER_SEC + tv->tv_usec;
	return usResult;
}

uint64_t TimeGetter::convertTimevalToNs(struct timeval *tv) {
	uint64_t nsResult = tv->tv_sec * NS_PER_SEC + tv->tv_usec * US_PER_NS;
	return nsResult;
}

uint64_t TimeGetter::convertTimespecToNs(struct timespec *ts) {
	uint64_t nsResult = ts->tv_sec * NS_PER_SEC + ts->tv_nsec;
	return nsResult;
}

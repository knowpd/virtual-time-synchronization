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


#ifndef TIMEGETTER_HPP_
#define TIMEGETTER_HPP_

#include <iostream>
#include <sys/time.h>
#include <stdint.h>

static const uint64_t US_PER_NS = (uint64_t) 1000;
static const uint64_t US_PER_SEC = (uint64_t) 1000000;
static const uint64_t NS_PER_SEC = (uint64_t) 1000000000;

using namespace std;

class TimeGetter{
public:
	static uint64_t getTimeUs(void);
	static uint64_t getTimeNs(void);
	static uint64_t getClockRealTime(void);
	static uint64_t getClockMonotonic(void);

private:
	static uint64_t convertTimevalToUs(struct timeval *tv);
	static uint64_t convertTimevalToNs(struct timeval *tv);
	static uint64_t convertTimespecToNs(struct timespec *ts);
};

#endif /* TIMEGETTER_HPP_ */

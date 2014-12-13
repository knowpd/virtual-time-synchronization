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

#ifndef QEMU_TIMER_H
#define QEMU_TIMER_H


#include <stdint.h>

#define USE_VM_GETTIMEOFDAY 1	// Used in ns-3
#define SEMGO 0
#define SEMSTOP 1

/* Sempahore: GOSTOP*/
#define NUMOFSEM_GOSTOP 4
#define VIRHOST 0

// The following structure has been moved from qemu-time.c, in order to include qemu-time.h in vl.c
// TimersState is defined in vl.c such as 'TimersState *timers_state'.
// TimersState is assigned as a shared memory in syncmgr.c
typedef struct TimersState {
	int64_t cpu_ticks_prev;
	int64_t cpu_ticks_offset;
	int64_t cpu_ticks_restart;	// by hlee
	int64_t cpu_ticks_log;		// by hlee
	int64_t cpu_clock_offset;
	int64_t cpu_clock_restart;	// by hlee
	int64_t cpu_clock_log;		// by hlee
	int32_t cpu_ticks_enabled;
	int64_t dummy;
} TimersState;

typedef struct SimulatorsState{
	uint64_t time;
	uint64_t overhead_delay;
} SimulatorsState;

/* TimersStateKey by hlee */
typedef struct TimersStateKey{
	key_t shmkey_timers_state;		// a key of shared memory
	key_t shmkey_tdf;				// a key of shared memory
	key_t shmkey_simulators_state;			// a key of shared memory
	key_t semkey_gostop;			// a key of semaphore for resume/suspend
} TimersStateKey;

//extern TimersState *timers_state;
extern long double *tdf;
extern int semid_gostop;




/* timers */
typedef struct QEMUClock QEMUClock;
typedef void QEMUTimerCB(void *opaque);

/* The real time clock should be used only for stuff which does not
 change the virtual machine state, as it is run even if the virtual
 machine is stopped. The real time clock has a frequency of 1000
 Hz. */
extern QEMUClock *rt_clock;

/* The virtual clock is only run during the emulation. It is stopped
 when the virtual machine is stopped. Virtual timers use a high
 precision clock, usually cpu cycles (use ticks_per_sec). */
extern QEMUClock *vm_clock;

/* The host clock should be use for device models that emulate accurate
 real time sources. It will continue to run when the virtual machine
 is suspended, and it will reflect system time changes the host may
 undergo (e.g. due to NTP). The host clock has the same precision as
 the virtual clock. */
extern QEMUClock *host_clock;

struct QEMUClock {
	int type;
	int enabled;
/* XXX: add frequency */
};

int64_t qemu_get_clock(QEMUClock *clock);
int64_t qemu_get_clock_ns(QEMUClock *clock);
int64_t cpu_get_clock(void);
int64_t cpu_get_ticks(void);
void cpu_enable_ticks(void);
void cpu_disable_ticks(void);
void init_clocks(void);
int vm_gettimeofday(struct timeval *tv, struct timezone *tz);	// by hlee
void timers_state_create(TimersStateKey tskey);					// by hlee
void timers_state_attach(TimersStateKey tskey);					// by hlee
void timers_state_detach(void);									// by hlee
void timers_state_remove(void);									// by hlee
void tdf_set(long double tr);							// by hlee


#if defined(__i386__)

static inline int64_t cpu_get_real_ticks(void)
{
    int64_t val;
    asm volatile ("rdtsc" : "=A" (val));
    return val;
}

#elif defined(__x86_64__)

static inline int64_t cpu_get_real_ticks(void)
{
    uint32_t low,high;
    int64_t val;
    asm volatile("rdtsc" : "=a" (low), "=d" (high));
    val = high;
    val <<= 32;
    val |= low;
    //printf("[qemu-timer.h] cpu_get_real_ticks(): val = %llx\n", (long long unsigned)val);	// by hlee
    return val;
}

#endif

#endif /* QEMU_TIMER_H */

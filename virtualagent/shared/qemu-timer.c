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

// Reference: QEMU System Emulator by Fabrice Bellard

/*
 #include "sysemu.h"
 #include "net.h"
 #include "monitor.h"
 #include "console.h"

 #include "hw/hw.h"
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <signal.h>
#ifdef __FreeBSD__
#include <sys/param.h>
#endif

#ifdef __linux__
#include <sys/ioctl.h>
#include <linux/rtc.h>
/* For the benefit of older linux systems which don't supply it,
 we use a local copy of hpet.h. */
/* #include <linux/hpet.h> */
//#include "hpet.h"
#endif

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#endif

#include <sys/time.h>
#include <sys/sem.h>	// by hlee
#include <sys/shm.h>	// by hlee
#include <sys/types.h>	// by hlee
#include "qemu-timer.h"	// by hlee
#include "bin_sem.h"	// by hlee

#define QEMU_CLOCK_REALTIME 0
#define QEMU_CLOCK_VIRTUAL  1
#define QEMU_CLOCK_HOST     2

QEMUClock *rt_clock;
QEMUClock *vm_clock;
QEMUClock *host_clock;

TimersState *timers_state;	// by hlee
long double *tdf;			// by hlee
SimulatorsState *simulators_state; // by hlee
int semid_gostop;			// by hlee
int shmid_timers_state;		// by hlee
int shmid_tdf;				// by hlee;
int shmid_simulators_state;	// by hlee

void __ipc_create(TimersStateKey tskey);
void __ipc_attach(TimersStateKey tskey);
void __ipc_detach(void);
void __ipc_remove(void);
int64_t __cpu_get_ticks(void);
int64_t __cpu_get_clock(void);

static int64_t get_clock_realtime(void) {
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000000LL + (tv.tv_usec * 1000);
}

static int use_rt_clock;

static void init_get_clock(void) {
	use_rt_clock = 0;
#if defined(__linux__) || (defined(__FreeBSD__) && __FreeBSD_version >= 500000) \
    || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
	{
		struct timespec ts;
		if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
			use_rt_clock = 1;
		}
	}
#endif
}

static int64_t get_clock(void) {
#if defined(__linux__) || (defined(__FreeBSD__) && __FreeBSD_version >= 500000) \
	|| defined(__DragonFly__) || defined(__FreeBSD_kernel__)
	if (use_rt_clock) {
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);		// commented by hlee
		//clock_gettime(CLOCK_REALTIME, &ts); // by hlee
		return ts.tv_sec * 1000000000LL + ts.tv_nsec;
	} else
#endif
	{
		/* XXX: using gettimeofday leads to problems if the date
		 changes, so it should be avoided. */
		return get_clock_realtime();
	}
}

/*************************** Beginning hlee's *******************************/
/* Create semaphore and shared memory - by hlee */
void __ipc_create(TimersStateKey tskey) {

	/* Create shared memory for timers_state */
	if ((shmid_timers_state = shmget(tskey.shmkey_timers_state,
			sizeof(*timers_state), IPC_CREAT | 0666)) < 0) {
		perror("shmget-timer_state");
		exit(2);
	}
	if ((timers_state = (TimersState *) shmat(shmid_timers_state, NULL, 0))
			== (void *) -1) {
		perror("shmat-timers_state");
		exit(2);
	}

	/* Create shared memory for TDF */
	if ((shmid_tdf = shmget(tskey.shmkey_tdf, sizeof(*tdf),
			IPC_CREAT | 0666)) < 0) {
		perror("shmget-tdf");
		exit(2);
	}
	if ((tdf = (long double *) shmat(shmid_tdf, NULL, 0))
			== (void *) -1) {
		perror("shmat-tdf");
		exit(2);
	}

	/* Create shared memory for TDF_sim */
	if ((shmid_simulators_state = shmget(tskey.shmkey_simulators_state,
			sizeof(*simulators_state), IPC_CREAT | 0666)) < 0) {
		perror("shmget-tdf_simulators_state");
		exit(2);
	}
	if ((simulators_state = (SimulatorsState *) shmat(shmid_simulators_state, NULL, 0))
			== (void *) -1) {
		perror("shmat-tdf_simulator_state");
		exit(2);
	}

	/* Create and initialize a semaphore of vmgostop */
	semid_gostop = semaphore_create(tskey.semkey_gostop, NUMOFSEM_GOSTOP);
}

/* Attach semaphore and shared memory*/
void __ipc_attach(TimersStateKey tskey) {

	/* Attach shared memory for timers_state */
	if ((shmid_timers_state = shmget(tskey.shmkey_timers_state,
			sizeof(*timers_state), 0666)) < 0) {
		perror("shmget-timer_state");
		exit(2);
	}
	if ((timers_state = (TimersState *) shmat(shmid_timers_state, NULL, 0))
			== (void *) -1) {
		perror("shmat-timers_state");
		exit(2);
	}

	/* Attach shared memory for TDF */
	if ((shmid_tdf = shmget(tskey.shmkey_tdf, sizeof(*tdf),
			0666)) < 0) {
		perror("shmget-tdf");
		exit(2);
	}
	if ((tdf = (long double *) shmat(shmid_tdf, NULL, 0))
			== (void *) -1) {
		perror("shmat-tdf");
		exit(2);
	}

	/* Attach shared memory for TDF_sim */
	if ((shmid_simulators_state = shmget(tskey.shmkey_simulators_state, sizeof(*simulators_state),
			0666)) < 0) {
		perror("shmget-tdf_sim");
		exit(2);
	}
	if ((simulators_state = (SimulatorsState *) shmat(shmid_simulators_state, NULL, 0))
			== (void *) -1) {
		perror("shmat-tdf_sim");
		exit(2);
	}

	/* Attach a semaphore of vmgostop */
	semid_gostop = semaphore_attach(tskey.semkey_gostop, NUMOFSEM_GOSTOP);

}

/* Detach semaphore and shared memory - hlee */
void __ipc_detach(void) {
	/* Detach shared memory */
	shmdt(timers_state);
	shmdt(tdf);
	shmdt(simulators_state);
}

/* Detach and remove semaphore and shared memory -  hlee */
void __ipc_remove(void) {
	/* Detach and remove shared memory */
	shmdt(timers_state);
	shmctl(shmid_timers_state, IPC_RMID, 0);
	shmdt(tdf);
	shmdt(simulators_state);
	shmctl(shmid_tdf, IPC_RMID, 0);
	semaphore_remove(semid_gostop);
}

/**
 * vm_gettimeofday replacing gettimeofday() in inetmanet
 * Function Author: hlee
 */
int vm_gettimeofday(struct timeval *tv, struct timezone *tz) {
	int64_t vm_time;

	vm_time = cpu_get_clock(); // unit: nsec
	//printf("vm_time = %lld\n", (long long)vm_time);

	tv->tv_sec = vm_time / 1000000000LL;
	tv->tv_usec = ((vm_time - tv->tv_sec * 1000000000LL) / 1000LL);
	return 0;
}

/* return the host CPU cycle counter and handle stop/restart */
int64_t cpu_get_ticks(void) {
	int64_t retval; // by hlee

	for(;;){
		if (!timers_state->cpu_ticks_enabled) {
			retval = timers_state->cpu_ticks_offset;
			if(!timers_state->cpu_ticks_enabled)
				break;
		}

		if(timers_state->cpu_ticks_enabled){
			int64_t ticks;
			ticks = cpu_get_real_ticks();
			if (timers_state->cpu_ticks_prev > ticks) {
				/* Note: non increasing ticks may happen if the host uses
				 software suspend */
				//timers_state->cpu_ticks_offset += timers_state->cpu_ticks_prev - ticks; // original
				timers_state->cpu_ticks_offset += (timers_state->cpu_ticks_prev - ticks) / *tdf;
			}
			timers_state->cpu_ticks_prev = ticks;
			//retval = ticks + timers_state->cpu_ticks_offset; // original
			retval = timers_state->cpu_ticks_restart + (ticks
					- timers_state->cpu_ticks_restart) / *tdf
					+ timers_state->cpu_ticks_offset; // by hlee
			if(timers_state->cpu_ticks_enabled)
				break;
		}
	}

	return retval;
}

/* return the host CPU monotonic timer and handle stop/restart */
int64_t cpu_get_clock(void) {
	int64_t ti;
	int64_t retval; // by hlee

	for(;;){
		if (!timers_state->cpu_ticks_enabled) {
			retval = timers_state->cpu_clock_offset;
			if(!timers_state->cpu_ticks_enabled)
				break;
		}

		if(timers_state->cpu_ticks_enabled){
			ti = get_clock();
			//retval = ti + timers_state->cpu_clock_offset;
			retval = timers_state->cpu_clock_restart
					+ (ti - timers_state->cpu_clock_restart) / *tdf
					+ timers_state->cpu_clock_offset; // by hlee
			if(timers_state->cpu_ticks_enabled)
				break;
		}
	}

	return retval;
}

/* Author: hlee
 *   Called by only cpu_disable_ticks()
 *   This is a copy of cpu_get_ticks().
 */
int64_t __cpu_get_ticks(void) {
	int64_t retval; // by hlee
	if (!timers_state->cpu_ticks_enabled) {
		retval = timers_state->cpu_ticks_offset;
	} else {
		int64_t ticks;
		ticks = cpu_get_real_ticks();
		if (timers_state->cpu_ticks_prev > ticks) {
			/* Note: non increasing ticks may happen if the host uses
			 software suspend */
			//timers_state->cpu_ticks_offset += timers_state->cpu_ticks_prev - ticks; // original
			timers_state->cpu_ticks_offset += (timers_state->cpu_ticks_prev - ticks) / *tdf;
		}
		timers_state->cpu_ticks_prev = ticks;
		//retval = ticks + timers_state->cpu_ticks_offset;
		retval = timers_state->cpu_ticks_restart + (ticks
				- timers_state->cpu_ticks_restart) / *tdf
				+ timers_state->cpu_ticks_offset; // by hlee
	}
	return retval;
}

/* Author: hlee
 *   Called by only cpu_disable_ticks()
 *   This is a copy of cpu_get_clock().
 */
int64_t __cpu_get_clock(void) {
	int64_t ti;
	int64_t retval; // by hlee

	if (!timers_state->cpu_ticks_enabled) {
		retval = timers_state->cpu_clock_offset;
	} else {
		ti = get_clock();
		//retval = ti + timers_state->cpu_clock_offset;
		retval = timers_state->cpu_clock_restart
				+ (ti - timers_state->cpu_clock_restart) / *tdf
				+ timers_state->cpu_clock_offset; // by hlee
	}
	return retval;
}

/* enable cpu_get_ticks() */
void cpu_enable_ticks(void) {
	if (!timers_state->cpu_ticks_enabled) {
		int64_t real_clock = get_clock();
		int64_t real_ticks = cpu_get_real_ticks();
		timers_state->cpu_ticks_offset -= real_ticks;
		timers_state->cpu_ticks_restart = real_ticks;
		timers_state->cpu_clock_offset -= real_clock;
		timers_state->cpu_clock_restart = real_clock;
		timers_state->cpu_ticks_enabled = 1;
	}
}

/* disable cpu_get_ticks() : the clock is stopped. You must not call
 cpu_get_ticks() after that.  */
void cpu_disable_ticks(void) {
	if (timers_state->cpu_ticks_enabled) {
		timers_state->cpu_ticks_offset = __cpu_get_ticks();
		timers_state->cpu_clock_offset = __cpu_get_clock();
		timers_state->cpu_ticks_enabled = 0;
	}
}

/* Initialize timers_state - hlee*/
void timers_state_create(TimersStateKey tskey) {
	__ipc_create(tskey); // by hlee

	int64_t curr_ticks = cpu_get_real_ticks();
	int64_t curr_clock = get_clock();
	timers_state->cpu_ticks_prev = curr_ticks;
	timers_state->cpu_ticks_offset = curr_ticks;
	timers_state->cpu_ticks_log = curr_ticks;
	timers_state->cpu_clock_offset = curr_clock;
	timers_state->cpu_clock_restart = curr_clock;
	timers_state->cpu_clock_log = curr_clock;
	timers_state->cpu_ticks_enabled = 0; // 0: disabled, 1: enabled
	timers_state->dummy = 0;
}

void timers_state_attach(TimersStateKey tskey) {
	__ipc_attach(tskey);
}

void timers_state_detach(void) {
	__ipc_detach();
}

void timers_state_remove(void) {
	__ipc_remove();
}

void tdf_set(long double tr) {
	(*tdf) = tr;
}
/*************************** Ending hlee's **********************************/

//static QEMUClock *qemu_new_clock(int type) {
//	QEMUClock *clock;
//	clock = qemu_mallocz(sizeof(QEMUClock));
//	clock->type = type;
//	clock->enabled = 1;
//	return clock;
//}
void init_clocks(void) {
	init_get_clock();

	rt_clock = (QEMUClock *) malloc(sizeof(rt_clock));
	vm_clock = (QEMUClock *) malloc(sizeof(vm_clock));
	host_clock = (QEMUClock *) malloc(sizeof(host_clock));

	rt_clock->type = QEMU_CLOCK_REALTIME;
	vm_clock->type = QEMU_CLOCK_VIRTUAL;
	host_clock->type = QEMU_CLOCK_HOST;

	rt_clock->enabled = 1;
	vm_clock->enabled = 1;
	host_clock->enabled = 1;

	//rt_clock = qemu_new_clock(QEMU_CLOCK_REALTIME);
	//vm_clock = qemu_new_clock(QEMU_CLOCK_VIRTUAL);
	//host_clock = qemu_new_clock(QEMU_CLOCK_HOST);
}

int64_t qemu_get_clock(QEMUClock *clock) {
	switch (clock->type) {
	case QEMU_CLOCK_REALTIME:
		return get_clock() / 1000000;
	default:
	case QEMU_CLOCK_VIRTUAL:
		return cpu_get_clock();
	case QEMU_CLOCK_HOST:
		return get_clock_realtime();
	}
}

int64_t qemu_get_clock_ns(QEMUClock *clock) {
	switch (clock->type) {
	case QEMU_CLOCK_REALTIME:
		return get_clock();
	default:
	case QEMU_CLOCK_VIRTUAL:
		return cpu_get_clock();
	case QEMU_CLOCK_HOST:
		return get_clock_realtime();
	}
}


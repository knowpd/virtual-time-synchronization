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

/**
 * Code listing from "Advanced Linux Programming," by CodeSourcery LLC
 * Copyright (C) 2001 by New Riders Publishing
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include "bin_sem.h"

/* Create specified number of semaphores */
int semaphore_create(key_t sem_key, int num_semaphores) {
	int semid;

	/* create new semaphore set of semaphores */
	if ((semid = semget(sem_key, num_semaphores, IPC_CREAT | 0666)) < 0) {
		perror("error in creating semaphore");
		exit(1);
	}
	return semid;
}

/* Attach specified number of semaphores - hlee*/
int semaphore_attach(key_t sem_key, int num_semaphores) {
	int semid;

	/* allocate new semaphore set of semaphores */
	if ((semid = semget(sem_key, num_semaphores, 0666)) < 0) {
		perror("error in attaching semaphore");
		exit(1);
	}
	return semid;
}

void semaphore_setval(int semid, int index, int value) { /* procedure to initialize specified semaphore to given value */
	if (semctl(semid, index, SETVAL, value) < 0) {
		perror("error in setting semaphore value");
		exit(1);
	}
}

void semaphore_remove(int semid){
	/* Remove the semaphore from the system and destroy the set of
	 semaphores and data structure associated with it. */
	if (semctl(semid, 0, IPC_RMID) < 0) {
		perror("error in removing semaphore from the system");
		exit(1);
	}
}

/* procedure to perform a P or wait operation on a semaphore of given index */
void semaphore_wait(int semid, int index) {
	struct sembuf sops[1]; /* only one semaphore operation to be executed */

	sops[0].sem_num = index;/* define operation on semaphore with given index */
	sops[0].sem_op = -1; /* subtract 1 to value for P operation */
	sops[0].sem_flg = 0; /* type "man semop" in shell window for details */

	if (semop(semid, sops, 1) == -1) {
		perror("error in semaphore operation(sem_op = -1)");
		exit(1);
	}
}

/* procedure to perform a V or signal operation on semaphore of given index */
void semaphore_post(int semid, int index) {
	struct sembuf sops[1]; /* define operation on semaphore with given index */

	sops[0].sem_num = index;/* define operation on semaphore with given index */
	sops[0].sem_op = 1; /* add 1 to value for V operation */
	sops[0].sem_flg = 0; /* type "man semop" in shell window for details */

	if (semop(semid, sops, 1) == -1) {
		perror("error in semaphore operation(sem_op = 1)");
		exit(1);
	}
}

/* procedure to perform a "wait-for-zero" operation on a semaphore of given index */
void semaphore_waitforzero(int semid, int index) {
	struct sembuf sops[1]; /* only one semaphore operation to be executed */

	sops[0].sem_num = index;/* define operation on semaphore with given index */
	sops[0].sem_op = 0; /* "wait-for-zero" operation */
	sops[0].sem_flg = 0; /* type "man semop" in shell window for details */

	//printf("[syncmgr][bin_sem.c] semaphore_waitforzero(): (-)semop()\n");
	if (semop(semid, sops, 1) == -1) {
		perror("error in semaphore operation(sem_op = 0)");
		exit(1);
	}
	//printf("[syncmgr][bin_sem.c] semaphore_waitforzero(): (+)semop()\n");
}

void semaphore_waitforzero_maskallsig(int semid, int index) {
	/* "wait-for-zero" operation */
	sigset_t newmask, oldmask;
	sigfillset(&newmask);
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		perror("SIG_BLOCK error");

	semaphore_waitforzero(semid, index);

	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		perror("SIG_SETMASK error");
}


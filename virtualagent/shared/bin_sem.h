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


#ifndef BIN_SEM_H_
#define BIN_SEM_H_
#include <sys/types.h>

/* We must define union semun ourselves. */
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};

#define SEMGO 0
#define SEMSTOP 1
#define VIRHOST 0

int semaphore_create(key_t sem_key, int num_semaphores);
int semaphore_attach(key_t sem_key, int num_semaphores);
void semaphore_setval(int semid, int index, int value);
void semaphore_remove(int semid);
void semaphore_wait(int semid, int index);
void semaphore_post(int semid, int index);
void semaphore_waitforzero(int semid, int index);
void semaphore_waitforzero_maskallsig(int semid, int index);

#endif /* BIN_SEM_H_ */

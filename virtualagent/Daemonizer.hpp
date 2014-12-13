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

#ifndef DAEMONIZER_HPP_
#define DAEMONIZER_HPP_

#include <cstdlib>
#include <signal.h>
#include <sys/stat.h>
#include <log4cxx/logger.h>

class Daemonizer {
public:
	Daemonizer();
	void daemonize();

private:
	static log4cxx::LoggerPtr logger;
};

#endif /* DAEMONIZER_HPP_ */

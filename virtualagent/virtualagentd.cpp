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

#include <iostream>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "Daemonizer.hpp"
#include "VirtualAgent.hpp"


log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("Main"));

int main(int argc, char *argv[]) {

	LOG4CXX_TRACE(logger, "Beginning of Program");
	int result = EXIT_SUCCESS;

	// Change myself into a daemon process
	Daemonizer daemon;

	try {
		// Set up log4cxx properties
		std::string log4cxxProperties = "log4cxx.properties";
		log4cxx::PropertyConfigurator::configure(log4cxxProperties);

		// Start VirtualAgent
		VirtualAgent virtualAgent;

	} catch (log4cxx::helpers::Exception& e) {
		std::cerr << "log4cxx Exception: " << e.what() << "\n";
		result = EXIT_FAILURE;
	}

	LOG4CXX_TRACE(logger, "End of Program");
	exit(result);
}

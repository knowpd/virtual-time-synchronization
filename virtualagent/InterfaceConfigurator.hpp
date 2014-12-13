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


#ifndef INTERFACECONFIGURATOR_HPP_
#define INTERFACECONFIGURATOR_HPP_

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp> // for boost::is_any_of()
#include <boost/algorithm/string.hpp>
#include <sys/wait.h>
#include <map>
#include <vector>
#include "MemoryDatabase.hpp"

using namespace std;

class InterfaceConfigurator {
public:
	InterfaceConfigurator(MemoryDatabase& memDb);
	static void replaceInterfaceAttribute(string iface, long long rate, long long delay,
			long double tdf);
	static void changeInterfaceAttribute(string iface, long long rate, long long delay,
			long double tdf);
	static void deleteInterfaceAttribute(string ifaceName);
	void updateInterfaceAttribute();
	static int system4Background(string cmdString);

private:
	static log4cxx::LoggerPtr logger;
	MemoryDatabase& memDb_;
};

#endif /* INTERFACECONFIGURATOR_HPP_ */

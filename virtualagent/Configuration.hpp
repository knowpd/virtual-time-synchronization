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

#ifndef CONFIGURATION_HPP_
#define CONFIGURATION_HPP_
#include <stdint.h>
#include <string>

class Configuration{
public:
	static std::string broadcastIp;
	static std::string tcpPort;
	static std::string udpPort;

	static double changeSlope;
	static double nonlinearFactor;
	static double alpha; // For exponential weighted moving average
	static double targetLoad;
	static unsigned int packetHandlePeriod;
	static long double tdfMin;
	static long double tdfMax;
	static long double tdfChangeThreshold;
	static int isVMSuspensionsEnabled;
	static int isLoadMonitored;
	static int isIfaceTdfUpdatedToOne;
	static int monitorMode;
	static uint64_t targetDelay;	// For TDF_sim
	static long double tdfSimGain;
private:

};

#endif /* CONFIGURATION_HPP_ */

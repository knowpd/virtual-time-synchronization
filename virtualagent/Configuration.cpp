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

#include "Configuration.hpp"

std::string Configuration::broadcastIp = "10.10.10.255"; // Change to your broadcast IP address
std::string Configuration::tcpPort = "9998";
std::string Configuration::udpPort = "9997";

double Configuration::changeSlope = 3.0;
double Configuration::nonlinearFactor = 4.0;
double Configuration::alpha = 0.125; // 0.125, 0.0625, 0.0312, 0.0156
double Configuration::targetLoad = 0.5; // 0 ~ 1(=100%)
unsigned int Configuration::packetHandlePeriod = 10000; // us
long double Configuration::tdfMin = 1.0;
long double Configuration::tdfMax = 20.0;
long double Configuration::tdfChangeThreshold = 0.1; // 0.1 = 10% change
int Configuration::isVMSuspensionsEnabled = 1; // 1: True, 0: False
int Configuration::isLoadMonitored = 1; // 1: True, 0: False
int Configuration::monitorMode = 0; // 0: TDF_load, 1: TDF_sim, 2: TDF_load + TDF_sim
uint64_t Configuration::targetDelay = 200000; // nanosecond
long double Configuration::tdfSimGain = 0.05;



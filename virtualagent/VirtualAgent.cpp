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

#include "VirtualAgent.hpp"

log4cxx::LoggerPtr VirtualAgent::logger(
		log4cxx::Logger::getLogger("VirtualAgent"));

VirtualAgent::VirtualAgent() {
	LOG4CXX_TRACE(logger, "Entering VirtualAgent::VirtualAgent()");
	tcpPort_ = "13210";
	udpPort_ = "13211";
	broadcastIp_ = Configuration::broadcastIp;
	start();
}

void VirtualAgent::start() {
	LOG4CXX_TRACE(logger, "Entering VirtualAgent::start()");

	// Set up signal handlers
	SignalHandler signalHandler;

	// Set up memory database
	MemoryDatabase memDb;

	// Created shared memroy
	memDb.createSharedMemory();

	try {
		// Set up TCP server
		LOG4CXX_DEBUG(logger, "Setting up TCP server");
		boost::asio::io_service ioService;
		TcpAsyncServer tcpAsyncServer(ioService, tcpPort_, memDb);

		// Set up UDP server
		LOG4CXX_DEBUG(logger, "Setting up UDP server");
		UdpAsyncServer udpAsyncServer(ioService, udpPort_, memDb);

		// Start TCP/UDP servers
		LOG4CXX_DEBUG(logger, "Staring TCP/UDP servers");
		boost::thread threadIoService(
				boost::bind(&boost::asio::io_service::run, &ioService));

		// Set up UDP cleint
		LOG4CXX_DEBUG(logger, "Setting up UDP client");
		UdpSyncClient udpClient(ioService, broadcastIp_, udpPort_);

		//		for (;;) {
		//			std::cout << "Enter message: ";
		//			char request[MAX_LENGTH]; //="0002|hightemplar|3.6";
		//			std::cin.getline(request, MAX_LENGTH);
		//			size_t requestLength = std::strlen(request);
		//			udpClient.sendTo(request, requestLength);
		//		}

		// Start virtual time
		LOG4CXX_DEBUG(logger, "Starting virtual time");
		VirtualTime virtualTime(memDb);
		boost::thread threadVirtualTime(virtualTime);

		// Monitor CPU load
		CpuMonitor cpuMonitor(udpClient, memDb);
		boost::thread threadCpuMonitor(cpuMonitor);

		// Wait for CpuMonitor thread termination
		threadCpuMonitor.join();

		// Notify to escape from virtual time loop
		memDb.tdfTable.cond.notify_all();

		// Wait for VirtualTime thread termination
		threadVirtualTime.join();

		// Stop async servers
		ioService.stop();
		threadIoService.join();

	} catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

}

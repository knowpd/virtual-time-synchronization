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

#include "SignalHandler.hpp"

log4cxx::LoggerPtr SignalHandler::logger(log4cxx::Logger::getLogger("SignalHandler"));
bool SignalHandler::isRunning = true;

SignalHandler::SignalHandler() {
	LOG4CXX_TRACE(logger, "Entering SignalHandler::SignalHandler()");

	setUpHandlers();
}

void SignalHandler::setUpHandlers() {
	LOG4CXX_TRACE(logger, "Entering SignalHandler::setUpHandlers()");

	signal(SIGCHLD, SIG_IGN); // Ignore child
	signal(SIGTSTP, SIG_IGN); // Ignore tty signals
	signal(SIGTTOU, SIG_IGN); // tty output for background process
	signal(SIGTTIN, SIG_IGN); // tty input for background process
	signal(SIGHUP, signalHandler); // Catch hangup signal
	signal(SIGTERM, signalHandler); // Catch kill signal
}

void SignalHandler::signalHandler(int sig) {
	LOG4CXX_TRACE(logger, "Entering SignalHandler::signalHandler(int sig)");

	switch (sig) {
	case SIGHUP:
		LOG4CXX_DEBUG(logger, "Hangup signal catched");
		break;
	case SIGTERM:
		LOG4CXX_DEBUG(logger, "Terminate signal catched");
		//exit(0);
		isRunning = false;
		break;
	}
}

//------------------------------------------------------------------------------
// Sample for setting up sigaction. Not tested yet.
void SignalHandler::setUpSigactions() {
	setUpSigActionForSIGHUP();
	// ...
}

void SignalHandler::setUpSigActionForSIGHUP() {
	sigset_t hupmask;
	sigemptyset(&hupmask);
	sigaddset(&hupmask, SIGHUP);

	memset(&act_hup, 0, sizeof(act_hup));
	act_hup.sa_sigaction = SIGHUPHandler;
	act_hup.sa_mask = hupmask;
	act_hup.sa_flags = SA_SIGINFO;
	sigaction(SIGHUP, &act_hup, NULL);
}

void SignalHandler::SIGHUPHandler(int signum, siginfo_t *info,
		void *ptr) {
	std::cout << "SIGHUP received" << std::endl;
	return;
}


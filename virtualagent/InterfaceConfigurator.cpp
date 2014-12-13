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

#include "InterfaceConfigurator.hpp"

log4cxx::LoggerPtr InterfaceConfigurator::logger(
		log4cxx::Logger::getLogger("InterfaceConfigurator"));

InterfaceConfigurator::InterfaceConfigurator(MemoryDatabase& memDb) :
		memDb_(memDb) {
	LOG4CXX_TRACE(logger,
			"Entering InterfaceConfigurator::InterfaceConfigurator(MemoryDatabase& memDb)");
}

void InterfaceConfigurator::replaceInterfaceAttribute(std::string iface,
		long long rate, long long delay, long double tdf) {
	LOG4CXX_TRACE(logger,
			"Entering InterfaceConfigurator::replaceInterfaceAttribute()");

	int status;
	long long adjustedRate = rate / tdf;
	long long adjustedDelay = delay * tdf;
	std::ostringstream cmdSS;

	cmdSS.str("");
	cmdSS << " tc qdisc replace dev " << iface;
	cmdSS << " handle 1: root htb default 10";
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);

	cmdSS.str("");
	cmdSS << " tc class replace dev " << iface;
	cmdSS << " parent 1: classid 1:10 htb rate " << adjustedRate;
	cmdSS << " burst " << rate; // byte (default was adjustedRate / 800)
	cmdSS << " cburst " << rate; // byte (default was adjustedRate / 800)
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);

	cmdSS.str("");
	cmdSS << " tc class replace dev " << iface;
	cmdSS << " parent 1: classid 1:20 htb rate " << adjustedRate;
	cmdSS << " burst " << rate; // byte (default was adjustedRate / 800)
	cmdSS << " cburst " << rate; // byte (default was adjustedRate / 800)
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);

	cmdSS.str("");
	cmdSS << " tc class replace dev " << iface;
	cmdSS << " parent 1: classid 1:30 htb rate " << adjustedRate;
	cmdSS << " burst " << rate; // byte (default was adjustedRate / 800)
	cmdSS << " cburst " << rate; // byte (default was adjustedRate / 800)
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);

	cmdSS.str("");
	cmdSS << " tc qdisc replace dev " << iface;
	cmdSS << " parent 1:10 handle 10: netem delay " << adjustedDelay;
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);

	cmdSS.str("");
	cmdSS << " tc qdisc replace dev " << iface;
	cmdSS << " parent 1:20 handle 20: netem delay " << adjustedDelay;
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);

	cmdSS.str("");
	cmdSS << " tc filter replace dev " << iface;
	cmdSS << " parent 1: protocol ip prio 1 u32 match ip src 192.168.0.0/16 flowid 1:20";
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);

	cmdSS.str("");
	cmdSS << " tc filter replace dev " << iface;
	cmdSS << " parent 1: protocol ip prio 1 u32 match ip src 172.16.0.0/16 flowid 1:30";
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);
}

void InterfaceConfigurator::changeInterfaceAttribute(std::string iface,
		long long rate, long long delay, long double tdf) {
	LOG4CXX_TRACE(logger,
			"Entering InterfaceConfigurator::changeInterfaceAttribute()");

	int status;
	long long adjustedRate = rate / tdf;
	long long adjustedDelay = delay * tdf;

	std::ostringstream cmdSS;

	cmdSS.str("");
	cmdSS << " tc class change dev " << iface;
	cmdSS << " classid 1:10 htb rate " << adjustedRate;
	cmdSS << " burst " << rate; // byte (default was adjustedRate / 800)
	cmdSS << " cburst " << rate; // byte (default was adjustedRate / 800)
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);

	cmdSS.str("");
	cmdSS << " tc class change dev " << iface;
	cmdSS << " classid 1:20 htb rate " << adjustedRate;
	cmdSS << " burst " << rate; // byte (default was adjustedRate / 800)
	cmdSS << " cburst " << rate; // byte (default was adjustedRate / 800)
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);

	cmdSS.str("");
	cmdSS << " tc class change dev " << iface;
	cmdSS << " classid 1:30 htb rate " << adjustedRate;
	cmdSS << " burst " << rate; // byte (default was adjustedRate / 800)
	cmdSS << " cburst " << rate; // byte (default was adjustedRate / 800)
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);

	cmdSS.str("");
	cmdSS << " tc qdisc change dev " << iface;
	cmdSS << " handle 10: netem delay " << adjustedDelay;
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);

	cmdSS.str("");
	cmdSS << " tc qdisc change dev " << iface;
	cmdSS << " handle 20: netem delay " << adjustedDelay;
	LOG4CXX_DEBUG(logger, "cmdSS = " << cmdSS.str());
	if ((status = system4Background(cmdSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: status = " << status);

}

void InterfaceConfigurator::deleteInterfaceAttribute(std::string ifaceName) {
	LOG4CXX_TRACE(logger,
			"Entering InterfaceConfigurator::deleteInterfaceAttribute()");

	// Delete interface with "tc qdisc del dev 'iface_name' root
	int status;
	std::ostringstream deleteSS;
	deleteSS << " tc qdisc del dev " << ifaceName << " root";
	LOG4CXX_DEBUG(logger, "deleteSS = " << deleteSS.str());

	if ((status = system4Background(deleteSS.str())) < 0)
		LOG4CXX_FATAL(logger, "system() error: delete");
}

void InterfaceConfigurator::updateInterfaceAttribute() {
	LOG4CXX_TRACE(logger,
			"Entering InterfaceConfigurator::updateInterfaceAttribute()");

	const map<string, InterfaceAttributeEntryPtr>& ifaceAttrMap =
			memDb_.ifaceAttrTable.getIfaceAttrMap();
	map<string, InterfaceAttributeEntryPtr>::const_iterator it;

//	for (it = ifaceAttrMap.begin(); it != ifaceAttrMap.end(); it++) {
//		string iface = it->first;
//		if(it->second->flagKeep){
//			long rate = it->second->rate;
//			long delay = it->second->delay;
//			long double tdf = memDb_.tdfTable.getLatestTdf();
//			replaceInterfaceAttribute(iface, rate, delay, tdf);
//		}else{
//			deleteInterfaceAttribute(iface);
//		}
//	}

	it = ifaceAttrMap.begin();
	while (it != ifaceAttrMap.end()) {
		string ifaceName = it->first;
		if(it->second->flagKeep){
			long long rate = it->second->rate;
			long long delay = it->second->delay;
			long double tdf = memDb_.tdfTable.getLatestTdf();
			LOG4CXX_DEBUG(logger, "rate" << rate << "delay" << delay << "tdf" << tdf);
			changeInterfaceAttribute(ifaceName, rate, delay, tdf);
			it++;
		}else{
			deleteInterfaceAttribute(ifaceName);
			it++;
			memDb_.ifaceAttrTable.erase(ifaceName);
			//ifaceAttrMap.erase(it++);
		}
	}
}



int InterfaceConfigurator::system4Background(string cmdString) {
	pid_t pid;
	int status;

	boost::trim(cmdString);

	vector<std::string> msgToken;
	boost::split(msgToken, cmdString, boost::is_any_of(" "),
			boost::token_compress_on);

	int msgTokenSize = msgToken.size();
	char const *newargv[msgTokenSize + 1];

	for (int i = 0; i < msgTokenSize; i++) {
		newargv[i] = msgToken[i].c_str();
	}

	newargv[msgTokenSize] = NULL;

//	for (int i = 0; i < msgTokenSize + 1; i++) {
//		cout << newargv[i] << endl;
//	}

	if (cmdString.empty())
		return (1);

	if ((pid = fork()) < 0) {
		status = -1;
	} else if (pid == 0) {
		execvp(newargv[0], (char* const *) newargv);
		LOG4CXX_DEBUG(logger, "InterfaceConfigurator::system4Background: pid = " << pid << ": " << newargv[0] << ", " << newargv[1]);
		_exit(127);
	} else {
		LOG4CXX_DEBUG(logger, "InterfaceConfigurator::system4Background: pid = " << pid );
		while (waitpid(pid, &status, 0) < 0)
			if (errno != EINTR) {
				status = errno;
				break;
			}
	}

	return (status);
}


/////////////////////////////////
// replaceInterfaceAttribute() //
/////////////////////////////////
/*
 	std::ostringstream rootSS;
	std::ostringstream rateSS;
	std::ostringstream delaySS;

	//rateSS << "echo " << password << " |sudo -S";
	rateSS << " tc qdisc replace dev " << iface;
	rateSS << " root handle 1: tbf rate " << adjustedRate;
	rateSS << " burst " << adjustedRate / 1000; // byte
	rateSS << " latency " << 100000 * tdf; // us
	LOG4CXX_DEBUG(logger, "rateSS = " << rateSS.str());

	//delaySS << "echo " << password << " |sudo -S";
	delaySS << " tc qdisc replace dev " << iface;
	delaySS << " parent 1: handle 10: netem delay " << adjustedDelay; // us
	LOG4CXX_DEBUG(logger, "delaySS = " << delaySS.str());
*/

	// Test code ////////////////////////////////////////////////////////////////
/*
	rateSS << " tc qdisc replace dev " << iface;
	rateSS << " root tbf rate " << adjustedRate;
	rateSS << " burst " << adjustedRate / 1000; // byte
	rateSS << " latency " << 100000 * tdf; // us
	LOG4CXX_DEBUG(logger, "rateSS = " << rateSS.str());

	delaySS << " tc qdisc replace dev " << iface << "e";
	delaySS << " root netem delay " << adjustedDelay; // us
	LOG4CXX_DEBUG(logger, "delaySS = " << delaySS.str());
*/
	////////////////////////////////////////////////////////////////////////////
/*
	rootSS << " tc qdisc replace dev " << iface;
	rootSS << " handle 1: root htb default 5";
	LOG4CXX_DEBUG(logger, "rootSS = " << rootSS.str());

	rateSS << " tc class replace dev " << iface;
	rateSS << " parent 1: classid 1:5 htb rate " << adjustedRate;
	rateSS << " burst " << adjustedRate / 1000; // byte
	LOG4CXX_DEBUG(logger, "rateSS = " << rateSS.str());

	delaySS << " tc qdisc replace dev " << iface;
	delaySS << " parent 1:5 handle 10: netem delay " << adjustedDelay; // us
	LOG4CXX_DEBUG(logger, "delaySS = " << delaySS.str());
*/


//////////////////////////////
// changeInterfaceAttribute //
//////////////////////////////
/*
std::ostringstream rateSS;
std::ostringstream delaySS;

//rateSS << "echo " << password << " |sudo -S";
rateSS << " tc qdisc change dev " << iface;
rateSS << " root handle 1: tbf rate " << adjustedRate;
rateSS << " burst " << adjustedRate / 1000; // byte
rateSS << " latency " << 100000 * tdf; // us
LOG4CXX_DEBUG(logger, "rateSS = " << rateSS.str());

//delaySS << "echo " << password << " |sudo -S";
delaySS << " tc qdisc replace dev " << iface;
delaySS << " parent 1: handle 10: netem delay " << adjustedDelay; // us
LOG4CXX_DEBUG(logger, "delaySS = " << delaySS.str());
*/
// Test code ////////////////////////////////////////////////////////////////
/*
rateSS << " tc qdisc change dev " << iface;
rateSS << " root tbf rate " << adjustedRate;
rateSS << " burst " << adjustedRate / 1000; // byte
rateSS << " latency " << 100000 * tdf; // us
LOG4CXX_DEBUG(logger, "rateSS = " << rateSS.str());

delaySS << " tc qdisc change dev " << iface << "e";
delaySS << " root netem delay " << adjustedDelay; // us
LOG4CXX_DEBUG(logger, "delaySS = " << delaySS.str());
*/
/////////////////////////////////////////////////////////////////////////////
/*
rateSS << " tc class change dev " << iface;
rateSS << " classid 1:5 htb rate " << adjustedRate;
rateSS << " burst " << adjustedRate / 1000; // byte
LOG4CXX_DEBUG(logger, "rateSS = " << rateSS.str());

delaySS << " tc qdisc change dev " << iface;
delaySS << " handle 10: netem delay " << adjustedDelay; // us
LOG4CXX_DEBUG(logger, "delaySS = " << delaySS.str());

if ((status = system4Background(rateSS.str())) < 0) // or system(rateSS.str().c_str()))
	LOG4CXX_FATAL(logger, "system() error: rate, status = " << status);

if ((status = system4Background(delaySS.str())) < 0)
	LOG4CXX_FATAL(logger, "system() error: delay, status = " << status);
*/

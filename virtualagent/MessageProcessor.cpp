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

#include "MessageProcessor.hpp"

log4cxx::LoggerPtr MessageProcessor::logger(
		log4cxx::Logger::getLogger("MessageProcessor"));

MessageProcessor::MessageProcessor(MemoryDatabase& memDb) :
		memDb_(memDb) {
}

void MessageProcessor::processUdpMsg(const char* udpMsg) {
	string udpMsgBody = udpMsg;
	parseUdpMsg(udpMsgBody);
}

void MessageProcessor::processTcpMsg(const char* tcpMsg) {
	string tcpMsgBody = tcpMsg;
	parseTcpMsg(tcpMsgBody);
}

void MessageProcessor::parseUdpMsg(string msg) {
	//LOG4CXX_DEBUG(logger, "parseUdpMsg: " + msg);

	vector<std::string> msgToken;
	boost::split(msgToken, msg, boost::is_any_of("|"),
			boost::token_compress_on);

	int protocolNumber = atoi(msgToken[0].c_str());

	switch (protocolNumber) {

	case 10: // Protocol-10: tdfTable.set(hostname, tdf)
	{
		if (msgToken.size() != 3) {
			LOG4CXX_ERROR(logger, "Protocol-10: size error, 3 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		string hostname = msgToken[1];
		long double tdf = atof(msgToken[2].c_str());
		LOG4CXX_INFO(logger, "Protocol-10: " << hostname << " " << tdf);
		memDb_.tdfTable.set(hostname, tdf);
		break;
	}

	case 3: // Protocol-3: ifaceAttrTable.set(ifaceName, rate, delay)
	{
		if (msgToken.size() != 4) {
			LOG4CXX_ERROR(logger, "Protocol-3: size error, 4 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		string ifaceName = msgToken[1];
		long long rate = atoll(msgToken[2].c_str());
		long long delay = atoll(msgToken[3].c_str());
		//cout << ifaceName << " " << rate << " " << delay << endl;
		LOG4CXX_INFO(logger, "Protocol-3: " << ifaceName << " " << rate << " " << delay);

		memDb_.ifaceAttrTable.set(ifaceName, rate, delay);
		//memDb_.tdfTable.cond.notify_all();
		long double latestTdf = memDb_.tdfTable.getLatestTdf();
		InterfaceConfigurator::changeInterfaceAttribute(ifaceName, rate, delay, latestTdf);
		break;
	}

	default:
		break;
	}
}

void MessageProcessor::parseTcpMsg(string msg) {
	//LOG4CXX_TRACE(logger, "parseTcpMsg: " + msg);

	vector<std::string> msgToken;
	boost::split(msgToken, msg, boost::is_any_of("|"),
			boost::token_compress_on);

	int protocolNumber = atoi(msgToken[0].c_str());

	switch (protocolNumber) {

	case 1: // Protocol-1: ifaceAttrTable.set(ifaceName, rate, delay)
	{
		if (msgToken.size() != 4) {
			LOG4CXX_ERROR(logger, "Protocol-1: size error, 4 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		string ifaceName = msgToken[1];
		long long rate = atoll(msgToken[2].c_str());
		long long delay = atoll(msgToken[3].c_str());
		//cout << ifaceName << " " << rate << " " << delay << endl;
		LOG4CXX_INFO(logger, "Protocol-1: " << ifaceName << " " << rate << " " << delay);

		memDb_.ifaceAttrTable.set(ifaceName, rate, delay);
		//memDb_.tdfTable.cond.notify_all();
		long double latestTdf = memDb_.tdfTable.getLatestTdf();
		InterfaceConfigurator::replaceInterfaceAttribute(ifaceName, rate, delay, 1.0);

		break;
	}

	case 2: // Protocol-2: ifaceAttrTable.erase(ifaceName)
	{
		if (msgToken.size() != 2) {
			LOG4CXX_ERROR(logger, "Protocol-2: size error, 2 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		string ifaceName = msgToken[1];
		LOG4CXX_INFO(logger, "Protocol-2: " << ifaceName);


		memDb_.ifaceAttrTable.setFlagKeep(ifaceName, false);

//		InterfaceConfigurator ifaceConfig(memDb_);
//		ifaceConfig.deleteInterfaceAttribute(ifaceName);

		break;
	}

	case 11: // Protocol-11: enforceTdf(tdf)
	{
		if (msgToken.size() != 2) {
			LOG4CXX_ERROR(logger, "Protocol-11: size error, 2 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		long double tdf = atof(msgToken[1].c_str());
		LOG4CXX_WARN(logger, "Protocol-11: " << tdf);
		memDb_.enforceTdf(tdf);
		break;
	}

	case 12: // Protocol-12: tdfTable.enableTable()
	{
		if (msgToken.size() != 1) {
			LOG4CXX_ERROR(logger, "Protocol-12: size error, 1 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		LOG4CXX_INFO(logger, "Protocol-12: tdfTable.enableTable()");
		memDb_.tdfTable.enableTable();
		break;
	}

	case 13: // Protocol-13: tdfTable.disableTable()
	{
		if (msgToken.size() != 1) {
			LOG4CXX_ERROR(logger, "Protocol-13: size error, 1 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		LOG4CXX_INFO(logger, "Protocol-13: tdfTable.disableTable()");
		memDb_.tdfTable.disableTable();
		break;
	}

	case 14: // Protocol-14: tdfMean
	{
		if (msgToken.size() != 3) {
			LOG4CXX_ERROR(logger, "Protocol-11: size error, 3 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		long double tdfMin = atof(msgToken[1].c_str());
		long double tdfMax = atof(msgToken[2].c_str());
		LOG4CXX_INFO(logger, "Protocol-14: tdfMin = " << tdfMin << ", tdfMax = " << tdfMax);
		Configuration::tdfMin = tdfMin;
		Configuration::tdfMax = tdfMax;
		break;
	}

	case 20: // Protocol-20: changeSlope, nonlinearFactor, alpha
	{
		if (msgToken.size() != 4) {
			//fprintf(stderr, "Protocol number 12: size error");
			LOG4CXX_ERROR(logger, "Protocol-20: size error, 4 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		LOG4CXX_INFO(logger, "Protocol-20: changeSlope/nonlinearFactor/alpha");

		Configuration::changeSlope = atof(msgToken[1].c_str());
		Configuration::nonlinearFactor = atof(msgToken[2].c_str());
		Configuration::alpha = atof(msgToken[3].c_str());

		LOG4CXX_INFO(logger, "=====>	" 	<< Configuration::changeSlope << "	"
											<< Configuration::nonlinearFactor << "	"
											<< Configuration::alpha);

		break;
	}

	case 21: // Protocol-21: Set targetLoad
	{
		if (msgToken.size() != 2) {
			LOG4CXX_ERROR(logger, "Protocol-21: size error, 2 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		Configuration::targetLoad = atof(msgToken[1].c_str());
		LOG4CXX_INFO(logger, "Protocol-21: targetLoad = " << Configuration::targetLoad);
		break;
	}

	case 22: // Protocol-22: Set packetHandlePeriod
	{
		if (msgToken.size() != 2) {
			LOG4CXX_ERROR(logger, "Protocol-22: size error, 2 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		Configuration::packetHandlePeriod = atoi(msgToken[1].c_str());
		LOG4CXX_INFO(logger, "Protocol-22: packetHandlePeriod = " << Configuration::packetHandlePeriod);
		break;
	}

	case 23: // Protocol-23: Set tdfChangeThreshold
	{
		if (msgToken.size() != 2) {
			LOG4CXX_ERROR(logger, "Protocol-21: size error, 2 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		Configuration::tdfChangeThreshold = atof(msgToken[1].c_str());
		LOG4CXX_INFO(logger, "Protocol-23: tdfChangeThreshold = " << Configuration::tdfChangeThreshold);
		break;
	}

	case 24: // Protocol-24: Enable VM Suspensions
	{
		if (msgToken.size() != 1) {
			LOG4CXX_ERROR(logger, "Protocol-24: size error, 1 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		LOG4CXX_INFO(logger, "Protocol-24: Enable VM Suspensions");
		Configuration::isVMSuspensionsEnabled = 1;
		break;
	}

	case 25: // Protocol-25: Disable VM Suspensions
	{
		if (msgToken.size() != 1) {
			LOG4CXX_ERROR(logger, "Protocol-25: size error, 1 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		LOG4CXX_INFO(logger, "Protocol-25: Disable VM Suspensions");
		Configuration::isVMSuspensionsEnabled = 0;
		break;
	}

	case 26: // Protocol-26: Enable system load monitoring
	{
		if (msgToken.size() != 1) {
			LOG4CXX_ERROR(logger, "Protocol-26: size error, 1 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		LOG4CXX_INFO(logger, "Protocol-26: Enable system load monitoring");
		Configuration::isLoadMonitored = 1;
		break;
	}

	case 27: // Protocol-27: Disable system load monitoring
	{
		if (msgToken.size() != 1) {
			LOG4CXX_ERROR(logger, "Protocol-27: size error, 1 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		LOG4CXX_INFO(logger, "Protocol-27: Disable system load monitoring");
		Configuration::isLoadMonitored = 0;
		break;
	}

	case 28: // Protocol-28: Clear tdfTable
	{
		if (msgToken.size() != 1) {
			LOG4CXX_ERROR(logger, "Protocol-28: size error, 1 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		LOG4CXX_INFO(logger, "Protocol-28: Clear tdfTable");
		memDb_.tdfTable.clear();
		break;
	}

	case 30: // Protocol-30: Suspend VMs
	{
		if (msgToken.size() != 1) {
			LOG4CXX_ERROR(logger, "Protocol-30: size error, 1 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		LOG4CXX_INFO(logger, "Protocol-30: Suspend VMs");
		cpu_disable_ticks();
		semaphore_setval(semid_gostop, VIRHOST, SEMSTOP);
		break;
	}

	case 31: // Protocol-31: Resume VMs
	{
		if (msgToken.size() != 1) {
			LOG4CXX_ERROR(logger, "Protocol-31: size error, 1 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		LOG4CXX_INFO(logger, "Protocol-31: Resume VMs");
		cpu_enable_ticks();
		semaphore_setval(semid_gostop, VIRHOST, SEMGO);
		break;
	}

	case 32: // Protocol-32: Set monitor mode
	{
		if (msgToken.size() != 2) {
			LOG4CXX_ERROR(logger, "Protocol-32: size error, 2 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		Configuration::monitorMode = atoi(msgToken[1].c_str());
		LOG4CXX_INFO(logger, "Protocol-32: monitorMode = " << Configuration::monitorMode);
		break;
	}

	case 33: // Protocol-33: Set tdfSimGain
	{
		if (msgToken.size() != 2) {
			LOG4CXX_ERROR(logger, "Protocol-33: size error, 2 expected");
			for(unsigned int i = 0; i < msgToken.size(); i++)
				LOG4CXX_ERROR(logger, "" << msgToken[i]);
			break;
		}
		Configuration::tdfSimGain = atof(msgToken[1].c_str());
		LOG4CXX_INFO(logger, "Protocol-33 = " << Configuration::tdfSimGain);
		break;
	}

	default:
		break;
	}
}

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

#include "MemoryDatabase.hpp"

log4cxx::LoggerPtr TdfTable::logger(log4cxx::Logger::getLogger("TdfTable"));
log4cxx::LoggerPtr InterfaceAttributeTable::logger(
		log4cxx::Logger::getLogger("InterfaceAttributeTable"));
log4cxx::LoggerPtr MemoryDatabase::logger(
		log4cxx::Logger::getLogger("MemoryDatabase"));

//--------------------------------------------------------------------
TdfEntry::TdfEntry(long double baseTdf) {
	this->baseTdf = baseTdf;
}

//--------------------------------------------------------------------
InterfaceAttributeEntry::InterfaceAttributeEntry(long long rate, long long delay) {
	this->rate = rate;
	this->delay = delay;
	this->flagKeep = true;
}

//--------------------------------------------------------------------

TdfTable::TdfTable(): isTableEnabled_(false), latestTdf_(1.0) {
	LOG4CXX_TRACE(logger, "Entering TdfTable::TdfTable()");
}

void TdfTable::set(string hostname, long double tdfVal) {
	LOG4CXX_TRACE(logger, "Entering TdfTable::set()");

	LOG4CXX_DEBUG(logger, hostname << ":" << tdfVal);

	// Lock for set
	//boost::lock_guard<boost::mutex> lock(mutex);
	boost::unique_lock<boost::mutex> lock(mutex);

	TdfEntryPtr tdfEntryPtr(new TdfEntry(tdfVal));

	tdfMap[hostname] = tdfEntryPtr;
	// ** My note: The above line can be replaced by the following.
	//tdfMap.insert(pair<string, TdfEntryPtr>(hostname, tdfEntryPtr));

	// print();	// for testing

	if (isTableEnabled()) {
		long double maxTdfVal = maxTdf();

		long double diff = fabs(maxTdfVal - getLatestTdf());
		diff = diff / getLatestTdf();

		//if(maxTdfVal != getLatestTdf()){
		if(diff >= Configuration::tdfChangeThreshold){
			setLatestTdf(maxTdfVal);

			// log for a graph regarding TDF
			//LOG4CXX_WARN(logger, TimeGetter::getTimeUs() << " "	<< maxTdfVal);

			cond.notify_all();
		}
	}
}

TdfEntryPtr TdfTable::get(string hostname) {
	LOG4CXX_TRACE(logger, "Entering TdfTable::get(string hostname)");

	return tdfMap[hostname];
}

int TdfTable::getSize(){
	return tdfMap.size();
}

void TdfTable::erase(string& hostname) {
	LOG4CXX_TRACE(logger, "Entering TdfTable::erase(string& hostname)");
	tdfMap.erase(hostname);
}

void TdfTable::clear(){
	LOG4CXX_TRACE(logger, "Entering TdfTable::erase(string& hostname)");
	tdfMap.clear();
}

void TdfTable::enableTable() {
	LOG4CXX_TRACE(logger, "Entering TdfTable::enableTable()");

	isTableEnabled_ = true;
}
void TdfTable::disableTable() {
	LOG4CXX_TRACE(logger, "Entering TdfTable::disableTable()");

	isTableEnabled_ = false;
}
bool TdfTable::isTableEnabled() {
	LOG4CXX_TRACE(logger, "Entering TdfTable::isTableEnabled()");

	return isTableEnabled_;
}

void TdfTable::print() {
	LOG4CXX_TRACE(logger, "Entering TdfTable::print()");

	map<string, TdfEntryPtr>::iterator iter;
	for (iter = tdfMap.begin(); iter != tdfMap.end(); iter++) {
		cout << iter->first << ": " << iter->second->baseTdf << endl;
	}
}

long double TdfTable::maxTdf() {
	LOG4CXX_TRACE(logger, "Entering TdfTable::maxTdf()");
	ostringstream strs;	// for logging

	map<string, TdfEntryPtr>::iterator iter;
	iter = tdfMap.begin();
	long double maxVal = iter->second->baseTdf;

	for (; iter != tdfMap.end(); iter++) {
		strs << iter->first << ": ";
		strs << iter->second->baseTdf << " ";	// for logging

		if (maxVal < iter->second->baseTdf)
			maxVal = iter->second->baseTdf;
	}

	// log for a graph regarding TDF
	LOG4CXX_INFO(logger, TimeGetter::getTimeUs() << " " << latestTdf_ << " "<< maxVal << " " << strs.str());



	return maxVal;
}

long double TdfTable::getLatestTdf(){
	LOG4CXX_TRACE(logger, "Entering TdfTable::getLatestTdf()");

	return latestTdf_;
}

void TdfTable::setLatestTdf(long double tdfVal){
	LOG4CXX_TRACE(logger, "Entering TdfTable::setLatestTdf(long double tdfVal)");

	latestTdf_ = tdfVal;
}

//--------------------------------------------------------------------
const map<string, InterfaceAttributeEntryPtr>& InterfaceAttributeTable::getIfaceAttrMap() const {
	LOG4CXX_TRACE(logger, "Entering InterfaceAttributeTable::getIfaceAttrMap()");

	return ifaceAttrMap;
}

//map<string, InterfaceAttributeEntryPtr> InterfaceAttributeTable::getIfaceAttrMap() const {
//	return ifaceAttrMap;
//}

void InterfaceAttributeTable::set(string ifaceName, long long rate, long long delay) {
	LOG4CXX_TRACE(logger, "Entering InterfaceAttributeTable::set()");
	LOG4CXX_DEBUG(logger, ifaceName << ":" << rate << "," << delay);

	//Lock for set
	//boost::lock_guard<boost::mutex> lock(mutex);
	boost::unique_lock<boost::mutex> lock(mutex);

	InterfaceAttributeEntryPtr interfaceAttributeEntryPtr(
			new InterfaceAttributeEntry(rate, delay));
	ifaceAttrMap[ifaceName] = interfaceAttributeEntryPtr;

	// For testing
	//print();

}

InterfaceAttributeEntryPtr InterfaceAttributeTable::get(string ifaceName) {
	LOG4CXX_TRACE(logger, "Entering InterfaceAttributeTable::get(string ifaceName)");

	return ifaceAttrMap[ifaceName];
}

void InterfaceAttributeTable::setFlagKeep(string ifaceName, bool flagKeep) {
	LOG4CXX_TRACE(logger, "InterfaceAttributeTable::setFlagKeepFalse(string ifaceName)");
	ifaceAttrMap[ifaceName]->flagKeep = flagKeep;
}

void InterfaceAttributeTable::erase(string& ifaceName) {
	LOG4CXX_TRACE(logger, "InterfaceAttributeTable::erase(string& ifaceName)");
	ifaceAttrMap.erase(ifaceName);
}

void InterfaceAttributeTable::print() {
	LOG4CXX_TRACE(logger, "InterfaceAttributeTable::print()");

	map<string, InterfaceAttributeEntryPtr>::iterator iter;
	for (iter = ifaceAttrMap.begin(); iter != ifaceAttrMap.end(); iter++) {
		cout << iter->first << ": " << iter->second->rate << ", ";
		cout << iter->second->delay << endl;
	}
}

//--------------------------------------------------------------------


MemoryDatabase::MemoryDatabase() {
	LOG4CXX_TRACE(logger, "Entering MemoryDatabase::MemoryDatabase()");

	//tdfTable.enableTable();
}

MemoryDatabase::~MemoryDatabase() {
	LOG4CXX_TRACE(logger, "Entering MemoryDatabase::~MemoryDatabase()");
}

void MemoryDatabase::enforceTdf(long double tdfVal) {
	LOG4CXX_TRACE(logger, "Entering MemoryDatabase::enforceTdf(long double tdfVal)");

	// Lock
	boost::unique_lock<boost::mutex> lock(tdfTable.mutex);

	tdfTable.disableTable();
	tdfTable.setLatestTdf(tdfVal);

	tdfTable.cond.notify_all();
}

void MemoryDatabase::createSharedMemory() {
	LOG4CXX_TRACE(logger, "Entering MemoryDatabase::createSharedMemory()");

	sharedMemory.create();
}

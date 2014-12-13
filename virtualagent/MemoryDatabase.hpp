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


#ifndef MEMORY_DATABASE_HPP
#define MEMORY_DATABASE_HPP

#include <math.h>
#include <iostream>
#include <sstream>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <log4cxx/logger.h>
#include "SharedMemory.hpp"
#include "TimeGetter.hpp"
#include "Configuration.hpp"

using namespace std;

//--------------------------------------------------------------------
class TdfEntry {
public:
	TdfEntry(long double baseTdf);

	long double baseTdf;
	//long double transit_tdf;
};

//--------------------------------------------------------------------
class InterfaceAttributeEntry {
public:
	InterfaceAttributeEntry(long long rate, long long delay);

	long long rate;
	long long delay;
	bool flagKeep;
};

//--------------------------------------------------------------------
typedef boost::shared_ptr<TdfEntry> TdfEntryPtr;

class TdfTable {
public:
	TdfTable();
	void set(string hostname, long double tdfVal);
	TdfEntryPtr get(string hostname);
	int getSize(); // Get map size
	void erase(string& hostname);
	void clear();
	void enableTable();
	void disableTable();
	bool isTableEnabled();
	long double maxTdf();
	long double getLatestTdf();
	void setLatestTdf(long double tdfVal);
	void print();

	// Class attributes
	boost::mutex mutex;
	boost::condition_variable_any cond;

private:
	static log4cxx::LoggerPtr logger;
	map<string,TdfEntryPtr> tdfMap;
	bool isTableEnabled_;
	long double latestTdf_;
};

//--------------------------------------------------------------------
typedef boost::shared_ptr<InterfaceAttributeEntry> InterfaceAttributeEntryPtr;

class InterfaceAttributeTable {
public:
	void set(string ifaceName, long long rate, long long delay);
	const map<string, InterfaceAttributeEntryPtr>& getIfaceAttrMap() const;
	//map<string, InterfaceAttributeEntryPtr> getIfaceAttrMap() const;
	InterfaceAttributeEntryPtr get(string ifaceName);
	void setFlagKeep(string ifaceName, bool flagKeep);
	void erase(string& ifaceName);
	void print();

private:
	static log4cxx::LoggerPtr logger;
	boost::mutex mutex;
	boost::condition_variable_any cond;
	map<string, InterfaceAttributeEntryPtr> ifaceAttrMap;
};

//--------------------------------------------------------------------
class MemoryDatabase {
public:
	// Constructor
	MemoryDatabase();
	~MemoryDatabase();

	void enforceTdf(long double tdf);
	//void setTdf(long double tdf);
	//long double getTdf();
	void createSharedMemory();

	// Class attributes
	TdfTable tdfTable;
	InterfaceAttributeTable ifaceAttrTable;
	SharedMemory sharedMemory;

private:
	static log4cxx::LoggerPtr logger;

};

#endif /* MEMORY_DATABASE_HPP */

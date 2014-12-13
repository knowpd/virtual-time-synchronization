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

#include "TcpConnectionManager.hpp"

void TcpConnectionManager::join(connParticipantPtr participant) {
	participants_.insert(participant);
	std::for_each(recentMsgs_.begin(), recentMsgs_.end(),
			boost::bind(&ConnParticipant::deliver, participant, _1));
}

void TcpConnectionManager::leave(connParticipantPtr participant) {
	participants_.erase(participant);
}

void TcpConnectionManager::deliver(const RawMessage& msg) {
	recentMsgs_.push_back(msg);
	while (recentMsgs_.size() > MAX_RECENT_MSGS)
		recentMsgs_.pop_front();

	std::for_each(participants_.begin(), participants_.end(),
			boost::bind(&ConnParticipant::deliver, _1, boost::ref(msg)));
}

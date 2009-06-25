/**
 * *********************************************************************
 * OpenSWG Sandbox Server
 * Copyright (C) 2006 OpenSWG Team <http://www.openswg.com>
 * *********************************************************************
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * To read the license please visit http://www.gnu.org/copyleft/gpl.html
 * *********************************************************************
 */

#ifndef OPENSWG_PLAYER_H
#define OPENSWG_PLAYER_H

#ifdef _MSC_VER
#include "stdint.h"
#else
#include <cstdint>
#endif

#include <string>

#include "Vector3.h"

class Player
{
public:
	Player();

    const Vector3<int>& position() const;
    void position(const Vector3<int>& position);

    int stationId() const;
    void stationId(int id);

	bool IsInitialized() { return mInitialized; };
	void ToggleInitialized();

	const char* GetStationName() { return mStationName.c_str(); }
	void SetStationName(char* stationName[17]) {
		std::string name((char*)stationName);
		mStationName = name; 
	}
	

	uint8_t GetMood() { return mMoodId; }
	void SetMood(uint8_t moodId) { mMoodId = moodId; }
	std::string GetLocation() { return mLocation; }
	void SetLocation(std::string location) { mLocation = location; }

private:
    Vector3<int> position_;
    int station_id_;

	bool mInitialized;
	uint8_t mMoodId;
	std::string mStationName;
	std::string mLocation;
};

#endif // OPENSWG_PLAYER_H
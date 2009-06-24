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

#ifndef OPENSWG_SOCKET_SERVER_H
#define OPENSWG_SOCKET_SERVER_H

#define MAX_RECV_SIZE 512

#ifdef _MSC_VER
#include "stdint.h"
#else
#include <cstdint>
#endif

#include <map>
#include <string>
#include <tr1/memory>

#include "NetworkAddress.h"

class GalaxySession;
class SocketServerImpl;

/** SocketServer class
 *  This class handles the sending and receiving of data from the client.
 */
class SocketServer
{
public:
	SocketServer();
    explicit SocketServer(uint16_t port);
    virtual ~SocketServer();

	/** Run Server function
	 *	Begins listening on the port specified in the configuration until
	 *	the server status is no longer set to running.
	 */
	void run();

	/** On Incoming Data function
	 *	Whenever information is received via the socket this function is
	 *	called to handle the data.
	 */
	void OnIncoming(NetworkAddress address, char *packet, size_t length);

	/** Updates the server state.
	 */
	void update();

	/** Send Packet function
	 *	Sends a packet to the specified to the specified client.
	 */
    void SendPacket(const NetworkAddress& address, char *packet, unsigned short length);

	/** Add New SWG Client function
	 *	Adds a new swg client to the client map.
	 */
    std::tr1::shared_ptr<GalaxySession> AddGalaxySession(NetworkAddress address);

	const uint16_t port();

private:	
	/** Updates the zone state.
	 */
	virtual void onUpdate() = 0;

    std::tr1::shared_ptr<SocketServerImpl> pimpl_;

    typedef std::map<NetworkAddress, std::tr1::shared_ptr<GalaxySession>> GalaxySessionMap;
	GalaxySessionMap sessions_;	

	time_t current_time_;
    time_t last_cleanup_time_;
};

#endif // OPENSWG_SOCKET_SERVER_H
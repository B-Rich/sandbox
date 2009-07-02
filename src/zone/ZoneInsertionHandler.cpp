/*
 * OpenSWG Combat Upgrade Sandbox - ZoneInsertionHandler.cpp
 *
 * @copyright   Copyright (c) 2009 Eric S. Barr Jr.
 * @author      Eric S. Barr Jr. <eric.barr@ericscottbarr.com>
**/

#include "ZoneInsertionHandler.h"
#include "Session.h"
#include "PacketTools.h"


void HandleClientReady(Session& session, std::tr1::shared_ptr<ByteBuffer> message)
{
	session.sendHeartbeat();
}


void HandleLoadDone(Session& session, std::tr1::shared_ptr<ByteBuffer> message)
{
	session.sendHardcodedPacket("packets\\ZoneInsertion\\LoadDone.txt", true);
}


void HandleLoadTerrain(Session& session, std::tr1::shared_ptr<ByteBuffer> message)
{
	session.sendHeartbeat();
}


void HandleZoneInsertionRequest(Session& session, std::tr1::shared_ptr<ByteBuffer> message)
{
	session.sendHeartbeat();

	// Send the initial batch of packets.
	session.sendHardcodedPacket("packets\\ZoneInsertion\\UnkByteFlag.txt", false);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\Int900Pkt.txt", false);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\LoadTerrain.txt", true);

	// Begin loading the character.
	session.sendHardcodedPacket("packets\\ZoneInsertion\\PreloadCharacter.txt", false);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\CreoCreate.txt", false);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\Creo3.txt", true);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\Creo6.txt", true);

	session.sendHardcodedPacket("packets\\ZoneInsertion\\Creo1.txt", true);

	// @TODO Implement a better way to send hardcoded fragmented packets.
	session.sendHardcodedPacket("packets\\ZoneInsertion\\Creo4.txt", true);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\Creo4pt2.txt", true);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\Creo4pt3.txt", true);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\Creo4pt4.txt", true);

	// Create our character.
	session.sendHardcodedPacket("packets\\ZoneInsertion\\Creo8.txt", true);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\Play8.txt", true);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\Play9.txt", false);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\PrecloseClose.txt", false);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\PlayClose.txt", false);
	session.sendHardcodedPacket("packets\\ZoneInsertion\\Play9Update.txt", false);

	// Inventory
	session.sendHardcodedPacket("packets\\ZoneInsertion\\Inventory.txt", false);

	// Environment
	session.sendHardcodedPacket("packets\\Objects\\KerenStarship.txt", false);
	session.sendHardcodedPacket("packets\\Objects\\KerenStarshipTicketCollector.txt", false);
}


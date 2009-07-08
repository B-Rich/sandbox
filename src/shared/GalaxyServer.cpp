/*
 * OpenSWG: GalaxyEngine - GalaxyServer.cpp
 *
 * @copyright   Copyright (c) 2009 Eric S. Barr Jr.
 * @author      Eric S. Barr Jr. <eric.barr@ericscottbarr.com>
**/

#include "GalaxyServer.h"
#include "Logger.h"
#include "Session.h"
#include "boost/thread.hpp"

GalaxyServer::GalaxyServer(uint16_t port)
    : network_listener_(port)
{    
    soe_protocol_.addHandler(0x0001, std::tr1::bind(&GalaxyServer::handleSessionRequest, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
    soe_protocol_.addHandler(0x0003, std::tr1::bind(&GalaxyServer::handleMultiPacket, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
    soe_protocol_.addHandler(0x0005, std::tr1::bind(&GalaxyServer::handleDisconnect, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
    soe_protocol_.addHandler(0x0006, std::tr1::bind(&GalaxyServer::handlePing, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
    soe_protocol_.addHandler(0x0007, std::tr1::bind(&GalaxyServer::handleNetStatus, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
    soe_protocol_.addHandler(0x0009, std::tr1::bind(&GalaxyServer::handleDataChannel, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
    soe_protocol_.addHandler(0x0015, std::tr1::bind(&GalaxyServer::handleAcknowledge, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
}


GalaxyServer::~GalaxyServer()
{}


uint16_t GalaxyServer::port()
{
    return network_listener_.port();
}


void GalaxyServer::port(uint16_t port)
{
    network_listener_.port(port);
}


void GalaxyServer::run()
{
    initializeProtocol();
        
    for(;;) {
        onUpdate();

        network_listener_.poll();

        boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}
}


void GalaxyServer::handleIncoming(const NetworkAddress& address, ByteBuffer& message)
{
    uint16_t opcode = message.read<uint16_t>();
    
	SoeMessageHandler handler = soe_protocol_.find(opcode);

    if (! handler) {
        Logger().log(INFO) << "Unidentified message received" << std::endl << message;
        return;
    }

    handler(address, message);
}


void GalaxyServer::sendToRemote(const NetworkAddress& address, ByteBuffer& message) const
{
    network_listener_.sendToRemote(address, message);
}


std::tr1::shared_ptr<Session> GalaxyServer::addSession(const NetworkAddress& address)
{
    Logger().log(INFO) << "Adding session for [" << address << "]";

	// Create a new session and store it in the session map. 
    std::tr1::shared_ptr<Session> session(new Session(*this, address, swg_protocol_));
	sessions_[address] = session;

	// Return the new session.
	return session;
}


std::tr1::shared_ptr<Session> GalaxyServer::findSession(const NetworkAddress& address)
{
    std::tr1::shared_ptr<Session> session;

	SessionMap::iterator i = sessions_.find(address);
	if (i != sessions_.end()) {
        session = (*i).second;
	}

    return session;
}


std::tr1::shared_ptr<Session> GalaxyServer::removeSession(const NetworkAddress& address)
{
    std::tr1::shared_ptr<Session> session;

    SessionMap::iterator i = sessions_.find(address);
    if (i != sessions_.end()) {
        session = (*i).second;
        sessions_.erase(i);
    }

    return session;
}


uint32_t GalaxyServer::sessionCount() const
{
    return sessions_.size();
}


void GalaxyServer::handleSessionRequest(const NetworkAddress& address, ByteBuffer& message)
{
    /*
    std::tr1::shared_ptr<Session> session = findSession(address);

    if (session) {
        Logger().log(INFO) << "Received session request from an address with an existing session: [" << address << "]";
        return;
    }

    session = addSession(address);

    session->crcLength(message->read<uint32_t>());
    session->connectionId(message->read<uint32_t>());
    session->maxUdpSize(message->read<uint32_t>());

    ByteBuffer session_response(SoeMessageFactory::buildSessionResponse(session));
    sendToRemote(address, session_response);
    */
}


void GalaxyServer::handleNetStatus(const NetworkAddress& address, ByteBuffer& message)
{}


void GalaxyServer::handleMultiPacket(const NetworkAddress& address, ByteBuffer& message)
{}


void GalaxyServer::handleAcknowledge(const NetworkAddress& address, ByteBuffer& message)
{}


void GalaxyServer::handleDataChannel(const NetworkAddress& address, ByteBuffer& message)
{}


void GalaxyServer::handleDisconnect(const NetworkAddress& address, ByteBuffer& message)
{}


void GalaxyServer::handlePing(const NetworkAddress& address, ByteBuffer& message)
{}


/*
 * OpenSWG Combat Upgrade Sandbox - ZoneServer.h
 *
 * @copyright   Copyright (c) 2009 Eric S. Barr Jr.
 * @author      Eric S. Barr Jr. <eric.barr@ericscottbarr.com>
**/

#include "SocketServer.h"

class ZoneServer : public SocketServer
{
public:
    explicit ZoneServer(uint16_t port);
    virtual ~ZoneServer();

    uint8_t shuttleState() const;
    void    shuttleState(uint8_t state);
    void    sendShuttleUpdate();
    
private:
	ZoneServer();
    ZoneServer(const ZoneServer&);
    ZoneServer& operator=(const ZoneServer&);

    virtual void onIncoming(const NetworkAddress& address, std::tr1::shared_ptr<ByteBuffer> message);
    virtual void onUpdate();
    virtual void initializeProtocol();

    Protocol<uint32_t> protocol_;
        
	uint8_t  shuttle_state_;
};
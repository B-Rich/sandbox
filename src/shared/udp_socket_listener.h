/*
 * SWG:ANH Combat Upgrade Sandbox
 *
 * @copyright   Copyright (c) 2010 The SWG:ANH Team
 * @author      Eric Barr <apathy@swganh.org>
**/

#ifndef UDP_SOCKET_LISTENER_H_
#define UDP_SOCKET_LISTENER_H_

#ifdef _MSC_VER
#include <functional>
#include <memory>
#else
#include <tr1/functional>
#include <tr1/memory>
#endif

#include <cstdint>

#include "network_address.h"

namespace sandbox {
namespace shared {

class ByteBuffer;

class UdpSocketListener
{
public:
    typedef std::tr1::function<void (const NetworkAddress&, ByteBuffer&)> Callback;

    UdpSocketListener(uint16_t port);
    ~UdpSocketListener();

    uint16_t port() const;
    void     port(uint16_t port);

    Callback callback();
    void     callback(Callback callback);

    void poll();
    void sendToRemote(const NetworkAddress& address, ByteBuffer& message) const;

private:
    UdpSocketListener();
    UdpSocketListener(const UdpSocketListener&);
    UdpSocketListener& operator=(const UdpSocketListener&);

    class UdpSocketListenerImpl;
    std::tr1::shared_ptr<UdpSocketListenerImpl> impl_;
};

}  // namespace sandbox
}  // namespace shared

#endif // UDP_SOCKET_LISTENER_H_


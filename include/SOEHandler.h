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

#ifndef OPENSWG_SOE_HANDLER_H
#define OPENSWG_SOE_HANDLER_H

#include <tr1/memory>

class GalaxySession;
class ByteBuffer;

void HandleSessionRequest(GalaxySession& session, std::tr1::shared_ptr<ByteBuffer> message);
void HandleNetStatus(GalaxySession& session, std::tr1::shared_ptr<ByteBuffer> message);
void HandleMultiPacket(GalaxySession& session, std::tr1::shared_ptr<ByteBuffer> message);
void HandleAcknowledge(GalaxySession& session, std::tr1::shared_ptr<ByteBuffer> message);
void HandleDataChannel(GalaxySession& session, std::tr1::shared_ptr<ByteBuffer> message);

void HandleSessionRequest(GalaxySession *session, const unsigned char *data, unsigned short length);
void HandleNetStatus(GalaxySession *session, const unsigned char *data, unsigned short length);
void HandleMultiPacket(GalaxySession *session, const unsigned char *data, unsigned short length);
void HandleAcknowledge(GalaxySession *session, const unsigned char *data, unsigned short length);
void HandleDataChannel(GalaxySession *session, const unsigned char *data, unsigned short length);
void HandleDisconnect(GalaxySession *session, const unsigned char *data, unsigned short length);
void HandlePing(GalaxySession *session, const unsigned char *data, unsigned short length);

#endif // OPENSWG_SOE_HANDLER_H
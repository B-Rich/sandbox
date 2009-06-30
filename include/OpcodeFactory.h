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

#ifndef OPENSWG_OPCODE_FACTORY_H
#define OPENSWG_OPCODE_FACTORY_H

#ifdef _MSC_VER
#include "stdint.h"
#else
#include <cstdint>
#endif

#include <map>
#include <tr1/functional>

/** Exception that is thrown by the OpcodeFactory class. Client code
 *	using the opcode handler should check to see if this exception is
 *	being thrown.
 */
struct OpcodeHandlerException
{};

// Forward declaration of GalaxySession needed b the opcode handlers.
class GalaxySession;
class ByteBuffer;

typedef std::tr1::function<void (GalaxySession&, std::tr1::shared_ptr<ByteBuffer>)> MessageHandler;
typedef std::map<uint32_t, MessageHandler> MessageHandlers;

/** Define our handler function type.
 */
typedef void(*handlerFunc)(GalaxySession *session, const unsigned char *packet, unsigned short length);

/** Define the map to hold the opcode handlers.
 */
typedef std::map<unsigned int, handlerFunc> OpcodeHandlers;

/** Opcode Factory
 *  This is a factory class that creates and returns opcode handlers.
 */
class OpcodeFactory
{
public:
	static MessageHandler getOpcodeHandler(std::tr1::shared_ptr<ByteBuffer> packet);
	static MessageHandler getOpcodeHandler(uint32_t opcode);

	/** Get Opcode Handler
	 *	Returns the opcode handler by parsing the raw packet. This returns the
	 *	handler for the first opcode found in the packet.
	 */
	static handlerFunc GetOpcodeHandler(std::tr1::shared_ptr<ByteBuffer> packet);
 
	/** Get Opcode Handler
	 *	Returns the opcode handler based on the opcode passed in.
	 */
	static handlerFunc GetOpcodeHandler(unsigned int* opcode);

private:
	static MessageHandlers _buildMessageHandlerMap();

	static OpcodeHandlers _buildOpcodeHandlerMap();
};

#endif // OPENSWG_OPCODE_FACTORY_H
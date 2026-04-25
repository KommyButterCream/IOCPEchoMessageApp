#pragma once

#include <stdint.h>

#include "../../../Modules/IOCPEngine/Job/JobDefs.h" // for HandlerContext

class ISession;
class PacketHandlerTable;

namespace EchoMessage
{
	namespace Server
	{
		// Register
		bool RegisterHandlers(PacketHandlerTable* handlerTable);

		// Packet Handlers
		bool HandleEchoRequest(ISession* session, const char* packetData, uint32_t packetSize, const HandlerContext& context);
	}
}

#include "EchoMessageClient.h"

#include <string.h>

#include "../../Modules/IOCPEngine/Job/Job.h"
#include "../../Modules/IOCPEngine/HandlerTable/PacketHandlerTable.h"

#include "../../Modules/IOCPEngine/Memory/SlabMemoryPoolHelper.h"

#include "../../Modules/IOCPEngine/Protocol/PacketID.h"

#include "../../Modules/IOCPEngine/Session/SessionJobQueue.h"
#include "../../Modules/IOCPEngine/Session/ClientSession.h" // for ClientSession

#include "../EchoProtocol/EchoPacket.h"
#include "./PacketHandler/EchoHandler_Client.h"

EchoMessageClient::EchoMessageClient()
	: m_requestSequence(0)
{

}

EchoMessageClient::~EchoMessageClient()
{
	StopClient();
}

bool EchoMessageClient::StartClient(const char* ipAddress, const uint16_t port)
{
	if (!IOCPClient::StartClient(ipAddress, port))
		return false;

	bool registerResult = true;

	registerResult &= EchoMessage::Client::RegisterHandlers(GetPacketHandlerTable());

	return registerResult;
}

void EchoMessageClient::StopClient()
{
	IOCPClient::StopClient();
}

void EchoMessageClient::OnClientConnect(ISession* session)
{

}

void EchoMessageClient::OnClientDisconnect(ISession* session)
{

}

void EchoMessageClient::OnReceive(ISession* session, uint16_t packetId, const char* packetData, uint32_t packetSize)
{
	const char* payload = packetData + sizeof(PACKET_HEADER);
	uint32_t payloadSize = packetSize - sizeof(PACKET_HEADER);

	ClientSession* clientSession = dynamic_cast<ClientSession*>(session);

	if (!clientSession)
	{
		__debugbreak();

		return;
	}

	Logger::Log(LogLevel::LOG_INFO, "[%s] RECV PACKET - ID: %u, Size: %d, %d, %s", __FUNCTION__, session->GetSessionID(), packetId, packetSize, payload);

	PacketHandlerTable* packetHandlerTable = GetPacketHandlerTable();
	if (!packetHandlerTable)
	{
		__debugbreak();

		return;
	}

	PacketHandlerFunc packetHandler = packetHandlerTable->GetHandler(packetId);
	if (!packetHandler)
	{
		__debugbreak();

		return;
	}

	// Job 생성
	Job* job = MEMORY_POOL::CreateJob(*GetJobMemoryPool());
	if (!job)
	{
		__debugbreak();

		return;
	}

	job->SetPacketJob(JobType::PACKET, packetHandler, session, packetId, packetData, packetSize, GetHandlerContext());

	bool wasEmpty = false;
	bool enqueueSucceeded = clientSession->GetJobQueue().EnqueueJob(job, wasEmpty);
}

void EchoMessageClient::OnSend(ISession* session, uint32_t bytesTransferred)
{
	ClientSession* clientSession = dynamic_cast<ClientSession*>(session);

	if (!clientSession)
	{
		__debugbreak();

		return;
	}
}

bool EchoMessageClient::SendChatMessage(const char* message)
{
	if (!message)
		return false;

	ClientSession* clientSession = GetClientSession();
	if (!clientSession || !clientSession->IsConnected())
		return false;

	PacketHandlerTable* packetHandlerTable = GetPacketHandlerTable();
	if (!packetHandlerTable)
	{
		__debugbreak();

		return false;
	}

	PacketHandlerFunc packetHandler = packetHandlerTable->GetHandler(ToPacketID(ECHO_PACKET_ID::CS_ECHO_REQUEST));
	if (!packetHandler)
	{
		__debugbreak();

		return false;
	}

	// Job 생성
	Job* job = MEMORY_POOL::CreateJob(*GetJobMemoryPool());
	if (!job)
	{
		__debugbreak();

		return false;
	}

	// Packet 생성
	void* packetMemory = MEMORY_POOL::CreatePacket(*GetPacketMemoryPool(), sizeof(CS_ECHO_REQUEST_PACKET));
	CS_ECHO_REQUEST_PACKET* requestPacket = reinterpret_cast<CS_ECHO_REQUEST_PACKET*>(packetMemory);
	if (!requestPacket)
		return false;

	requestPacket->header.packetId = ToPacketID(ECHO_PACKET_ID::CS_ECHO_REQUEST);
	requestPacket->header.packetSize = sizeof(CS_ECHO_REQUEST_PACKET);

	requestPacket->requestId = 0/*++m_requestSequence*/;

	size_t maxMessageLength = sizeof(requestPacket->message);
	size_t messageLength = strnlen_s(message, maxMessageLength - 1);
	memcpy_s(requestPacket->message, sizeof(requestPacket->message), message, messageLength);
	requestPacket->message[messageLength] = '\0';

	job->SetPacketJob(JobType::PACKET, packetHandler, clientSession, ToPacketID(ECHO_PACKET_ID::CS_ECHO_REQUEST), reinterpret_cast<const char*>(requestPacket), sizeof(CS_ECHO_REQUEST_PACKET), GetHandlerContext());

	bool wasEmpty = false;
	bool enqueueSucceeded = clientSession->GetJobQueue().EnqueueJob(job, wasEmpty);

	return enqueueSucceeded;
}

bool EchoMessageClient::EnqueueSendPacket(void** packetData, uint32_t packetSize)
{
	if (!packetData || packetSize == 0)
		return false;

	ClientSession* clientSession = GetClientSession();
	if (!clientSession || !clientSession->IsConnected())
		return false;

	if (!clientSession->EnqueueSendPacket(packetData, packetSize))
		return false;

	return true;
}

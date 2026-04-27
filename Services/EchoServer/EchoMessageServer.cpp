#include "EchoMessageServer.h"

#include "../../../../Module/IOCPNetworkEngine/Job/Job.h" 
#include "../../../../Module/IOCPNetworkEngine/HandlerTable/PacketHandlerTable.h" 
#include "../../../../Module/IOCPNetworkEngine/Scheduler/ReadySessionQueue.h" 

#include "../../../../Module/IOCPNetworkEngine/Memory/SlabMemoryPoolHelper.h" 

#include "../../../../Module/IOCPNetworkEngine/Protocol/PacketID.h" 

#include "../../../../Module/IOCPNetworkEngine/Session/SessionJobQueue.h" 
#include "../../../../Module/IOCPNetworkEngine/Session/ClientSession.h" // for ClientSession

#include "./PacketHandler/EchoHandler_Server.h"

using namespace Core::Util;

EchoMessageServer::EchoMessageServer()
{

}

EchoMessageServer::~EchoMessageServer()
{
	StopServer();
}

bool EchoMessageServer::StartServer(const char* ipAddress, const uint16_t port, const uint32_t maxConnectionCount)
{
	if (!IOCPServer::StartServer(ipAddress, port, maxConnectionCount))
		return false;

	bool registerResult = true;

	registerResult &= EchoMessage::Server::RegisterHandlers(GetPacketHandlerTable());

	return registerResult;
}

void EchoMessageServer::StopServer()
{
	IOCPServer::StopServer();
}

void EchoMessageServer::OnClientConnect(ISession* session)
{

}

void EchoMessageServer::OnClientDisconnect(ISession* session)
{

}

void EchoMessageServer::OnReceive(ISession* session, uint16_t packetId, const char* packetData, uint32_t packetSize)
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

	if (wasEmpty && enqueueSucceeded)
	{
		// JobQueue 가 비어있었다면 ReadySessionQueue 에 Session 을 스케줄링 하기 위해 Push 해주어야 한다.
		// 중복으로 Session 을 Push 하는 것을 방지 하기 위해 Processing 플래그를 Atomic 하게 검사.
		if (clientSession->IsProcessingReady())
		{
			// Session 이 Idle 상태인 경우
			// ReadySessionQueue 에 Push 하자!
			if (!GetReadySessionQueue()->Push(clientSession))
			{
				// Push 실패한 경우 플래그를 원상 복구하고 로그 처리 하자.
				clientSession->UpdateProcessingFlag(0);
			}
		}
		else
		{
			// 이미 Session 이 Processing 중 일 때
		}
	}
	else
	{
		if (!enqueueSucceeded)
		{
			// EnqueueJob 에 실패한 경우
			// log...
		}

		if (enqueueSucceeded && !wasEmpty)
		{
			// 이미 Session 이 Processing 중 일 때
		}
	}
}

void EchoMessageServer::OnSend(ISession* session, uint32_t bytesTransferred)
{
	ClientSession* clientSession = dynamic_cast<ClientSession*>(session);

	if (!clientSession)
	{
		__debugbreak();

		return;
	}
}

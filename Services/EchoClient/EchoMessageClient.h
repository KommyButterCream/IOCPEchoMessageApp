#pragma once

#include "../../Modules/IOCPEngine/Core/IOCPClient.h" // for IOCPClient

#ifdef BUILD_IOCP_ECHO_CLIENT_DLL
#define IOCP_ECHO_CLIENT_API __declspec(dllexport)
#else
#define IOCP_ECHO_CLIENT_API __declspec(dllimport)
#endif

class ISession;

class IOCP_ECHO_CLIENT_API EchoMessageClient : public IOCPClient
{
public:
	EchoMessageClient();
	~EchoMessageClient() override;

public:
	bool StartClient(const char* ipAddress, const uint16_t port);
	void StopClient();

private:
	void OnClientConnect(ISession* session) override;
	void OnClientDisconnect(ISession* session) override;
	void OnReceive(ISession* session, uint16_t packetId, const char* packetData, uint32_t packetSize) override;
	void OnSend(ISession* session, uint32_t bytesTransferred) override;

	// Service Logic
public:
	// 서버로 에코 메시지 전송
	bool SendChatMessage(const char* message);

private:
	bool EnqueueSendPacket(void** packetData, uint32_t packetSize);

private:
	uint32_t m_requestSequence;
};

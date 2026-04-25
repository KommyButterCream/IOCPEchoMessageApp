#pragma once

#include "../../Modules/IOCPEngine/Core/IOCPServer.h" // for IOCPServer

#ifdef BUILD_IOCP_ECHO_SERVER_DLL
#define IOCP_ECHO_SERVER_API __declspec(dllexport)
#else
#define IOCP_ECHO_SERVER_API __declspec(dllimport)
#endif

class ISession;

class IOCP_ECHO_SERVER_API EchoMessageServer : public IOCPServer
{
public:
	EchoMessageServer();
	~EchoMessageServer() override;

public:
	bool StartServer(const char* ipAddress, const uint16_t port, const uint32_t maxConnectionCount);
	void StopServer();

private:
	void OnClientConnect(ISession* session) override;
	void OnClientDisconnect(ISession* session) override;
	void OnReceive(ISession* session, uint16_t packetId, const char* packetData, uint32_t packetSize) override;
	void OnSend(ISession* session, uint32_t bytesTransferred) override;
};
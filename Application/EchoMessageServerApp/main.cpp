#include <stdio.h>

#include "../../Services/EchoServer/EchoMessageServer.h"

int main()
{
	EchoMessageServer* server = new EchoMessageServer;
	const char* serverIpAddress = "127.0.0.1";
	const uint16_t serverPort = 12500;

	if (!server->StartServer(serverIpAddress, serverPort, 10))
	{
		delete server;
		server = nullptr;

		return -1;
	}

	char szBuffer[2048] = { 0 };
	while (true)
	{
		gets_s(szBuffer);
		if (strcmp(szBuffer, "exit") == 0) break;
	}

	delete server;
	server = nullptr;

	return 0;
}
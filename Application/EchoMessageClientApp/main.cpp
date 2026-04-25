#include <stdio.h>
#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <atomic>
#include <thread>

#include "../../Services/EchoClient/EchoMessageClient.h"

std::atomic<bool> g_KeepRunning(true);

void SingleClientEchoTest();
void MultiClientEchoTest(size_t clientCount);


int main()
{
	SingleClientEchoTest();
	//MultiClientEchoTest(10);

	return 0;
}

void SingleClientEchoTest()
{
	EchoMessageClient* pClient = new EchoMessageClient;
	const char* serverIpAddress = "127.0.0.1";
	const uint16_t serverPort = 12500;

	if (!pClient->StartClient(serverIpAddress, serverPort))
	{
		delete pClient;
		pClient = nullptr;

		return;
	}

	char szBuffer[2048] = { 0 };
	while (true)
	{
		gets_s(szBuffer);
		if (strcmp(szBuffer, "exit") == 0) break;
		else if (strlen(szBuffer) > 0)
		{
			bool bSent = pClient->SendChatMessage(szBuffer);

			if (!bSent)
			{
				printf_s("CLIENT ERROR : Failed to Send Message To Server\n");
			}
		}
	}

	delete pClient;
	pClient = nullptr;
}

void MultiClientEchoTest(size_t clientCount)
{
	const char* serverIpAddress = "127.0.0.1";
	const uint16_t serverPort = 12500;

	std::vector<EchoMessageClient*> clients;
	std::vector<std::thread> workerThreads;

	for (size_t i = 0; i < clientCount; i++)
	{
		EchoMessageClient* client = new EchoMessageClient();
		if (client->StartClient(serverIpAddress, serverPort))
		{
			clients.push_back(client);
		}
		else
		{
			delete client;
			client = nullptr;
		}
	}

	for (size_t i = 0; i < clients.size(); i++)
	{
		workerThreads.emplace_back([client = clients[i], i]() {
			const char* msg = "hello";
			int count = 0;

			while (g_KeepRunning)
			{
				if (!client->SendChatMessage(msg))
				{
					__debugbreak();
					break;
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			});
	}

	std::string command;
	while (std::cin >> command)
	{
		if (command == "exit")
		{
			g_KeepRunning = false;
			break;
		}
	}

	for (auto& thread : workerThreads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}

	for (auto client : clients)
	{
		client->StopClient();
		delete client;
	}
}

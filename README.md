# IOCPEchoMessageApp
Sample echo message application built on top of `Core` and `IOCPNetworkEngine`.

# Info
`IOCPEchoMessageApp` is a Windows C++ sample project that integrates the `Core` library and the `IOCPNetworkEngine` network engine as sibling repositories/projects.

This repository provides:
- Echo message server application
- Echo message client application
- Shared service/protocol layer for client-server communication
- Integrated solution structure for building and testing the full stack together

# Features
- Echo server/client example built on IOCP
- Integration with shared `Core` and `IOCPNetworkEngine` projects
- Shared protocol definitions for packet-based communication
- Separate application and service layer structure
- Visual Studio solution for end-to-end build and test

# Dependencies
- Core
- IOCPNetworkEngine
- Windows WinSock2 / MSWSock
- C++20
- MSVC (Visual Studio 2022)

# Build Environment
- Windows 10/11 x64
- Visual Studio 2022
- MSVC toolset
- C++20

# Project Structure
- `Application/EchoMessageServerApp/` : executable entry project for the echo server
- `Application/EchoMessageClientApp/` : executable entry project for the echo client
- `Services/EchoServer/` : echo server service logic
- `Services/EchoClient/` : echo client service logic
- `Services/EchoProtocol/` : shared packet/protocol definitions
- `IOCPEchoMessageApp/` : shared application-level project

# Repository Layout
This project expects `IOCPEchoMessageApp`, `Core`, and `IOCPNetworkEngine` to be placed under the same parent directory.

Example:
```text
Module/
+-- Core/
+-- IOCPNetworkEngine/
+-- IOCPEchoMessageApp/
```

The Visual Studio solution references:
- `../Core/Core/Core.vcxproj`
- `../IOCPNetworkEngine/IOCPNetworkEngine/IOCPNetworkEngine.vcxproj`

# Clone
- Clone this repository:
```bash
git clone https://github.com/KommyButterCream/IOCPEchoMessageApp.git
```

Also make sure the `Core` and `IOCPNetworkEngine` repositories are cloned as sibling directories under the same parent directory.

# Build
1. Open `IOCPEchoMessageApp.sln` in Visual Studio 2022
2. Select `Debug | x64` or `Release | x64`
3. Build the solution

# Run
1. Start `EchoMessageServerApp`
2. Start `EchoMessageClientApp`
3. Check that messages sent from the client are echoed back by the server

# Purpose
This project is intended as:
- an integration example of `Core` and `IOCPNetworkEngine`
- a sample echo client/server application
- a base project for further network application development

# Notes
- The shared `Core` library and `IOCPNetworkEngine` are managed as sibling repositories/projects.
- Open `IOCPEchoMessageApp.sln` with Visual Studio 2022.
- Build the x64 configuration before running the server/client applications.

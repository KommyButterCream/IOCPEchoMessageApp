# IOCPEchoMessageApp
Sample echo message application built on top of `Core` and `IOCPNetworkEngine`.

# Info
`IOCPEchoMessageApp` is a Windows C++ sample project that integrates the `Core` library and the `IOCPNetworkEngine` network engine as Git submodules.

This repository provides:
- Echo message server application
- Echo message client application
- Shared service/protocol layer for client-server communication
- Integrated solution structure for building and testing the full stack together

# Features
- Echo server/client example built on IOCP
- `Core` and `IOCPNetworkEngine` integrated as submodules
- Shared protocol definitions for packet-based communication
- Separate application and service layer structure
- Visual Studio solution for end-to-end build and test

# Dependencies
- [Core](./Modules/Core)
- [IOCPNetworkEngine](./Modules/IOCPEngine)
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
- `Modules/Core/` : Core library submodule
- `Modules/IOCPEngine/` : IOCP network engine submodule

# Notes
- This repository depends on Git submodules.
- Make sure submodules are initialized before opening the solution.
- `IOCPNetworkEngine` itself also uses `Modules/Core` internally, so recursive submodule initialization is recommended.
- The solution is intended for Windows-based development with Visual Studio 2022.

# Clone
- Clone with submodules:
```bash
git clone --recurse-submodules https://github.com/KommyButterCream/IOCPEchoMessageApp.git
```
- If already cloned without submodules:
```bash
git submodule update --init --recursive
```
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

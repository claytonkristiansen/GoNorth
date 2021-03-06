// NorthernLights.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace GoNorth;

//------------------------------------------------------------------------------------------------
int NetReceiveData(void *pBuffer, CSimpleSocket &socket)
{
    int size = 0;
    if (socket.Receive(reinterpret_cast<char *>(&size), sizeof(size), 0) < 0)
    {
        return -1;
    }

    if (size > 0)
    {
        if (socket.Receive(reinterpret_cast<char *>(pBuffer), size, 0) < 0)
        {
            return -1;
        }
    }

    return size;
}

//------------------------------------------------------------------------------------------------
int NetSendData(const void *pBuffer, int size, CSimpleSocket &socket)
{
    if (socket.Send(reinterpret_cast<const char *>(&size), sizeof(size), 0) < 0)
    {
        return -1;
    }

    if (size > 0)
    {
        if (socket.Send(reinterpret_cast<const char *>(pBuffer), size, 0) < 0)
        {
            return -1;
        }
    }

    return size;
}

//------------------------------------------------------------------------------------------------
std::string ReadSocketString(CSimpleSocket &readSocket)
{
    // Receive input string
    char buffer[4096];
    int size = NetReceiveData(buffer, readSocket);

    return std::string(buffer, size);
}

//------------------------------------------------------------------------------------------------
void WriteSocketString(const std::string &string, CSimpleSocket &writeSocket)
{
    int size = NetSendData(string.c_str(), static_cast<int>(string.length()), writeSocket);
}

//------------------------------------------------------------------------------------------------
class CNorthernLightsGame : public GoNorth::CGame
{
    CSimpleSocket m_socket;
    CSimpleSocket m_acceptSocket;
    std::ostringstream m_storedOutput;

public:
    void Setup(const char *pPort)
    {
        WSADATA wsaData;

        // Initialize Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            CGame::Game().LogStream() << "WSAStartup failed: " << WSAGetLastError() << "\n";
        }

        CSimpleSocket socket;
        int result = socket.Bind(pPort);
        if (result != 0)
        {
            CGame::Game().LogStream() << "ERROR: Bind failed with error: " << result << "\n";
            return;
        }

        // Listen for a connection
        result = socket.Listen();
        if (result != 0)
        {
            CGame::Game().LogStream() << "ERROR: Listen failed with error: " << result << "\n";
            return;
        }

        m_socket = std::move(socket);
    }

    virtual ~CNorthernLightsGame()
    {
        m_acceptSocket.Close();
        m_socket.Close();
        WSACleanup();
    }

    void BeginFrame()
    {
        // Wait for a client to connect...
        int result = m_acceptSocket.Accept(m_socket);
        if (result != 0)
        {
            LogStream() << "ERROR accept failed with error: " << result << "\n";
        }

        // Read player id string...
        std::string playerId = ReadSocketString(m_acceptSocket);
    }

    void EndFrame()
    {
        // Flush stored output...
        FlushOutput();

        // Close the connection
        m_acceptSocket.Close();
    }

    void FlushOutput()
    {
        WriteSocketString(m_storedOutput.str(), m_acceptSocket);
        m_storedOutput = std::ostringstream(); // Reset the stored output stream
    }

    virtual std::string ReadInput() override
    {
        // Flush stored output...
        FlushOutput();

        // Read intput...
        return ReadSocketString(m_acceptSocket);
    }

    virtual void WriteOutput(const char *pOutputString, int stringLength) override
    {
        m_storedOutput << pOutputString;
    }

    void Initialize() override
    {
        __super::Initialize();

        size_t room = MakeLocation<CStaticLocation>("You are in a room with a door on the South wall.");
        GetPlayer(0)->SetLocationId(room);

        OutputStream() << "Welcome to NorthernLights\n";
    }
};

class CNorthernLightsClient
{
    CSimpleSocket m_socket;
    const char *m_pServer = nullptr;
    const char *m_pPort = nullptr;

public:
    ~CNorthernLightsClient()
    {
        Cleanup();
    }

    void Setup(const char *pServer, const char *pPort)
    {
        WSADATA wsaData;

        // Initialize Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            CGame::Game().LogStream() << "WSAStartup failed: " << WSAGetLastError() << "\n";
        }

        m_pServer = pServer;
        m_pPort = pPort;
    }

    void Cleanup()
    {
        m_socket.Close();
        WSACleanup();
    }

    void Run()
    {
        for (;;)
        {
            // Connect to server...
            DWORD dTime = 0;
            DWORD startTime = timeGetTime();
            int result;
            for (result = m_socket.Connect(m_pServer, m_pPort); dTime < 15000 && result != 0; result = m_socket.Connect(m_pServer, m_pPort))
            {
                Sleep(0);
                DWORD time = timeGetTime();
                dTime = time - startTime;
            }

            if (result != 0)
            {
                printf("Client: Connect attempt timed out: %d\n", result);
                continue;
            }

            // Send client id
            std::string connectString = "TestClient";
            WriteSocketString(connectString, m_socket);

            // Read stored server output...
            std::string response = ReadSocketString(m_socket);
            std::cout << response;

            // Wait for input...
            std::string messageString;
            std::getline(std::cin, messageString);

            // Send input to server
            WriteSocketString(messageString, m_socket);

            // Wait for response...
            response = ReadSocketString(m_socket);
            std::cout << response;
        }
    }
};

//------------------------------------------------------------------------------------------------
int main(int argc, const char *argv[])
{
    std::unique_ptr<CNorthernLightsGame> pGame;
    std::unique_ptr<CNorthernLightsClient> pClient;

    const char *pServerName = nullptr;
    const char *pPort = nullptr;

    // Process command line arguments...
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        std::transform(arg.begin(), arg.end(), arg.begin(), ::tolower);

        if (std::string("-server") == arg)
        {
            // Run northern lights in server mode
            // Next argument should be the port number
            if (++i == argc)
            {
                return -1;
            }

            pPort = argv[i];
        }
        else if (std::string("-client") == arg)
        {
            // Run northern lights in client mode

            // Next argument is server name
            if (++i == argc)
            {
                return -1;
            }

            pServerName = argv[i];

                // Next argument is port number
            if (++i == argc)
            {
                return -1;
            }

            pPort = argv[i];
        }
    }

    if (pPort)
    {
        // Run over network
        if (pServerName)
        {
            // Run as client...
            pClient = std::make_unique<CNorthernLightsClient>();
            pClient->Setup(pServerName, pPort);
            pClient->Run();
        }
        else
        {
            // Run as server...
            pGame = std::make_unique<CNorthernLightsGame>();
            pGame->Setup(pPort);
            pGame->Run();
        }
    }

    return 0;
}


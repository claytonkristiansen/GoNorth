//------------------------------------------------------------------------------------------------
// Game.cpp
//
// Implementation of CGame class
//------------------------------------------------------------------------------------------------

#include "stdafx.h"

namespace GoNorth
{
    CGame *CGame::s_pThis = nullptr;

    //------------------------------------------------------------------------------------------------
    DIRECTION GetReverseDirection(DIRECTION direction)
    {
        switch (direction)
        {
        case NORTH:
            return SOUTH;
        case NORTHEAST:
            return NORTHWEST;
        case EAST:
            return WEST;
        case SOUTHEAST:
            return NORTHWEST;
        case SOUTH:
            return NORTH;
        case SOUTHWEST:
            return NORTHEAST;
        case WEST:
            return EAST;
        case NORTHWEST:
            return SOUTHEAST;
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        }
        return NUMDIRECTIONS; // ERROR
    }

    //------------------------------------------------------------------------------------------------
    CGame::CGame()
    {
        if (s_pThis)
        {
            // Singleton class only
            throw(std::bad_alloc());
        }

        m_logStream.Open("gonorth.log");

        // Register words
        m_dictionary.AddWord("Help",        WORD_HELP);
        m_dictionary.AddWord("Exit",        WORD_EXIT);
        m_dictionary.AddWord("Use",         WORD_USE);
        m_dictionary.AddWord("Go",          WORD_GO);
        m_dictionary.AddWord("Get",         WORD_GET);
        m_dictionary.AddWord("Drop",        WORD_DROP);
        m_dictionary.AddWord("Move",        WORD_MOVE);
        m_dictionary.AddWord("Look",        WORD_LOOK);
        m_dictionary.AddWord("North",       WORD_NORTH);
        m_dictionary.AddWord("Northeast",   WORD_NORTHEAST);
        m_dictionary.AddWord("East",        WORD_EAST);
        m_dictionary.AddWord("Southeast",   WORD_SOUTHEAST);
        m_dictionary.AddWord("South",       WORD_SOUTH);
        m_dictionary.AddWord("Southwest",   WORD_SOUTHWEST);
        m_dictionary.AddWord("West",        WORD_WEST);
        m_dictionary.AddWord("Northwest",   WORD_NORTHWEST);
        m_dictionary.AddWord("Up",          WORD_UP);
        m_dictionary.AddWord("Down",        WORD_DOWN);
        m_dictionary.AddWord("Yes",         WORD_YES);
        m_dictionary.AddWord("No",          WORD_NO);
        m_dictionary.AddWord("Inventory",   WORD_INVENTORY);

        // Aliases
        m_dictionary.AddWord("?",           WORD_HELP);
        m_dictionary.AddWord("Quit",        WORD_EXIT);
        m_dictionary.AddWord("Q",           WORD_EXIT);
        m_dictionary.AddWord("N",           WORD_NORTH);
        m_dictionary.AddWord("NE",          WORD_NORTHEAST);
        m_dictionary.AddWord("E",           WORD_EAST);
        m_dictionary.AddWord("SE",          WORD_SOUTHEAST);
        m_dictionary.AddWord("S",           WORD_SOUTH);
        m_dictionary.AddWord("SW",          WORD_SOUTHWEST);
        m_dictionary.AddWord("W",           WORD_WEST);
        m_dictionary.AddWord("NW",          WORD_DOWN);
        m_dictionary.AddWord("I",           WORD_INVENTORY);
        m_dictionary.AddWord("Y",           WORD_YES);
        m_dictionary.AddWord("N",           WORD_NO);

        // Add default player and select
        m_selectedPlayerId = MakePlayer<CPlayer>();

        // Set the default prompt string
        m_promptString = "> ";

        s_pThis = this;
    }

    //------------------------------------------------------------------------------------------------
    CGame::~CGame()
    {

    }

    //------------------------------------------------------------------------------------------------
    CLocation *CGame::GetLocation(LocationId locationId)
    {
        return locationId >= 0 && locationId < m_locations.size() ? m_locations[locationId].get() : nullptr;
    }

    //------------------------------------------------------------------------------------------------
    CPlayer *CGame::GetPlayer(PlayerId playerId)
    {
        return playerId >= 0 && playerId < m_players.size() ? m_players[playerId].get() : nullptr;
    }

    //------------------------------------------------------------------------------------------------
    void CGame::SetLocationNeighborId(LocationId locationId, LocationId neighborId, DIRECTION direction, bool biDirectional)
    {
        m_locations[locationId]->SetNeighborId(direction, neighborId);
        if (neighborId != -1)
        {
            // Set the neighbor return path
            m_locations[neighborId]->SetNeighborId(GetReverseDirection(direction), locationId);
        }
    }

    //------------------------------------------------------------------------------------------------
    std::string CGame::ReadInput()
    {
        // The default implementation is to wait for console input from stdin
        std::string inputString;
        std::getline(std::cin, inputString);
        return inputString;
    }

    //------------------------------------------------------------------------------------------------
    void CGame::WriteOutput(const char *outputString, int /*stringLength*/)
    {
        // The default implementation is to write to stdout
        std::cout << outputString;
    }

    //------------------------------------------------------------------------------------------------
    void CGame::Run()
    {
        Initialize();

        // Main loop
        for (; m_exit == false;)
        {
            BeginFrame();

            m_outputStream << "\n" << m_promptString;
            std::string commandString = ReadInput();
            m_logStream << commandString;
            m_outputStream << "\n";

            CCommand command;
            if (CCommandBuilder::Tokenize(m_dictionary, commandString, command))
            {
                CPlayer *pPlayer = GetPlayer(m_selectedPlayerId);

                // First let the player handle the command
                if(!pPlayer->DoCommand(command))
                {
                    if (!DoCommand(pPlayer, command))
                    {
                        m_outputStream << "That can't be done.\n";
                    }
                }
            }

            EndFrame();
        }

        Cleanup();
    }

    //------------------------------------------------------------------------------------------------
    bool CGame::DoCommand(CPlayer *pPlayer, const CCommand &command)
    {
        // Parse the command...
        if (command.IsMatch(WORD_HELP))
        {
            PrintHelp();
            return true;
        }

        if (command.IsMatch(WORD_EXIT))
        {
            m_exit = true;
            return true;
        }

        return false;
    }

    //------------------------------------------------------------------------------------------------
    PlayerId CGame::SelectPlayer(PlayerId playerId)
    {
        m_selectedPlayerId = playerId >= 0 && playerId < m_players.size() ? playerId : -1;
        CPlayer *pPlayer = GetPlayer(m_selectedPlayerId);

        if (pPlayer)
        {
            CLocation *pLocation = CGame::Game().GetLocation(pPlayer->GetLocationId());
            if (pLocation)
            {
                pLocation->DoLook();
            }
        }

        return m_selectedPlayerId;
    }

    //------------------------------------------------------------------------------------------------
    void CGame::PrintHelp()
    {
        m_outputStream << "No help for you yet\n";
    }

    //------------------------------------------------------------------------------------------------
    WordId CGame::MakeItem(WordId wordId, const std::string &inventoryDescription, const std::string &locationDescription)
    {
        try
        {
            m_items.emplace(wordId, std::make_unique<CItem>(wordId, inventoryDescription, locationDescription)); // throw(std::bad_alloc)
        }
        catch (std::bad_alloc &)
        {
            return 0UL - 1;
        }

        return wordId;
    }
}
//------------------------------------------------------------------------------------------------
// Game.h
//
// Header file for GoNorth CGame class
//------------------------------------------------------------------------------------------------

#pragma once

namespace GoNorth
{
    //------------------------------------------------------------------------------------------------
    class CLogStream
    {
        std::ofstream m_fileStream;

    public:
        CLogStream() = default;

        void Open(const char *pFilename)
        {
            try
            {
                m_fileStream.open(pFilename, std::ios_base::out | std::ios_base::trunc);
            }
            catch (std::ios_base::failure &)
            {
                assert(!"File open failed");
            }
        }

        template<typename _T>
        CLogStream &operator << (const _T &v)
        {
            // If a file stream is active then write the output to the file
            if (m_fileStream.is_open())
            {
                m_fileStream << v;
            }

            // Write to the debug output
            std::ostringstream temp;
            temp << v;
            OutputDebugStringA(temp.str().c_str());

            return *this;
        }
    };

    //------------------------------------------------------------------------------------------------
    class COutputStream
    {
    public:
        COutputStream() = default;

        template<typename _T>
        COutputStream &operator << (const _T &v)
        {
            std::ostringstream stringStream;
            stringStream << v;

            CGame::Game().WriteOutput(stringStream.str().c_str(), static_cast<int>(stringStream.str().length()));

            return *this;
        }
    };

    //------------------------------------------------------------------------------------------------
    // Base class for the main GoNorth game object.  This is a singleton class,
    // meaning that only one object of type CGame can esist in the process.
    class CGame : public CCommandInterpreter
    {
        // Container of indexable game locations
        std::vector<std::unique_ptr<CLocation>> m_locations;
        std::vector<std::unique_ptr<CPlayer>> m_players;
        std::map<WordId, std::unique_ptr<CItem>> m_items;

        // Set to true when the player has opted to exit
        bool m_exit = false;

        CDictionary m_dictionary;

        static CGame *s_pThis; // This is a singleton class

        COutputStream m_outputStream; // Wraps output streams (e.g. stdout and/or file)
        CLogStream m_logStream;
        PlayerId m_selectedPlayerId;

        std::string m_promptString;

    public:
        CGame();
        virtual ~CGame();

        // Accessors
        PlayerId GetSelectedPlayerId() const { return m_selectedPlayerId; }
        CDictionary &Dictionary() { return m_dictionary; }
        CLocation *GetLocation(LocationId locationId);
        CPlayer *GetPlayer(PlayerId playerId);
        COutputStream &OutputStream() { return m_outputStream; }
        CLogStream &LogStream() { return m_logStream; }
        CItem *GetItem(WordId itemWordId)
        {
            CItem *pItem = nullptr;
            auto it = m_items.find(itemWordId);
            if (it != m_items.end())
            {
                pItem = it->second.get();
            }
            return pItem;
        }

        static CGame &Game() { return *s_pThis; }

        void SetLocationNeighborId(LocationId locationId, LocationId neighborId, DIRECTION direction, bool biDirectional);

        void SetPrompt(const char *promptString)
        {
            m_promptString = promptString;
        }

        //
        // CGame overridables
        //

        // Executes the main game loop
        virtual void Run();

        // Override this to perform additional game initialization
        virtual void Initialize() {}

        // Override this to perform cleanup
        virtual void Cleanup() {}

        // Reads input.
        // Does not return until input is recieved.
        // Returns a string containing the input.
        virtual std::string ReadInput();

        // Write output
        virtual void WriteOutput(const char *outputString, int stringLength);

        // Start of a cycle in the main loop
        virtual void BeginFrame() {}

        // End of a cycle in the main loop
        virtual void EndFrame() {}

        // Sets the currently selected player id (for multiple players)
        // If successful returns playerId, otherwise -1.
        // Override this to handle custom player selection (e.g.
        // restricting the player selection to a list of available players in
        // multi-player scenario).
        virtual PlayerId SelectPlayer(PlayerId playerId);

        //
        // CCommandInterpreter overrides
        //

        // Responds to command.
        // Returns 'true' if the command is processed.
        virtual bool DoCommand(CPlayer *pPlayer, const CCommand &command) override;

        // Constructs and adds a location to the game. Returns the location index.
        // Uses a template with variadic arguments passed as the LocationClass
        // constructor parameters.
        template<class LocationClass, typename... Arguments>
        LocationId MakeLocation(Arguments... params)
        {
            try
            {
                LocationId locationId = m_locations.size();
                std::unique_ptr<CLocation> pLocation = std::make_unique<LocationClass>(LocationClass(params...)); // throw(std::bad_alloc)
                m_locations.emplace_back(std::move(pLocation)); // throw(std::bad_alloc)
                return locationId;
            }
            catch (std::bad_alloc)
            {
                return -1;
            }
        }

        template<class PlayerClass, typename... Arguments>
        PlayerId MakePlayer(Arguments... params)
        {
            try
            {
                PlayerId playerId = m_players.size();
                std::unique_ptr<CPlayer> pPlayer = std::make_unique<PlayerClass>(PlayerClass(params...)); // throw(std::bad_alloc)
                m_players.emplace_back(std::move(pPlayer)); // throw(std::bad_alloc)
                return playerId;
            }
            catch (std::bad_alloc)
            {
                return -1;
            }
        }

        WordId MakeItem(WordId wordId, const::std::string &inventoryDescription, const std::string &locationDescription);

        void PrintHelp();
    };
}

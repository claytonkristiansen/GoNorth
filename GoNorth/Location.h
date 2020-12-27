//------------------------------------------------------------------------------------------------
// Locastion.h
//
// Header file for GoNorth CLocation class
//------------------------------------------------------------------------------------------------

#pragma once

namespace GoNorth
{
    typedef size_t LocationId;

    //------------------------------------------------------------------------------------------------
    // Enum values representing compass directions and up/down.  
    // These are used to describe player movement between locations.
    enum DIRECTION
    {
        NORTH = 0,
        NORTHEAST,
        EAST,
        SOUTHEAST,
        SOUTH,
        SOUTHWEST,
        WEST,
        NORTHWEST,
        UP,
        DOWN,
        NUMDIRECTIONS,
        NO_DIRECTION = -1
    };

    extern const char *DirectionString(DIRECTION direction);

    //------------------------------------------------------------------------------------------------
    // Helper function that translates directional word ids to DIRECTION values
    inline DIRECTION WordIdToDirection(WordId wordId)
    {
        DIRECTION direction = NO_DIRECTION;

        switch (wordId)
        {
        case WORD_NORTH:
            direction = NORTH;
            break;
        case WORD_NORTHEAST:
            direction = NORTHEAST;
            break;
        case WORD_EAST:
            direction = EAST;
            break;
        case WORD_SOUTHEAST:
            direction = SOUTHEAST;
            break;
        case WORD_SOUTH:
            direction = SOUTH;
            break;
        case WORD_SOUTHWEST:
            direction = SOUTHWEST;
            break;
        case WORD_WEST:
            direction = WEST;
            break;
        case WORD_NORTHWEST:
            direction = NORTHWEST;
            break;
        case WORD_UP:
            direction = UP;
            break;
        case WORD_DOWN:
            direction = DOWN;
            break;
        }

        return direction;
    }

    //------------------------------------------------------------------------------------------------
    // Base class for a GoNorth game location.
    // GoNorth client applications can use define CLoation
    // classes.
    class CLocation : public CCommandInterpreter
    {
        LocationId m_neighborIds[NUMDIRECTIONS];
        CInventory m_inventory;

    public:
        CLocation()
        {
            for (auto &index : m_neighborIds)
            {
                index = -1;
            }
        }
        virtual ~CLocation() {}

        //
        // Accessors
        //
        void SetNeighborId(DIRECTION direction, LocationId neighborIndex);
        LocationId GetNeighborId(DIRECTION direction) const { return m_neighborIds[direction]; }

        CInventory &Inventory() { return m_inventory; }

        //
        // Overridable methods
        //

        // Respond to player attempt to move in the given direction.
        // Returns 'true' if the move is allowed.
        virtual bool DoGoDirection(DIRECTION direction);

        // Respond to 'look' command
        virtual void DoLook();

        // Respond to player entering the room
        virtual void DoEnter(LocationId previousLocationId)
        {
            // By default just call DoLook()
            DoLook();
        }

    };

    //------------------------------------------------------------------------------------------------
    // Immutable CLocation-derived class.
    // A static location carries no state and always gives
    // the same description.
    class CStaticLocation : public CLocation
    {
        std::string m_description;

    public:
        CStaticLocation(const std::string &description) :
            m_description(description),
            CLocation()
        {
        }

        virtual void DoLook();
    };
}
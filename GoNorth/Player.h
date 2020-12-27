//------------------------------------------------------------------------------------------------
// Player.h
//
// Header file for GoNorth CPlayer class
//------------------------------------------------------------------------------------------------

#pragma once

namespace GoNorth
{
    // Forward declarations
    class CCommand;

    // Typedefs
    typedef size_t PlayerId;

    //------------------------------------------------------------------------------------------------
    class CPlayer
    {
        // Player location
        LocationId m_locationId;

        // Player inventory
        CInventory m_inventory;

    public:
        // Accessors
        LocationId GetLocationId() { return m_locationId; }
        void SetLocationId(LocationId locationId, LocationId fromLocationId = -1);
        CInventory &Inventory() { return m_inventory; }

        // Overridable
        virtual bool DoCommand(const CCommand &command);

        // Attempts to move the player to an adjacent location.
        // Returns 'false' if the direction is not available.
        virtual bool DoGoDirection(DIRECTION direction);

        // Invokes the player location DoLook.  Override to perform additional 
        // Player-focused DoLook behavior;
        virtual void DoLook();
    };
}
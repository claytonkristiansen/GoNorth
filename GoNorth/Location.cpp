#include "stdafx.h"

namespace GoNorth
{
    void CLocation::DoLook()
    {
        // List inventory items
        m_inventory.PrintItemsInLocation();
    }

    //------------------------------------------------------------------------------------------------
    void CLocation::SetNeighborId(DIRECTION direction, LocationId locationId)
    {
        m_neighborIds[direction] = locationId;
    }

    //------------------------------------------------------------------------------------------------
    bool CLocation::DoGoDirection(DIRECTION direction)
    {
        if (m_neighborIds[direction] == -1)
        {
            CGame::Game().OutputStream() << "You can't go that way.\n";
            return false;
        }

        return true;
    }

    void CStaticLocation::DoLook()
    {
        CGame::Game().OutputStream() << m_description << "\n";
        __super::DoLook();
    }


    //------------------------------------------------------------------------------------------------
    const char *DirectionString(DIRECTION direction)
    {
        switch (direction)
        {
        case NORTH:
            return "North";
        case NORTHEAST:
            return "Northeast";
        case EAST:
            return "East";
        case SOUTHEAST:
            return "Southeast";
        case SOUTH:
            return "South";
        case SOUTHWEST:
            return "Southwest";
        case WEST:
            return "West";
        case NORTHWEST:
            return "Northwest";
        case UP:
            return "Up";
        case DOWN:
            return "Down";
        default:
            return "None";
        }
    }

}
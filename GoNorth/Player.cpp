//------------------------------------------------------------------------------------------------
// Player.cpp
//
// Implements GoNorth CPlayer class
//------------------------------------------------------------------------------------------------

#include "stdafx.h"

namespace GoNorth
{
    bool CPlayer::DoCommand(const CCommand &command)
    {
        if (command.IsMatch(WORD_LOOK))
        {
            DoLook();
            return true;
        }

        if (command.IsMatch(WORD_INVENTORY))
        {
            if (Inventory().GetItemCount() > 0)
            {
                CGame::Game().OutputStream() << "You have the following:\n";
                Inventory().PrintItemsInInventory();
            }
            else
            {
                CGame::Game().OutputStream() << "You have nothing.\n";
            }
            return true;
        }

        // GO ...
        if (command[0] == WORD_GO || command[0] == WORD_MOVE)
        {
            auto NoGo = [this]()
            {
                CGame::Game().OutputStream() << "You can only try to go one of the following directions:\n";
                CGame::Game().OutputStream() << " North\n";
                CGame::Game().OutputStream() << " Northeast\n";
                CGame::Game().OutputStream() << " East\n";
                CGame::Game().OutputStream() << " Southeast\n";
                CGame::Game().OutputStream() << " South\n";
                CGame::Game().OutputStream() << " Southwest\n";
                CGame::Game().OutputStream() << " West\n";
                CGame::Game().OutputStream() << " Northwest\n";
                CGame::Game().OutputStream() << " Up\n";
                CGame::Game().OutputStream() << " Down\n";
            };

            if (command.Length() == 1)
            {
                CGame::Game().OutputStream() << "Go where?\n";
            }
            else if (command.Length() > 2)
            {
                NoGo();
            }
            else
            {
                DIRECTION direction = WordIdToDirection(command[1]);
                if (direction == NO_DIRECTION)
                {
                    NoGo();
                }
                else
                {
                    DoGoDirection(direction);
                }
            }
        }

        if (command.IsMatch(WORD_NORTH))
        {
            DoGoDirection(NORTH);
            return true;
        }

        if (command.IsMatch(WORD_NORTHEAST))
        {
            DoGoDirection(NORTHEAST);
            return true;
        }

        if (command.IsMatch(WORD_EAST))
        {
            DoGoDirection(EAST);
            return true;
        }

        if (command.IsMatch(WORD_SOUTHEAST))
        {
            DoGoDirection(SOUTHEAST);
            return true;
        }

        if (command.IsMatch(WORD_SOUTH))
        {
            DoGoDirection(SOUTH);
            return true;
        }

        if (command.IsMatch(WORD_SOUTHWEST))
        {
            DoGoDirection(SOUTHWEST);
            return true;
        }

        if (command.IsMatch(WORD_WEST))
        {
            DoGoDirection(WEST);
            return true;
        }

        if (command.IsMatch(WORD_NORTHWEST))
        {
            DoGoDirection(NORTHWEST);
            return true;
        }

        if (command.IsMatch(WORD_UP))
        {
            DoGoDirection(UP);
            return true;
        }

        if (command.IsMatch(WORD_DOWN))
        {
            DoGoDirection(DOWN);
            return true;
        }

        CLocation *pLocation = CGame::Game().GetLocation(m_locationId);

        switch (command[0])
        {
        case WORD_GET:
            switch (command.Length())
            {
            case 1:
                CGame::Game().OutputStream() << "Get what?\n";
                break;
            case 2:
                if(pLocation)
                {
                    if (pLocation->Inventory().HasItem(command[1]))
                    {
                        // Move the item from the location inventory to the player inventory
                        pLocation->Inventory().RemoveItem(command[1]);
                        Inventory().AddItem(command[1]);
                        CGame::Game().OutputStream() << "Getting " << ToLower(CGame::Game().Dictionary().GetWordString(command[1])) << "\n";
                    }
                    else
                    {
                        CGame::Game().OutputStream() << "Where do I get that?\n";
                    }
                }
                else
                {
                    CGame::Game().OutputStream() << "There is nothing at your location.\n";
                }
                break;
            default:
                CGame::Game().OutputStream() << "Can only get one thing at a time.\n";
                break;
            }
            return true;
            break;

        case WORD_DROP:
            switch (command.Length())
            {
            case 1:
                CGame::Game().OutputStream() << "Get what?\n";
                break;
            case 2:
                if(pLocation)
                {
                    pLocation->Inventory();
                    if (Inventory().HasItem(command[1]))
                    {
                        // Move the item from the player inventory to the location inventory
                        Inventory().RemoveItem(command[1]);
                        pLocation->Inventory().AddItem(command[1]);
                        CGame::Game().OutputStream() << "Dropping " << ToLower(CGame::Game().Dictionary().GetWordString(command[1])) << "\n";
                    }
                    else
                    {
                        CGame::Game().OutputStream() << "You don't have that.\n";
                    }
                }
                else
                {
                    CGame::Game().OutputStream() << "There is nowhere to drop anything.\n";
                }
                break;
            }
            return true;
            break;
        }

        // Give the player location a chance to process the command
        if (pLocation)
        {
            return pLocation->DoCommand(this, command);
        }

        return false;
    }

    void CPlayer::DoLook()
    {
        CLocation *pLocation = CGame::Game().GetLocation(m_locationId);

        if (pLocation)
        {
            pLocation->DoLook();
        }
        else
        {
            CGame::Game().OutputStream() << "Nothing to look at\n";
        }
    }

    //------------------------------------------------------------------------------------------------
    void CPlayer::SetLocationId(LocationId locationId, LocationId fromLocationId)
    {
        m_locationId = locationId; 
    }

    //------------------------------------------------------------------------------------------------
    bool CPlayer::DoGoDirection(DIRECTION direction)
    {
        LocationId fromLocationId = m_locationId;
        CLocation *pLocation = CGame::Game().GetLocation(m_locationId);

        if (pLocation->DoGoDirection(direction))
        {
            SetLocationId(pLocation->GetNeighborId(direction));

            pLocation = CGame::Game().GetLocation(m_locationId);
            if (pLocation)
            {
                pLocation->DoEnter(fromLocationId);
            }

            return true;
        }

        return false;
    }

}
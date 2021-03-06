// GoNorthGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace GoNorth;

enum MY_WORDS
{
    WORD_KEYS = CUSTOM_WORD, // Starting at the end of the standard GoNorth word ids
    WORD_OPEN,
    WORD_SAFE,
    WORD_MEDALLION,
    WORD_BECOME,
    WORD_BOB,
    WORD_BETTY,
};

class CSafeRoom : public CLocation
{
    bool m_isSafeOpen = false;
    WordId m_medallionId;

public:
    CSafeRoom()
    {
        // Create the medallion but do not yet add it to the room inventory
        m_medallionId = CGame::Game().MakeItem(WORD_MEDALLION, "Shiny Gold Medallion", "There is a shiny gold medallion here.");
    }

    virtual bool DoCommand(CPlayer *pPlayer, const CCommand &command)
    {
        if (command.IsMatch(WORD_OPEN, WORD_SAFE))
        {
            if (m_isSafeOpen)
            {
                CGame::Game().OutputStream() << "The safe is already open\n";
            }
            else
            {
                // Does the player have a key?
                if (pPlayer->Inventory().HasItem(WORD_KEYS))
                {
                    CGame::Game().OutputStream() << "You use one of the keys to open the safe. Inside is a shiny gold medallion.\n";
                    Inventory().AddItem(m_medallionId);
                    m_isSafeOpen = true;
                }
                else
                {
                    CGame::Game().OutputStream() << "The safe requires a key to open.\n";
                }
            }

            return true;
        }

        return false;
    }

    virtual void DoLook()
    {
        CGame::Game().OutputStream() << "You are in a an office with pictures of horses on the walls.\n";
        if (m_isSafeOpen)
        {
            CGame::Game().OutputStream() << "There is an open safe in the corner of the room.\n";
        }
        else
        {
            CGame::Game().OutputStream() << "There is a secure looking safe in the corner of the room.\n";
        }
        CGame::Game().OutputStream() << "To the South is the master bedroom.\n";
        
        __super::DoLook();
    }
};

class CMyPlayer : public CPlayer
{
};

class CMyGame : public CGame
{
    PlayerId m_bob;
    PlayerId m_betty;

public:
    virtual void Initialize()
    {
        //Initialize rooms and create map
        LocationId room = MakeLocation<CStaticLocation>("You are in a room with a door on the South wall.");
        LocationId hall = MakeLocation<CStaticLocation>("You are in a hallway that runs East and West.  There is a door to the North.");
        LocationId library = MakeLocation<CStaticLocation>("You are in a large room full of books with a door to the West.");
        LocationId kitchen = MakeLocation<CStaticLocation>("You are in a kitchen with a door to the East.");
        LocationId hall_1 = MakeLocation<CStaticLocation>("You are in a darkly lit hallway that runs East and West. There are doors to the North and South.");
        LocationId masterBedroom = MakeLocation<CStaticLocation>("You are in a fancy bedroom with a large bed and a door to the South and another room to the North.");
        LocationId broomCloset = MakeLocation<CStaticLocation>("You are in a broom closet you dolt, get out through a door to the North.");
        LocationId office = MakeLocation<CSafeRoom>();

        SetLocationNeighborId(room, hall, SOUTH, true);
        SetLocationNeighborId(hall, library, EAST, true);
        SetLocationNeighborId(hall, hall_1, WEST, true);
        SetLocationNeighborId(hall_1, kitchen, WEST, true);
        SetLocationNeighborId(hall_1, masterBedroom, NORTH, true);
        SetLocationNeighborId(hall_1, broomCloset, SOUTH, true);
        SetLocationNeighborId(masterBedroom, office, NORTH, true);

        // Bob is the default player
        m_bob = MakePlayer<CMyPlayer>();
        GetPlayer(m_bob)->SetLocationId(hall);
        SetPrompt("Bob> ");

        // Betty is a second player
        m_betty = MakePlayer<CMyPlayer>();
        GetPlayer(m_betty)->SetLocationId(kitchen);

        // Show location for first player
        SelectPlayer(m_bob);

        Dictionary().AddWord("Keys", WORD_KEYS);
        Dictionary().AddWord("Open", WORD_OPEN);
        Dictionary().AddWord("Safe", WORD_SAFE);
        Dictionary().AddWord("Medallion", WORD_MEDALLION);
        Dictionary().AddWord("Become", WORD_BECOME);
        Dictionary().AddWord("Bob", WORD_BOB);
        Dictionary().AddWord("Betty", WORD_BETTY);

        GetLocation(room)->Inventory().AddItem(MakeItem(WORD_KEYS, "Set of keys", "Someone left their keys here"));
    }

    virtual bool DoCommand(CPlayer *pPlayer, const CCommand &command) override
    {
        if (command[0] == WORD_BECOME)
        {
            switch (command[1])
            {
            case WORD_BOB:
                OutputStream() << "You are now Bob\n\n";
                SelectPlayer(m_bob);
                SetPrompt("Bob> ");
                return true;
                break;

            case WORD_BETTY:
                OutputStream() << "You are now Betty\n\n";
                SelectPlayer(m_betty);
                SetPrompt("Betty> ");
                return true;
                break;
            }
        }

        return __super::DoCommand(pPlayer, command);
    }

    //virtual bool DoGoDirection(DIRECTION direction)
    //{
    //    CGame::Game().OutputStream() << "Heading " << DirectionString(direction) << "\n";
    //    return __super::DoGoDirection(direction);
    //}
};

int main()
{
    CMyGame *pGame = new CMyGame();

    pGame->Run();

    delete(pGame);

    return 0;
}


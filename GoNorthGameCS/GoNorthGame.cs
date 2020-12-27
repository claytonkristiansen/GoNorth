using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoNorth;

namespace GoNorthGameCS
{
    enum MY_WORDS
    {
        WORD_KEYS = WORDS.CUSTOM_WORD, // Starting at the end of the standard GoNorth word ids
        WORD_OPEN,
        WORD_SAFE,
        WORD_MEDALLION,
        WORD_BECOME,
        WORD_BOB,
        WORD_BETTY,
    };

    public class SafeRoom : Location
    {
        bool m_isSafeOpen = false;
        int m_medallionId;

        public SafeRoom(Game game)
        {
            // Create the medallion but do not yet add it to the room inventory
            m_medallionId = game.AddItem(new Item((int)MY_WORDS.WORD_MEDALLION, "Shiny Gold Medallion", "There is a shiny gold medallion here."));
        }

        public override bool DoCommand(Game game, Player player, Command command)
        {
            if (command.IsMatch((int)MY_WORDS.WORD_OPEN, (int)MY_WORDS.WORD_SAFE))
            {
                if (m_isSafeOpen)
                {
                    game.WriteOutput("The safe is already open\n");
                }
                else
                {
                    // Does the player have a key?
                    if (player.Inventory.HasItem((int)MY_WORDS.WORD_KEYS))
                    {
                        game.WriteOutput("You use one of the keys to open the safe. Inside is a shiny gold medallion.\n");
                        Inventory.AddItem(m_medallionId);
                        m_isSafeOpen = true;
                    }
                    else
                    {
                        game.WriteOutput("The safe requires a key to open.\n");
                    }
                }

                return true;
            }

            return false;
        }

        public override void DoLook(Game game)
        {
            game.WriteOutput("You are in a an office with pictures of horses on the walls.\n");
            if (m_isSafeOpen)
            {
                game.WriteOutput("There is an open safe in the corner of the room.\n");
            }
            else
            {
                game.WriteOutput("There is a secure looking safe in the corner of the room.\n");
            }
            game.WriteOutput("To the South is the master bedroom.\n");

            base.DoLook(game);
        }
    };

    public class MyGame : Game
    {
        int m_bob;
        int m_betty;

        public override void Initialize()
        {
            //Initialize rooms and create map
            int room = AddLocation(new StaticLocation("You are in a room with a door on the South wall."));
            int hall = AddLocation(new StaticLocation("You are in a hallway that runs East and West.  There is a door to the North."));
            int library = AddLocation(new StaticLocation("You are in a large room full of books with a door to the West."));
            int kitchen = AddLocation(new StaticLocation("You are in a kitchen with a door to the East."));
            int hall_1 = AddLocation(new StaticLocation("You are in a darkly lit hallway that runs East and West. There are doors to the North and South."));
            int masterBedroom = AddLocation(new StaticLocation("You are in a fancy bedroom with a large bed and a door to the South and another room to the North."));
            int broomCloset = AddLocation(new StaticLocation("You are in a broom closet you dolt, get out through a door to the North."));
            int office = AddLocation(new SafeRoom(this));

            SetLocationNeighborId(room, hall, Direction.South, true);
            SetLocationNeighborId(hall, library, Direction.East, true);
            SetLocationNeighborId(hall, hall_1, Direction.West, true);
            SetLocationNeighborId(hall_1, kitchen, Direction.West, true);
            SetLocationNeighborId(hall_1, masterBedroom, Direction.North, true);
            SetLocationNeighborId(hall_1, broomCloset, Direction.South, true);
            SetLocationNeighborId(masterBedroom, office, Direction.North, true);

            // Bob is the default player
            m_bob = AddPlayer(new Player());
            GetPlayer(m_bob).SetLocationId(hall);
            PromptString = "Bob> ";

            // Betty is a second player
            m_betty = AddPlayer(new Player());
            GetPlayer(m_betty).SetLocationId(kitchen);

            // Show location for first player
            SelectPlayer(m_bob);

            Dictionary.AddWord("Keys", (int)MY_WORDS.WORD_KEYS);
            Dictionary.AddWord("Open", (int)MY_WORDS.WORD_OPEN);
            Dictionary.AddWord("Safe", (int)MY_WORDS.WORD_SAFE);
            Dictionary.AddWord("Medallion", (int)MY_WORDS.WORD_MEDALLION);
            Dictionary.AddWord("Become", (int)MY_WORDS.WORD_BECOME);
            Dictionary.AddWord("Bob", (int)MY_WORDS.WORD_BOB);
            Dictionary.AddWord("Betty", (int)MY_WORDS.WORD_BETTY);

            int keys = AddItem(new Item((int)MY_WORDS.WORD_KEYS, "Set of keys", "Someone left their keys here"));
            GetLocation(room).Inventory.AddItem(keys);
        }

        public override bool DoCommand(Game game, Player player, Command command)
        {
            if (command[0] == (int)MY_WORDS.WORD_BECOME)
            {
                switch (command[1])
                {
                    case (int)MY_WORDS.WORD_BOB:
                        WriteOutput("You are now Bob\n\n");
                        SelectPlayer(m_bob);
                        PromptString = "Bob> ";
                        return true;

                    case (int)MY_WORDS.WORD_BETTY:
                        WriteOutput("You are now Betty\n\n");
                        SelectPlayer(m_betty);
                        PromptString = "Betty> ";
                        return true;
                }
            }

            return base.DoCommand(game, player, command);
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Game game = new MyGame();
            game.Run();
        }
    }
}

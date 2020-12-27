using System;
using System.Collections.Generic;
using System.Text;

namespace GoNorth
{
    //------------------------------------------------------------------------------------------------
    public class Game : CommandInterpreter
    {
        //
        // Data members
        //
        bool _exit = false;
        SortedList<int, Item> _items = new SortedList<int, Item>();
        SortedList<int, Location> _locations = new SortedList<int, Location>();
        SortedList<int, Player> _players = new SortedList<int, Player>();
        Dictionary _dictionary = new Dictionary();
        public string PromptString { get; set; }
        int _selectedPlayerId = 0;

        //
        // Constructor
        //
        public Game()
        {
            // Register words
            _dictionary.AddWord("Help", (int)WORDS.WORD_HELP);
            _dictionary.AddWord("Exit", (int)WORDS.WORD_EXIT);
            _dictionary.AddWord("Use", (int)WORDS.WORD_USE);
            _dictionary.AddWord("Go", (int)WORDS.WORD_GO);
            _dictionary.AddWord("Get", (int)WORDS.WORD_GET);
            _dictionary.AddWord("Drop", (int)WORDS.WORD_DROP);
            _dictionary.AddWord("Move", (int)WORDS.WORD_MOVE);
            _dictionary.AddWord("Look", (int)WORDS.WORD_LOOK);
            _dictionary.AddWord("North", (int)WORDS.WORD_NORTH);
            _dictionary.AddWord("Northeast", (int)WORDS.WORD_NORTHEAST);
            _dictionary.AddWord("East", (int)WORDS.WORD_EAST);
            _dictionary.AddWord("Southeast", (int)WORDS.WORD_SOUTHEAST);
            _dictionary.AddWord("South", (int)WORDS.WORD_SOUTH);
            _dictionary.AddWord("Southwest", (int)WORDS.WORD_SOUTHWEST);
            _dictionary.AddWord("West", (int)WORDS.WORD_WEST);
            _dictionary.AddWord("Northwest", (int)WORDS.WORD_NORTHWEST);
            _dictionary.AddWord("Up", (int)WORDS.WORD_UP);
            _dictionary.AddWord("Down", (int)WORDS.WORD_DOWN);
            _dictionary.AddWord("Yes", (int)WORDS.WORD_YES);
            _dictionary.AddWord("No", (int)WORDS.WORD_NO);
            _dictionary.AddWord("Inventory", (int)WORDS.WORD_INVENTORY);

            // Aliases
            _dictionary.AddWord("?", (int)WORDS.WORD_HELP);
            _dictionary.AddWord("Quit", (int)WORDS.WORD_EXIT);
            _dictionary.AddWord("Q", (int)WORDS.WORD_EXIT);
            _dictionary.AddWord("N", (int)WORDS.WORD_NORTH);
            _dictionary.AddWord("NE", (int)WORDS.WORD_NORTHEAST);
            _dictionary.AddWord("E", (int)WORDS.WORD_EAST);
            _dictionary.AddWord("SE", (int)WORDS.WORD_SOUTHEAST);
            _dictionary.AddWord("S", (int)WORDS.WORD_SOUTH);
            _dictionary.AddWord("SW", (int)WORDS.WORD_SOUTHWEST);
            _dictionary.AddWord("W", (int)WORDS.WORD_WEST);
            _dictionary.AddWord("NW", (int)WORDS.WORD_DOWN);
            _dictionary.AddWord("I", (int)WORDS.WORD_INVENTORY);
            _dictionary.AddWord("Y", (int)WORDS.WORD_YES);
            _dictionary.AddWord("N", (int)WORDS.WORD_NO);

            // Add default player and select
            _selectedPlayerId = AddPlayer(new Player());

            // Set the default prompt string
            PromptString = "> ";
        }

        //
        // Accessors
        //

        public Dictionary Dictionary
        {
            get { return _dictionary; }
        }

        //------------------------------------------------------------------------------------------------
        public Item GetItem(int WordId)
        {
            return _items[WordId];
        }

        //------------------------------------------------------------------------------------------------
        public Location GetLocation(int LocationId)
        {
            return _locations[LocationId];
        }

        //------------------------------------------------------------------------------------------------
        public Player GetPlayer(int PlayerId)
        {
            return _players[PlayerId];
        }

        //------------------------------------------------------------------------------------------------
        public static Direction WordIdToDirection(int WordId)
        {
            Direction direction = Direction.NoDirection;

            switch (WordId)
            {
                case (int) WORDS.WORD_NORTH:
                    direction = Direction.North;
                    break;
                case (int)WORDS.WORD_NORTHEAST:
                    direction = Direction.Northeast;
                    break;
                case (int)WORDS.WORD_EAST:
                    direction = Direction.East;
                    break;
                case (int)WORDS.WORD_SOUTHEAST:
                    direction = Direction.Southeast;
                    break;
                case (int)WORDS.WORD_SOUTH:
                    direction = Direction.South;
                    break;
                case (int)WORDS.WORD_SOUTHWEST:
                    direction = Direction.Southwest;
                    break;
                case (int)WORDS.WORD_WEST:
                    direction = Direction.West;
                    break;
                case (int)WORDS.WORD_NORTHWEST:
                    direction = Direction.Northwest;
                    break;
                case (int)WORDS.WORD_UP:
                    direction = Direction.Up;
                    break;
                case (int)WORDS.WORD_DOWN:
                    direction = Direction.Down;
                    break;
            }

            return direction;
        }

        //
        // Overridable Methods
        //

        //------------------------------------------------------------------------------------------------
        public virtual void Initialize() { }

        //------------------------------------------------------------------------------------------------
        public virtual void BeginFrame() { }

        //------------------------------------------------------------------------------------------------
        public virtual void EndFrame() { }

        //------------------------------------------------------------------------------------------------
        public virtual void Cleanup() { }

        //------------------------------------------------------------------------------------------------
        public virtual string ReadInput()
        {
            // The default implementation is to wait for console input from stdin
            return Console.ReadLine();
        }

        //------------------------------------------------------------------------------------------------
        public virtual void WriteOutput(string outputString)
        {
            // By default write output to the console
            Console.Write(outputString);
        }

        public virtual void WriteLog(string logString)
        {

        }

        //------------------------------------------------------------------------------------------------
        public virtual void Run()
        {
            Initialize();

            // Main loop
            for (; _exit == false;)
            {
                BeginFrame();

                WriteOutput("\n" + PromptString);
                string commandString = ReadInput();
                WriteLog(commandString);
                WriteOutput("\n");

                Command command = new Command();
                if (CommandBuilder.Tokenize(this, _dictionary, commandString, command))
                {
                    Player player = GetPlayer(_selectedPlayerId);

                    // First let the player handle the command
                    if (!player.DoCommand(this, command))
                    {
                        if (!DoCommand(this, player, command))
                        {
                            WriteOutput("That can't be done.\n");
                        }
                    }
                }

                EndFrame();
            }

            Cleanup();
        }

        //------------------------------------------------------------------------------------------------
        public override bool DoCommand(Game game, Player player, Command command)
        {
            // Parse the command...
            if (command.IsMatch((int)WORDS.WORD_HELP))
            {
                PrintHelp();
                return true;
            }

            if (command.IsMatch((int)WORDS.WORD_EXIT))
            {
                _exit = true;
                return true;
            }

            return false;
        }

        //
        // Methods
        //

        //------------------------------------------------------------------------------------------------
        public void SetLocationNeighborId(int locationId, int neighborId, Direction direction, bool biDirectional)
        {
            _locations[locationId].SetNeighborId(direction, neighborId);
            if (neighborId != -1 && biDirectional)
            {
                // Set the neighbor return path
                _locations[neighborId].SetNeighborId(GetReverseDirection(direction), locationId);
            }
        }

        //------------------------------------------------------------------------------------------------
        public int SelectPlayer(int playerId)
        {
            _selectedPlayerId = playerId >= 0 && playerId < _players.Count ? playerId : -1;
            Player player = GetPlayer(_selectedPlayerId);

            if (player != null)
            {
                Location location = GetLocation(player.LocationId);
                if (location != null)
                {
                    location.DoLook(this);
                }
            }

            return _selectedPlayerId;
        }

        //------------------------------------------------------------------------------------------------
        public void PrintHelp()
        {
            WriteOutput("No help for you yet\n");
        }

        //------------------------------------------------------------------------------------------------
        public int AddItem(Item item)
        {
            _items.Add(item.WordId, item);

            return item.WordId;
        }

        //------------------------------------------------------------------------------------------------
        public int AddLocation(Location location)
        {
            int locationId = _locations.Count;
            _locations.Add(locationId, location);
            return locationId;
        }

        //------------------------------------------------------------------------------------------------
        public int AddPlayer(Player player)
        {
            int playerId = _players.Count;
            _players.Add(playerId, player);
            return playerId;
        }

        //------------------------------------------------------------------------------------------------
        public Direction GetReverseDirection(Direction direction)
        {
            switch (direction)
            {
                case Direction.North:
                    return Direction.South;
                case Direction.Northeast:
                    return Direction.Northwest;
                case Direction.East:
                    return Direction.West;
                case Direction.Southeast:
                    return Direction.Northwest;
                case Direction.South:
                    return Direction.North;
                case Direction.Southwest:
                    return Direction.Northeast;
                case Direction.West:
                    return Direction.East;
                case Direction.Northwest:
                    return Direction.Southeast;
                case Direction.Up:
                    return Direction.Down;
                case Direction.Down:
                    return Direction.Up;
            }
            return Direction.NumDirections; // ERROR
        }
    }
}


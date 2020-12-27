using System;
using System.Collections.Generic;
using System.Text;

namespace GoNorth
{
    public class Player
    {
        int _locationId;
        public int LocationId
        {
            get => _locationId; set => _locationId = value;
        }

        Inventory _inventory = new Inventory();
        public Inventory Inventory
        {
            get => _inventory;
        }

        void NoGo(Game game)
        {
            game.WriteOutput("You can only try to go one of the following directions:\n");
            game.WriteOutput(" North\n");
            game.WriteOutput(" Northeast\n");
            game.WriteOutput(" East\n");
            game.WriteOutput(" Southeast\n");
            game.WriteOutput(" South\n");
            game.WriteOutput(" Southwest\n");
            game.WriteOutput(" West\n");
            game.WriteOutput(" Northwest\n");
            game.WriteOutput(" Up\n");
            game.WriteOutput(" Down\n");
        }

        public virtual bool DoCommand(Game game, Command command)
        {
            if (command.Equals(new Command((int)WORDS.WORD_LOOK)))
            {
                DoLook(game);
                return true;
            }

            if (command.Equals(new Command((int)WORDS.WORD_INVENTORY)))
            {
                if (Inventory.ItemCount > 0)
                {
                    game.WriteOutput("You have the following:\n");
                    _inventory.PrintItemsInInventory(game);
                }
                else
                {
                    game.WriteOutput("You have nothing.\n");
                }
                return true;
            }

            // GO ...
            if (command[0] == (int)WORDS.WORD_GO || command[0] == (int)WORDS.WORD_MOVE)
            {
                if (command.Length == 1)
                {
                    game.WriteOutput("Go where?\n");
                }
                else if (command.Length > 2)
                {
                    NoGo(game);
                }
                else
                {
                    Direction direction = Game.WordIdToDirection(command[1]);
                    if (direction == Direction.NoDirection)
                    {
                        NoGo(game);
                    }
                    else
                    {
                        DoGoDirection(game, direction);
                    }
                }
            }

            if (command.IsMatch((int)WORDS.WORD_NORTH))
            {
                DoGoDirection(game, Direction.North);
                return true;
            }

            if (command.IsMatch((int)WORDS.WORD_NORTHEAST))
            {
                DoGoDirection(game, Direction.Northeast);
                return true;
            }

            if (command.IsMatch((int)WORDS.WORD_EAST))
            {
                DoGoDirection(game, Direction.East);
                return true;
            }

            if (command.IsMatch((int)WORDS.WORD_SOUTHEAST))
            {
                DoGoDirection(game, Direction.Southeast);
                return true;
            }

            if (command.IsMatch((int)WORDS.WORD_SOUTH))
            {
                DoGoDirection(game, Direction.South);
                return true;
            }

            if (command.IsMatch((int)WORDS.WORD_SOUTHWEST))
            {
                DoGoDirection(game, Direction.Southwest);
                return true;
            }

            if (command.IsMatch((int)WORDS.WORD_WEST))
            {
                DoGoDirection(game, Direction.West);
                return true;
            }

            if (command.IsMatch((int)WORDS.WORD_NORTHWEST))
            {
                DoGoDirection(game, Direction.Northwest);
                return true;
            }

            if (command.IsMatch((int)WORDS.WORD_UP))
            {
                DoGoDirection(game, Direction.Up);
                return true;
            }

            if (command.IsMatch((int)WORDS.WORD_DOWN))
            {
                DoGoDirection(game, Direction.Down);
                return true;
            }

            Location Location = game.GetLocation(_locationId);

            switch (command[0])
            {
                case (int)WORDS.WORD_GET:
                    switch (command.Length)
                    {
                        case 1:
                            game.WriteOutput("Get what?\n");
                            break;
                        case 2:
                            if (Location != null)
                            {
                                if (Location.Inventory.HasItem(command[1]))
                                {
                                    // Move the item from the location inventory to the player inventory
                                    Location.Inventory.RemoveItem(command[1]);
                                    Inventory.AddItem(command[1]);
                                    game.WriteOutput("Getting " + game.Dictionary.GetWordString(command[1]) + "\n");
                                }
                                else
                                {
                                    game.WriteOutput("Where do I get that?\n");
                                }
                            }
                            else
                            {
                                game.WriteOutput("There is nothing at your location.\n");
                            }
                            break;
                        default:
                            game.WriteOutput("Can only get one thing at a time.\n");
                            break;
                    }
                    return true;

                case (int)WORDS.WORD_DROP:
                    switch (command.Length)
                    {
                        case 1:
                            game.WriteOutput("Get what?\n");
                            break;
                        case 2:
                            if (Location != null)
                            {
                                if (Inventory.HasItem(command[1]))
                                {
                                    // Move the item from the player inventory to the location inventory
                                    Inventory.RemoveItem(command[1]);
                                    Location.Inventory.AddItem(command[1]);
                                    game.WriteOutput("Dropping " + game.Dictionary.GetWordString(command[1]).ToLower() + "\n");
                                }
                                else
                                {
                                    game.WriteOutput("You don't have that.\n");
                                }
                            }
                            else
                            {
                                game.WriteOutput("There is nowhere to drop anything.\n");
                            }
                            break;
                    }
                    return true;
            }

            // Give the player location a chance to process the command
            if (Location != null)
            {
                return Location.DoCommand(game, this, command);
            }

            return false;
        }

        public void DoLook(Game game)
        {
            Location location = game.GetLocation(_locationId);

            if (location != null)
            {
                location.DoLook(game);
            }
            else
            {
                game.WriteOutput("Nothing to look at\n");
            }
        }

        //------------------------------------------------------------------------------------------------
        public void SetLocationId(int locationId)
        {
            _locationId = locationId;
        }

        //------------------------------------------------------------------------------------------------
        public bool DoGoDirection(Game game, Direction direction)
        {
            int fromLocationId = _locationId;
            Location location = game.GetLocation(_locationId);

            if (location.DoGoDirection(game, direction))
            {
                SetLocationId(location.NeighborIds[(int)direction]);

                location = game.GetLocation(_locationId);
                if (location != null)
                {
                    location.DoEnter(game, fromLocationId);
                }

                return true;
            }

            return false;
        }
    }

}

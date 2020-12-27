using System;
using System.Collections.Generic;
using System.Text;

namespace GoNorth
{
    //------------------------------------------------------------------------------------------------
    // Enum values representing compass directions and up/down.  
    // These are used to describe player movement between locations.
    public enum Direction : int
    {
        North = 0,
        Northeast,
        East,
        Southeast,
        South,
        Southwest,
        West,
        Northwest,
        Up,
        Down,
        NumDirections,
        NoDirection = -1
    };

    //------------------------------------------------------------------------------------------------
    public class Location : CommandInterpreter
    {
        Inventory _inventory = new Inventory();
        public Inventory Inventory
        {
            get { return _inventory; }
        }

        private int[] _neighborIds;

        public int[] NeighborIds { get => _neighborIds; set => _neighborIds = value; }

        //------------------------------------------------------------------------------------------------
        public Location()
        {
            NeighborIds = new int[(int)Direction.NumDirections] { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
        }

        //------------------------------------------------------------------------------------------------
        virtual public void DoLook(Game game)
        {
            _inventory.PrintItemsInLocation(game);
        }

        //------------------------------------------------------------------------------------------------
        virtual public void DoEnter(Game game, int fromLocationId)
        {
            DoLook(game);
        }

        //------------------------------------------------------------------------------------------------
        public void SetNeighborId(Direction direction, int locationId)
        {
            NeighborIds[(int)direction] = locationId;
        }

        //------------------------------------------------------------------------------------------------
        public bool DoGoDirection(Game game, Direction direction)
        {
            if (_neighborIds[(int)direction] == -1)
            {
                game.WriteOutput("You can't go that way.\n");
                return false;
            }

            return true;
        }
    }

    //------------------------------------------------------------------------------------------------
    public class StaticLocation : Location
    {
        string _description;
        public string Description
        {
            get => _description; 
        }

        //------------------------------------------------------------------------------------------------
        public StaticLocation(string description )
        {
            _description = description;
        }

        //------------------------------------------------------------------------------------------------
        public override void DoLook(Game game)
        {
            game.WriteOutput(_description + "\n");
            base.DoLook(game);
        }
    }
}

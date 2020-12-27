using System;
using System.Collections.Generic;
using System.Text;

namespace GoNorth
{
    //------------------------------------------------------------------------------------------------
    public class Item
    {
        int _wordId;
        string _inventoryDescription;
        string _locationDescription;
        public int WordId { get => _wordId; }
        public string InventoryDescription { get => _inventoryDescription; }
        public string LocationDescription { get => _locationDescription; }

        //------------------------------------------------------------------------------------------------
        public Item(int wordId, string inventoryDescription, string locationDescription)
        {
            _wordId = wordId;
            _inventoryDescription = inventoryDescription;
            _locationDescription = locationDescription;
        }
    }

    //------------------------------------------------------------------------------------------------
    public class Inventory 
    {
        SortedSet<int> _intentoryItems = new SortedSet<int>();

        //------------------------------------------------------------------------------------------------
        public void AddItem(int WordId)
        {
            _intentoryItems.Add(WordId);
        }

        //------------------------------------------------------------------------------------------------
        public void RemoveItem(int WordId)
        {
            _intentoryItems.Remove(WordId);
        }

        //------------------------------------------------------------------------------------------------
        public bool HasItem(int WordId)
        {
            return _intentoryItems.Contains(WordId);
        }

        //------------------------------------------------------------------------------------------------
        public int ItemCount
        {
            get { return _intentoryItems.Count; }
        }

        //------------------------------------------------------------------------------------------------
        public void PrintItemsInLocation(Game game)
        {
            foreach( int WordId in _intentoryItems)
            {
                game.WriteOutput(game.GetItem(WordId).LocationDescription + "\n");
            }
        }

        //------------------------------------------------------------------------------------------------
        public void PrintItemsInInventory(Game game)
        {
            foreach (int WordId in _intentoryItems)
            {
                game.WriteOutput(game.GetItem(WordId).InventoryDescription + "\n");
            }
        }
    }
}

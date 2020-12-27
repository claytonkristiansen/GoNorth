//------------------------------------------------------------------------------------------------
// Item.h
//
// Header file for GoNorth CItem and CInventory classes
//------------------------------------------------------------------------------------------------

#pragma once

namespace GoNorth
{
    //------------------------------------------------------------------------------------------------
    // Class of objects that can be placed in inventory or found in locations
    class CItem
    {
        WordId m_wordId;                    // Dictionary word id of noun representing item
        std::string m_inventoryDescription; // Description of item in inventory list
        std::string m_locationDescription;  // Description of item in location description

    public:
        CItem(WordId wordId, const std::string &inventoryDescription, const std::string &locationDescription) :
            m_wordId(wordId),
            m_inventoryDescription(inventoryDescription),
            m_locationDescription(locationDescription)
        {

        }

        WordId GetWordId() const { return m_wordId; }
        const std::string &GetInventoryDescription() const { return m_inventoryDescription; }
        const std::string &GetLocationDescription() const { return m_locationDescription; }
    };

//------------------------------------------------------------------------------------------------
    class CInventory
    {
        // Stores items indexed by dictionary id of item name
        std::set<WordId> m_inventoryItems;

    public:
        CInventory() = default;

        void AddItem(WordId itemWordId); // throw(std:bad_alloc)
        void RemoveItem(WordId itemWordId);
        bool HasItem(WordId itemWordId) const;
        void PrintItemsInInventory() const;
        void PrintItemsInLocation() const;

        size_t GetItemCount() const { return m_inventoryItems.size(); }
    };
}

//------------------------------------------------------------------------------------------------
// Item.cpp
//
// Implements CItem and CInventory classes
//------------------------------------------------------------------------------------------------

#include "stdafx.h"

namespace GoNorth
{
    //------------------------------------------------------------------------------------------------
    void CInventory::AddItem(WordId itemWordId)
    {
        // Insert item pointer into map
        m_inventoryItems.insert(itemWordId);
    }

    //------------------------------------------------------------------------------------------------
    void CInventory::RemoveItem(WordId itemWordId)
    {
        // Remove the item from the map
        m_inventoryItems.erase(itemWordId);
    }

    //------------------------------------------------------------------------------------------------
    bool CInventory::HasItem(WordId itemWordId) const
    {
        return m_inventoryItems.end() != m_inventoryItems.find(itemWordId);
    }

    //------------------------------------------------------------------------------------------------
    void CInventory::PrintItemsInInventory() const
    {
        for (auto itemWordId : m_inventoryItems)
        {
            CGame::Game().OutputStream() << CGame::Game().GetItem(itemWordId)->GetInventoryDescription() << "\n";
        }
    }

    //------------------------------------------------------------------------------------------------
    void CInventory::PrintItemsInLocation() const
    {
        for (auto itemWordId : m_inventoryItems)
        {
            CGame::Game().OutputStream() << CGame::Game().GetItem(itemWordId)->GetLocationDescription() << "\n";
        }
    }
}
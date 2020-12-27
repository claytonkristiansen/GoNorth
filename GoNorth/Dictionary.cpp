//------------------------------------------------------------------------------------------------
// Dictionary.cpp
//
// Implementation of CDictionary class
//------------------------------------------------------------------------------------------------

#include "stdafx.h"

namespace GoNorth
{
    //------------------------------------------------------------------------------------------------
    std::string ToLower(const std::string &word)
    {
        std::string lcword(word.length(), 0);
        std::transform(word.begin(), word.end(), lcword.begin(), ::tolower);
        return lcword;
    }

    //------------------------------------------------------------------------------------------------
    WordId CDictionary::AddWord(const std::string &wordString, WordId wordId)
    {
        try
        {
            std::string lower = ToLower(wordString);

            // See if the string was already used
            if (m_stringToWordIdMap.end() != m_stringToWordIdMap.find(lower))
            {
                // Duplicate string
                return 0;
            }

            if (m_wordIdToStringMap.end() == m_wordIdToStringMap.find(wordId))
            {
                // Only add first use of a given id to the map
                m_wordIdToStringMap.emplace(wordId, wordString); // throw(std::bad_alloc)
            }

            // Add the string to the table
            m_stringToWordIdMap.emplace(lower, wordId);
        }
        catch (const std::bad_alloc &)
        {
            return WORD_NONE;
        }

        return wordId;
    }

    //------------------------------------------------------------------------------------------------
    WordId CDictionary::GetWordId(const std::string &word) const
    {
        auto it = m_stringToWordIdMap.find(ToLower(word));
        if (it != m_stringToWordIdMap.end())
        {
            return it->second;
        }

        return WORD_NONE;
    }

    //------------------------------------------------------------------------------------------------
    const std::string &CDictionary::GetWordString(WordId wordId) const
    {
        static std::string unknown("");
        auto it = m_wordIdToStringMap.find(wordId);
        if (it != m_wordIdToStringMap.end())
        {
            return it->second;
        }

        return unknown;
    }
}
//------------------------------------------------------------------------------------------------
// Command.cpp
//
// Implementation of CCommand class
//------------------------------------------------------------------------------------------------

#include "stdafx.h"

namespace GoNorth
{

    //------------------------------------------------------------------------------------------------
    CCommand::CCommand(WordId *pWordIds, size_t numWordIds) :
        m_commandWordIds(numWordIds)
    {
        for (size_t i = 0; i < numWordIds; ++i)
        {
            m_commandWordIds[i] = pWordIds[i];
        }
    }

    //------------------------------------------------------------------------------------------------
    CCommand::CCommand(WordId word0)
    {
        m_commandWordIds.push_back(word0);
    }
    CCommand::CCommand(WordId word0, WordId word1)
    {
        m_commandWordIds.push_back(word0);
        m_commandWordIds.push_back(word1);
    }
    CCommand::CCommand(WordId word0, WordId word1, WordId word2)
    {
        m_commandWordIds.push_back(word0);
        m_commandWordIds.push_back(word1);
        m_commandWordIds.push_back(word2);
    }
    CCommand::CCommand(WordId word0, WordId word1, WordId word2, WordId word3)
    {
        m_commandWordIds.push_back(word0);
        m_commandWordIds.push_back(word1);
        m_commandWordIds.push_back(word2);
        m_commandWordIds.push_back(word3);
    }
    CCommand::CCommand(WordId word0, WordId word1, WordId word2, WordId word3, WordId word4)
    {
        m_commandWordIds.push_back(word0);
        m_commandWordIds.push_back(word1);
        m_commandWordIds.push_back(word2);
        m_commandWordIds.push_back(word3);
        m_commandWordIds.push_back(word4);
    }
    
    //------------------------------------------------------------------------------------------------
    CCommand::CCommand::CCommand(const CCommand &o) : 
        m_commandWordIds(o.m_commandWordIds)
    {
    }

    //------------------------------------------------------------------------------------------------
    CCommand::CCommand(CCommand &&o) :
        m_commandWordIds(std::move(o.m_commandWordIds))
    {
    }

    //------------------------------------------------------------------------------------------------
    CCommand &CCommand::operator=(const CCommand &o)
    {
        m_commandWordIds.resize(o.Length());
        for (size_t i = 0; i < o.Length(); ++i)
        {
            m_commandWordIds[i] = o[i];
        }
        return *this;
    }

    //------------------------------------------------------------------------------------------------
    CCommand &CCommand::operator=(CCommand &&o)
    {
        m_commandWordIds = std::move(o.m_commandWordIds);
        return *this;
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::operator==(const CCommand &o) const
    {
        return m_commandWordIds == o.m_commandWordIds;
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::IsMatch(WordId *pWordIds, size_t numWordIds) const 
    {
        return IsMatchAtIndex(0, pWordIds, numWordIds);
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::IsMatch(WordId word0) const
    {
        return IsMatchAtIndex(0, word0);
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::IsMatch(WordId word0, WordId word1) const
    {
        return IsMatchAtIndex(0, word0, word1);
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::IsMatch(WordId word0, WordId word1, WordId word2) const
    {
        return IsMatchAtIndex(0, word0, word1, word2);
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::IsMatch(WordId word0, WordId word1, WordId word2, WordId word3) const
    {
        return IsMatchAtIndex(0, word0, word1, word2, word3);
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::IsMatch(WordId word0, WordId word1, WordId word2, WordId word3, WordId word4) const
    {
        return IsMatchAtIndex(0, word0, word1, word2, word3, word4);
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::IsMatchAtIndex(WordId startIndex, WordId *pWordIds, size_t numWordIds) const
    {
        if (Length() != numWordIds + startIndex) return false;

        for (WordId i = 0; i < numWordIds; ++i)
        {
            if (m_commandWordIds[i + startIndex] != pWordIds[i]) return false;
        }

        return true;
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::IsMatchAtIndex(WordId startIndex, WordId word0) const
    {
        if (Length() <= startIndex) return false;

        return m_commandWordIds[0 + startIndex] == word0;
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::IsMatchAtIndex(WordId startIndex, WordId word0, WordId word1) const
    {
        if (Length() != 2 + startIndex) return false;
        if (m_commandWordIds[0 + startIndex] != word0) return false;
        if (m_commandWordIds[1 + startIndex] != word1) return false;
        return true;
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::IsMatchAtIndex(WordId startIndex, WordId word0, WordId word1, WordId word2) const
    {
        if (Length() != 3 + startIndex) return false;
        if (m_commandWordIds[0 + startIndex] != word0) return false;
        if (m_commandWordIds[1 + startIndex] != word1) return false;
        if (m_commandWordIds[2 + startIndex] != word2) return false;
        return true;
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::IsMatchAtIndex(WordId startIndex, WordId word0, WordId word1, WordId word2, WordId word3) const
    {
        if (Length() != 4 + startIndex) return false;
        if (m_commandWordIds[0 + startIndex] != word0) return false;
        if (m_commandWordIds[1 + startIndex] != word1) return false;
        if (m_commandWordIds[2 + startIndex] != word2) return false;
        if (m_commandWordIds[3 + startIndex] != word3) return false;
        return true;
    }

    //------------------------------------------------------------------------------------------------
    bool CCommand::IsMatchAtIndex(WordId startIndex, WordId word0, WordId word1, WordId word2, WordId word3, WordId word4) const
    {
        if (Length() != 5 + startIndex) return false;
        if (m_commandWordIds[0 + startIndex] != word0) return false;
        if (m_commandWordIds[1 + startIndex] != word1) return false;
        if (m_commandWordIds[2 + startIndex] != word2) return false;
        if (m_commandWordIds[3 + startIndex] != word3) return false;
        if (m_commandWordIds[4 + startIndex] != word4) return false;
        return true;
    }

    //------------------------------------------------------------------------------------------------
    void CCommand::AddWord(WordId token)
    {
        m_commandWordIds.push_back(token);
    }

    //------------------------------------------------------------------------------------------------
    bool CCommandBuilder::Tokenize(const CDictionary &dictionary, const std::string &inputString, CCommand &command)
    {
        size_t wordStart = 0;

        // Iterate over the words in the string
        for (; wordStart != std::string::npos;)
        {
            // Strip out leading spaces
            wordStart = inputString.find_first_not_of(' ', wordStart);

            if (wordStart != std::string::npos)
            {
                size_t wordEnd = inputString.find_first_of(' ', wordStart);

                // Get the word string
                size_t count = wordEnd == std::string::npos ? std::string::npos : wordEnd - wordStart;
                std::string wordString = inputString.substr(wordStart, count);

                // Lookup the word in the dictionary
                WordId wordId = dictionary.GetWordId(wordString);

                if (wordId == 0)
                {
                    // Error: Cannot tokenize the string
                    CGame::Game().OutputStream() << "Don't know what \"" << wordString << "\" means.";
                    return false;
                }

                command.AddWord(wordId);

                wordStart = wordEnd;
            }
        }

        return true;
    }
}
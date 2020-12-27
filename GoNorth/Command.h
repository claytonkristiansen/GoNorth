//------------------------------------------------------------------------------------------------
// Command.h
//
// Header file for GoNorth CCommand class and CCommandBuilder class
//------------------------------------------------------------------------------------------------

#pragma once

namespace GoNorth
{
    // Forward declarations
    class CPlayer;

    //------------------------------------------------------------------------------------------------
    // Container for a set of tokenized commands.
    class CCommand
    {
        std::vector<WordId> m_commandWordIds;

    public:
        // Default constructor
        CCommand() = default;

        // Construct from word token array
        CCommand(WordId *pWordIds, size_t numWordIds);

        // Helper constructors for small word counts
        CCommand(WordId word0);
        CCommand(WordId word0, WordId word1);
        CCommand(WordId word0, WordId word1, WordId word2);
        CCommand(WordId word0, WordId word1, WordId word2, WordId word3);
        CCommand(WordId word0, WordId word1, WordId word2, WordId word3, WordId word4);

        // Copy constructor
        CCommand(const CCommand &o);

        // Move constructor
        CCommand(CCommand &&o);

        // Copy assignment
        CCommand &operator=(const CCommand &o);

        // Move assigmnent
        CCommand &operator=(CCommand &&o);

        // Comparison
        bool operator==(const CCommand &o) const;
        bool IsMatch(WordId *pWordIds, size_t numWordIds) const;
        bool IsMatch(WordId word0) const;
        bool IsMatch(WordId word0, WordId word1) const;
        bool IsMatch(WordId word0, WordId word1, WordId word2) const;
        bool IsMatch(WordId word0, WordId word1, WordId word2, WordId word3) const;
        bool IsMatch(WordId word0, WordId word1, WordId word2, WordId word3, WordId word4) const;
        bool IsMatchAtIndex(WordId startIndex, WordId *pWordIds, size_t numWordIds) const;
        bool IsMatchAtIndex(WordId startIndex, WordId word0) const;
        bool IsMatchAtIndex(WordId startIndex, WordId word0, WordId word1) const;
        bool IsMatchAtIndex(WordId startIndex, WordId word0, WordId word1, WordId word2) const;
        bool IsMatchAtIndex(WordId startIndex, WordId word0, WordId word1, WordId word2, WordId word3) const;
        bool IsMatchAtIndex(WordId startIndex, WordId word0, WordId word1, WordId word2, WordId word3, WordId word4) const;

        // Indexer
        WordId operator[](size_t index) const { return index < Length() ? m_commandWordIds[index] : WORD_NONE; }

        void AddWord(WordId wordId);
        WordId GetWord(size_t index) const { return index < m_commandWordIds.size() ? m_commandWordIds[index] : WORD_NONE; }
        size_t Length() const { return m_commandWordIds.size(); }
    };

    //------------------------------------------------------------------------------------------------
    // Base class for all objects than handle game commands,
    // this includes CPlayer, CLocation, and CGame.
    class CCommandInterpreter
    {
    public:
        // Returns 'true' if the command is finished being processed
        virtual bool DoCommand(CPlayer *pPlayer, const CCommand &command) { return false; }
    };

    //------------------------------------------------------------------------------------------------
    // Static helper class for converting command strings into an array of
    // word ids.  This is frequently referred to as tokenizing.
    class CCommandBuilder
    {
    public:
        // Converts a space-delimited input string into a set of word id tokens.
        // Returns 'true' if successful
        static bool Tokenize(const CDictionary &dictionary, const std::string &inputString, CCommand &command);
    };

}
//------------------------------------------------------------------------------------------------
// Dictionary.h
//
// Header file for GoNorth CDictionary class
//------------------------------------------------------------------------------------------------

#pragma once

namespace GoNorth
{
    typedef unsigned int WordId;

    //------------------------------------------------------------------------------------------------
    // Ids for words reserved by the GoNorth engine.
    // App can start enumerating custom words starting with CUSTOM_WORD_ID
    enum WORDS
    {
        WORD_NONE = 0,
        WORD_NORTH,
        WORD_NORTHEAST,
        WORD_EAST,
        WORD_SOUTHEAST,
        WORD_SOUTH,
        WORD_SOUTHWEST,
        WORD_WEST,
        WORD_NORTHWEST,
        WORD_UP,
        WORD_DOWN,
        WORD_HELP,
        WORD_EXIT,
        WORD_USE,
        WORD_GO,
        WORD_MOVE,
        WORD_GET,
        WORD_DROP,
        WORD_LOOK,
        WORD_YES,
        WORD_NO,
        WORD_INVENTORY,
        CUSTOM_WORD,
    };

    // Helper function converting strings to lower case
    extern std::string ToLower(const std::string &word);

    //------------------------------------------------------------------------------------------------
    // The dictionary is the master list of words that
    // can be used in game commands.  Client applications can
    // add custom words to the dictionary.
    class CDictionary
    {
    public:
    private:
        std::map<std::string, WordId> m_stringToWordIdMap; // Maps string to unique word id
        std::map<WordId, std::string> m_wordIdToStringMap; // Maps unique id to word entry;

    public:
        CDictionary() = default;

        WordId AddWord(const std::string &word, WordId wordId);

        WordId GetWordId(const std::string &word) const;
        const std::string &GetWordString(WordId wordId) const;
    };
}

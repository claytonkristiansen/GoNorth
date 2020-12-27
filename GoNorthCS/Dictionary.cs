using System;
using System.Collections.Generic;
using System.Text;

namespace GoNorth
{
    public enum WORDS : int
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

    //------------------------------------------------------------------------------------------------
    public class Dictionary
    {
        SortedList<string, int> _stringToWordIdList = new SortedList<string, int>();
        SortedList<int, string> _wordIdToStringList = new SortedList<int, string>();

        //------------------------------------------------------------------------------------------------
        public int AddWord(string s, int wordId)
        {
            string lower = s.ToLower();

            // See if the string was already used
            if(_stringToWordIdList.ContainsKey(lower))
            {
                // Duplicate string
                return 0;
            }

            if(!_wordIdToStringList.ContainsKey(wordId))
            {
                // Only add first use of a given id to the word list
                _wordIdToStringList.Add(wordId, s);
            }

            // Add the string to the table
            _stringToWordIdList.Add(lower, wordId);

            return wordId;
        }

        //------------------------------------------------------------------------------------------------
        public int GetWordId(string s)
        {
            string ls = s.ToLower();
            if(!_stringToWordIdList.ContainsKey(ls))
            {
                return (int) WORDS.WORD_NONE;
            }

            return _stringToWordIdList[ls];
        }

        //------------------------------------------------------------------------------------------------
        public string GetWordString(int wordId)
        {
            if(!_wordIdToStringList.ContainsKey(wordId))
            {
                return "";
            }

            return _wordIdToStringList[wordId];
        }
    }
}

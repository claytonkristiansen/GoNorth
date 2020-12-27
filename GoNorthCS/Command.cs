using System;
using System.Collections.Generic;
using System.Text;

namespace GoNorth
{
    //------------------------------------------------------------------------------------------------
    public class Command
    {
        List<int> _commandWordIds = new List<int>();

        public Command() { }

        // Construct from word token array
        public Command(int[] commandWordIds)
        {
            foreach (int wordId in commandWordIds)
            {
                _commandWordIds.Add(wordId);
            }
        }

        // Helper constructors for small word counts
        public Command(int word0)
        {
            _commandWordIds.Add(word0);
        }

        public Command(int word0, int word1)
        {
            _commandWordIds.Add(word0);
            _commandWordIds.Add(word1);
        }

        public Command(int word0, int word1, int word2)
        {
            _commandWordIds.Add(word0);
            _commandWordIds.Add(word1);
            _commandWordIds.Add(word2);
        }

        public Command(int word0, int word1, int word2, int word3)
        {
            _commandWordIds.Add(word0);
            _commandWordIds.Add(word1);
            _commandWordIds.Add(word2);
            _commandWordIds.Add(word3);
        }

        public Command(int word0, int word1, int word2, int word3, int word4)
        {
            _commandWordIds.Add(word0);
            _commandWordIds.Add(word1);
            _commandWordIds.Add(word2);
            _commandWordIds.Add(word3);
            _commandWordIds.Add(word4);
        }

        public bool IsMatch(int word0)
        {
            return
                _commandWordIds.Count == 1 && 
                _commandWordIds[0] == word0;
        }

        public bool IsMatch(int word0, int word1)
        {
            return
                _commandWordIds.Count == 2 &&
                _commandWordIds[0] == word0 &&
                _commandWordIds[1] == word1;
        }

        public bool IsMatch(int word0, int word1, int word2)
        {
            return
                _commandWordIds.Count == 3 &&
                _commandWordIds[0] == word0 &&
                _commandWordIds[1] == word1 &&
                _commandWordIds[2] == word2;
        }

        public bool IsMatch(int word0, int word1, int word2, int word3)
        {
            return
                _commandWordIds.Count == 4 &&
                _commandWordIds[0] == word0 &&
                _commandWordIds[1] == word1 &&
                _commandWordIds[2] == word2 &&
                _commandWordIds[3] == word3;
        }

        public bool IsMatch(int word0, int word1, int word2, int word3, int word4)
        {
            return
                _commandWordIds.Count == 5 &&
                _commandWordIds[0] == word0 &&
                _commandWordIds[1] == word1 &&
                _commandWordIds[2] == word2 &&
                _commandWordIds[3] == word3 &&
                _commandWordIds[4] == word4;
        }

        public void AddWord(int wordId)
        {
            _commandWordIds.Add(wordId);
        }

        // Comparison
        public bool Equals(Command o)
        {
            if(_commandWordIds.Count != o._commandWordIds.Count)
            {
                return false;
            }

            for (int i = 0; i < _commandWordIds.Count; ++i)
            {
                if (_commandWordIds[i] != o._commandWordIds[i])
                {
                    return false;
                }
            }

            return true;
        }

        // Indexer
        public int this[int index]
        {
            get { return _commandWordIds[index]; }
            set { _commandWordIds[index] = value; }
        }

        public int Length
        {
            get { return _commandWordIds.Count; }
        }
    }

    //------------------------------------------------------------------------------------------------
    public class CommandInterpreter
    {
        public virtual bool DoCommand(Game game, Player player, Command command) { return false; }
    }

    //------------------------------------------------------------------------------------------------
    public class CommandBuilder
    {
        public static bool Tokenize(Game game, Dictionary dictionary, string inputString, Command command)
        {
            // Remove trailing whitespace
            inputString.TrimEnd(' ');

            // Iterate over the words in the string
            for (; inputString.Length > 0;)
            {
                // Strip out leading spaces
                inputString = inputString.TrimStart(' ');
                if (inputString.Length > 0)
                {
                    // Get the word string
                    int wordEnd = inputString.IndexOf(' ', 0);
                    string wordString = wordEnd == -1 ? inputString : inputString.Substring(0, wordEnd);

                    // Lookup the word in the dictionary
                    int wordId = dictionary.GetWordId(wordString);

                    if (wordId == 0)
                    {
                        // Error: Cannot tokenize the string
                        game.WriteOutput("Don't know what \"" + wordString + "\" means.\n");
                        return false;
                    }

                    command.AddWord(wordId);
                    inputString = inputString.Substring(wordString.Length);
                }
            }

            return true;
        }
    }
}
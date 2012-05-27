using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MethodGroup1
{
    public class TestSequence
    {
        /// <summary>
        /// Default constructor
        /// </summary>
        /// <param name="name"></param>
        public TestSequence(String name)
        {
            this.Name = name;
            this.Path = new List<string>();
        }

        /// <summary>
        /// constructor for auto-generated names.
        /// </summary>
        public TestSequence()
        {
            this.Name = new Random().Next().ToString();
            this.path = new List<string>();
        }

        private string name;
        /// <summary>
        /// Name of Test Sequence. Same name of the Finite State machine from where it comes.
        /// </summary>
        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        private List<String> path;
        /// <summary>
        /// Generated Path.
        /// </summary>
        public List<String> Path
        {
            get { return path; }
            set { path = value; }
        }

        internal static string[][] ToStringMatrix(TestSequence[] sequences)
        {
            int numberOfTestSequences = sequences.Count();
            string[][] jaggedStringArray = new string[numberOfTestSequences][];

            int sequenceIndex = 0;
            foreach (TestSequence sequence in sequences)
            {
                string[] stringSequence = new string[sequence.Path.Count()];
                for (int i = 0; i < stringSequence.Length; i++)
                {
                    stringSequence[i] = sequence.Path[i];
                }
                jaggedStringArray[sequenceIndex] = stringSequence;
                sequenceIndex++;
            }
            return jaggedStringArray;
        }
    }
}

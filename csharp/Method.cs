using System;
using System.Collections.Generic;
using System.Linq;
using System.Data;
using MethodGroup1.Fsm;
using System.Collections;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Threading;

namespace MethodGroup1
{
    public class Method
    {
        /// <summary>
        /// Finite State Machine property.
        /// </summary>
        private FiniteStateMachine fsm;
        /// <summary>
        /// Generates TestSequences from a FiniteStateMachine.
        /// </summary>
        /// <param name="fsm">Given FiniteStateMachine.</param>
        /// <returns>A set of TestSequence.</returns>
        public TestSequence[] Do(FiniteStateMachine fsma)
        {
            this.fsm = fsma;
            fsm.RefreshData();

            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Implemente aqui seu método de geração de casos de testes ou a chamada externa do método implementado //
            //////////////////////////////////////////////////////////////////////////////////////////////////////////

            ///////////////////
            // BEGIN SAMPLE  //
            ///////////////////

            //define current work path
            string currentPath = Directory.GetCurrentDirectory()+ @"\";
            string machineFile = @"machine.txt";
            string outputFile = @"test_sequence.txt";
            
            //Add aliases to machine (stateNAme => s_0)
            Dictionary<string, string> aliases = new Dictionary<string,string>();
            AddMachineAliases(aliases);

            //Export fsm as text.
            this.WriteFsmToFile(fsm, currentPath + machineFile);


            Console.ForegroundColor = ConsoleColor.DarkRed;
            Process p = new Process();

            p.StartInfo.FileName = currentPath + @"statemachines.exe"; // start qt executable
            p.StartInfo.Arguments = machineFile + @" " + outputFile;
            p.StartInfo.UseShellExecute = false; //Should be enable to redirect output
            p.StartInfo.RedirectStandardOutput = true; //redirect java output to console window
            p.Start();
            p.WaitForExit();

            //print output
            TextReader reader = new StreamReader(currentPath + outputFile);
            string output = reader.ReadToEnd();
            reader.Close();
            Console.WriteLine(output);

            //restore console color
            Console.ForegroundColor = ConsoleColor.White;

            //returns a set of sequences.
            List<TestSequence> sequences = this.ReadSequencesFromFile(currentPath + outputFile);

            //rename machines
            foreach (TestSequence seq in sequences)
                seq.Name = fsm.Name;

            foreach (Transition t in fsm.Transitions)
                Console.WriteLine(t.ToString());

            sequences = RemoveAliases(sequences, aliases);

            foreach(TestSequence t in sequences){
                Console.WriteLine("");
                foreach (string s in t.Path)
                    Console.Write(s);
            }
            return sequences.ToArray();

            ////////////////
            // END SAMPLE //
            //////////////// 
        }

        private List<TestSequence> RemoveAliases(List<TestSequence> sequences, Dictionary<string, string> aliases) {
            List<TestSequence> newSequences = new List<TestSequence>();
            foreach (TestSequence seq in sequences) {
                for (int i = 0; i < seq.Path.Count; i++) {
                    Console.Write(seq.Path[i] + " => ");

                    IEnumerable<string> x = from KeyValuePair<string, string> pair in aliases
                                            where pair.Key == seq.Path[i]
                                            select pair.Value;

                    seq.Path[i] = x.First();
                    Console.WriteLine(seq.Path[i]);
                }
            }
            return newSequences;
        }
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="aliases"></param>
        private void AddMachineAliases(Dictionary<string, string> aliases) {
            foreach (Transition t in fsm.Transitions) {

                if (!aliases.Values.Contains("" + t.Input))
                    aliases.Add("" + aliases.Count, "" + t.Input);

                t.Input = (from KeyValuePair<string, string> pair in aliases
                           where pair.Value == "" + t.Input
                           select pair.Key).First();

                //output
                if (!aliases.Values.Contains("" + t.Output))
                    aliases.Add("" + aliases.Count, "" + t.Output);

                t.Output = (from KeyValuePair<string, string> pair in aliases
                            where pair.Value == "" + t.Output
                            select pair.Key).First();

                //states (source and target)
                if (!aliases.Values.Contains("" + t.SourceState.Name))
                    aliases.Add("" + aliases.Count, "" + t.SourceState.Name);

                t.SourceState.Name = (from KeyValuePair<string, string> pair in aliases
                                      where pair.Value == "" + t.SourceState.Name
                                      select pair.Key).First();

                if (!aliases.Values.Contains("" + t.TargetState.Name))
                    aliases.Add("" + aliases.Count, "" + t.TargetState.Name);

                t.TargetState.Name = (from KeyValuePair<string, string> pair in aliases
                                      where pair.Value == "" + t.TargetState.Name
                                      select pair.Key).First();
            }
            
            Console.ForegroundColor = ConsoleColor.Cyan;
            foreach(KeyValuePair<string,string> pair in aliases)
                Console.WriteLine(pair.Key + " => " + pair.Value);
            Console.ForegroundColor = ConsoleColor.White;
        }
        /// <summary>
        /// Writes FSM to textual file.
        /// </summary>
        /// <param name="fsm"></param>
        /// <param name="p"></param>
        private void WriteFsmToFile(FiniteStateMachine fsm, string p) {
            String textualMachine = "";

            foreach (Transition t in fsm.Transitions)
                textualMachine +=
                    t.SourceState.Name + " -- " +
                    t.Input.ToString() + " / " +
                    t.Output.ToString() + " -> " +
                    t.TargetState.Name + "\r\n";

            TextWriter writer = new StreamWriter(p);
            writer.Write(textualMachine);
            writer.Close();
        }
        /// <summary>
        /// Reads a sequence set from a textual file.
        /// </summary>
        /// <param name="p"></param>
        /// <returns></returns>
        private List<TestSequence> ReadSequencesFromFile(string p) {
            List<TestSequence> sequences = new List<TestSequence>();
            
            //reads file contents
            string line;
            TextReader reader = new StreamReader(p);

            //iterates through lines
            while((line = reader.ReadLine()) != null){
                
                //turn sequence into a list
                TestSequence seq = new TestSequence();
                seq.Path = line.Split(',').ToList();
                
                if(seq.Path.Count > 1)
                    sequences.Add(seq);
            }

            reader.Dispose();
            return sequences;
        }
    }
}

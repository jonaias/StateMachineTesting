using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using PLeTsdev;
using System.IO;
using System.Xml.Serialization;
using MethodGroup1.Fsm;

namespace MethodGroup1 {
    public class MyMethod : AbstractMethodGroup1{
        /// <summary>
        /// Plug-in default initializer.
        /// </summary>
        /// <returns></returns>
        public override PLeTsdev.AbstractMethodGroup1 Initialize() {
            Console.ForegroundColor = ConsoleColor.DarkGray;
            Console.WriteLine("Loaded plug-in " + this.GetType().Name);
            Console.ResetColor();

            return this;
        }
        /// <summary>
        /// Generate sequences from finite state machine.
        /// </summary>
        /// <param name="stream"></param>
        /// <returns></returns>
        public MemoryStream GenerateSequenceSet(MemoryStream serializedFiniteStateMachine) {
            Fsm.FiniteStateMachine machine = this.UnserializeFiniteStateMachine(serializedFiniteStateMachine);
            Method method = new Method();
            TestSequence[] sequences = method.Do(machine);

            foreach (TestSequence t in sequences) {
                Console.WriteLine(t.Name);
                foreach (string s in t.Path) {
                    Console.Write(s);
                }
            }

            string[][] matrixSequences = TestSequence.ToStringMatrix(sequences);
            MemoryStream serializedSequences = new MemoryStream();

            XmlSerializer serializer = new XmlSerializer(typeof(string[][]));
            serializer.Serialize(serializedSequences, matrixSequences);
            return serializedSequences;
        }
        /// <summary>
        /// Unserialize serialized finite state machine.
        /// </summary>
        /// <param name="serializedFiniteStateMachine"></param>
        /// <returns></returns>
        private Fsm.FiniteStateMachine UnserializeFiniteStateMachine(MemoryStream serializedFiniteStateMachine) {
            serializedFiniteStateMachine.Position = 0;
            XmlSerializer serializer = new XmlSerializer(typeof(Fsm.FiniteStateMachine));
            return (Fsm.FiniteStateMachine)serializer.Deserialize(serializedFiniteStateMachine);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Xml.Serialization;

namespace MethodGroup1.Fsm
{
    /// <summary>
    /// Represents a finite state machine model.
    /// </summary>
    [Serializable]
    public class FiniteStateMachine
    {
        #region Fields
        /// <summary>
        /// EPSILON constant. Denotes empty sets.
        /// </summary>
        public static string EPSILON  = new String('\u0190', 1);
        #endregion
        #region Properties

        /// <summary>
        /// Input alphabet property.
        /// </summary>
        [XmlElement()]
        public List<string> InputAlphabet {
            get;
            set;
        }
        /// <summary>
        /// Output alphabet property.
        /// </summary>
        [XmlElement()]
        public List<string> OutputAlphabet{
            get;
            set;
        }
        /// <summary>
        /// States property.
        /// </summary>
        [XmlElement()]
        public List<State> States { 
            get; 
            set;
        }
        /// <summary>
        /// Defines a name to the instance.
        /// </summary>
        [XmlAttribute()]
        public String Name { 
            get; 
            set; 
        }
        /// <summary>
        /// Initial State.
        /// </summary>
        [XmlElement()]
        public State InitialState { 
            get; 
            set; 
        }
        /// <summary>
        /// Transitions.
        /// </summary>
        [XmlElement()]
        public List<Transition> Transitions{
            get;
            set;
        }
        #endregion
        #region Constructors
        /// <summary>
        /// Default constructor
        /// </summary>
        public FiniteStateMachine(String name){
            this.Name = name;
            this.States = new List<State>();
            this.Transitions = new List<Transition>();
            this.InputAlphabet = new List<string>();
            this.OutputAlphabet = new List<string>();
        }
        /// <summary>
        /// Parameterless constructor. Used by serializer.
        /// </summary>
        public FiniteStateMachine()
        {
            this.States = new List<State>();
            this.Transitions = new List<Transition>();
            this.InputAlphabet = new List<string>();
            this.OutputAlphabet = new List<string>();
        }
        #endregion
        #region Methods
        /// <summary>
        /// Adds a new transition to fsm transitions data.
        /// </summary>
        /// <param name="sourceStateId">Id of state from where the transition comes. That state must be inside fsm, otherwise a exception shall be thrown.</param>
        /// <param name="targetStateId">Id of state to where the transition goes. That state must be inside fsm, otherwise a exception shall be thrown.</param>
        /// <param name="inputData">Input data needed to walk throught the transition. That data will be added to input alphabet. If null, a exception shall be thrown.</param>
        /// <param name="outputData">Output data returned when walked throught the transition. That data will be added to output alphabet.If null, a exception shall be thrown. </param>
        public Boolean AddTransition(String sourceStateName, String targetStateName, string inputData, string outputData)
        {
            State source, target;

            //looking for source state
            IEnumerable<State> foundStates = from State s in States
                                      where s.Name.Equals(sourceStateName)
                                      select s;

            if (foundStates.Count() > 0) //found!
                source = foundStates.First();
            else {
                source = new State(sourceStateName);
                AddState(source);
            }

            //looking for target state
            foundStates = from State s in States
                          where s.Name.Equals(targetStateName)
                          select s;

            if (foundStates.Count() > 0) //found!
                target = foundStates.First();
            else {
                target = new State(targetStateName);
                AddState(target);
            }

            //updates dictinaries.
            AddInput(inputData);
            AddOutput(outputData);

            //Adds transition to transition list.
            Transition transition = new Transition(source, target, inputData, outputData);

            return AddTransition(transition);
        }
        /// <summary>
        /// Add a input data to input alphabet.
        /// </summary>
        /// <param name="inputData"></param>
        public Boolean AddInput(string inputData)
        {
            if (!InputAlphabet.Contains(inputData)) {
                InputAlphabet.Add(inputData);
                return true;
            }
            return false;
        }
        /// <summary>
        /// Adds a output to output data.
        /// </summary>
        /// <param name="outputData"></param>
        public Boolean AddOutput(string outputData)
        {
            if (!OutputAlphabet.Contains(outputData)) {
                OutputAlphabet.Add(outputData);
                return true;
            }
            return false;
        }
        /// <summary>
        /// Adds a state to the state machine. Make sure that the added state has one or more trasitions with it.
        /// </summary>
        /// <param name="state">A state reference. States are identified by their names - if the machine has a state with the same name the new state wont be added.</param>
        /// <returns>Returns true if the state were added to the state list. Returns false if the machine already has the state in the list.</returns>
        public Boolean AddState(State state)
        {
            //checks if this state is alreadyin state list.
            IEnumerable<State> stateList = from s in this.States
                              where s.Equals(state)
                              select s;

            //if not exists a state with the same name (same name, same state), adds it.
            if (stateList.Count() == 0) {
                this.States.Add(state);
                return true;
            }

            //state found in list. Abort.
            return false;
        }
        /// <summary>
        /// Cleans Input and Output dictionaries generating data from current transition list.
        /// </summary>
        public void RefreshData() {
            //cleans dictionaries.
            this.InputAlphabet = new List<string>();
            this.OutputAlphabet = new List<string>();

            //Adds Input/Output to lists
            foreach (Transition t in this.Transitions) {
                if (!InputAlphabet.Contains(t.Input))
                    AddInput(t.Input);

                if (!OutputAlphabet.Contains(t.Output))
                    AddOutput(t.Output);

                if (!States.Contains(t.SourceState))
                    States.Add(t.SourceState);

                if (!States.Contains(t.TargetState))
                    States.Add(t.TargetState);
            }

            this.States = new List<State>();
            Boolean skip = false;
            foreach (Transition t in Transitions){
                skip = false;
                foreach (State s in this.States) {
                    if (t.SourceState.Name == s.Name)
                        skip = true;
                }
                if (!skip)
                    this.States.Add(t.SourceState);
                skip = false;
                foreach (State ss in this.States) {
                    if (t.TargetState.Name == ss.Name)
                        skip = true;
                }
                if (!skip)
                    this.States.Add(t.TargetState);
            }
        }
        /// <summary>
        /// Adds a existin transition to the machine´s transition list
        /// </summary>
        /// <param name="transition"></param>
        /// <returns></returns>
        public Boolean  AddTransition(Transition transition) {
            //checks if the transitions list already has the transition.
            foreach (Transition t in Transitions) {
                if (t.CompareTo(transition) == 0) { // its the same transition
                    return false;
                }
            }

            //else, adds to list.
            Transitions.Add(transition);

            //Everything works. Rest in peace.
            return true;
        }
        /// <summary>
        /// ToString implementation.
        /// </summary>
        /// <returns></returns>
        public override string ToString() {
            String msg = "\n" + Name + ":\n";

            foreach (Transition t in Transitions) {
                msg += t.SourceState.Name + " -- " + 
                    t.Input.ToString() + " / " + 
                    t.Output.ToString() + " -> " +
                    t.TargetState.Name + "\n";
            }

            return msg;
        }
        /// <summary>
        /// Enable events, used before machine minimization.
        /// </summary>
        public void EnableEvents(){

            Dictionary<string, List<string>> listStateInputs = new Dictionary<string, List<string>>();

            //group inputs in an state-input pair dictionary
            foreach(Transition t in this.Transitions)
                if (listStateInputs.Keys.Contains(t.SourceState.Name))
                    listStateInputs[t.SourceState.Name].Add(t.Input);
                else
                    listStateInputs.Add(t.SourceState.Name, new List<string>(){t.Input});
            

            //populate machine transitions with enableInputs-transitions
            foreach (KeyValuePair<string, List<String>> pair in listStateInputs) {
                string output = null;
                foreach (string s in pair.Value)
                    output += s + ",";

                output = "[" + output.Substring(0, output.Length - 1) + "]";

                State st = new State(pair.Key);
                this.Transitions.Add(new Transition(st,st, "enabledInputs", output));
            }

            //fix enableInputs for no-input states
            foreach (State s in this.States)
                if (!listStateInputs.Keys.Contains(s.Name))
                    this.Transitions.Add(new Transition(s, s, "enabledInputs", "[]"));
            Console.ForegroundColor = ConsoleColor.DarkYellow;
            foreach (Transition t in this.Transitions)
                Console.WriteLine(t.ToString());
        }
        /// <summary>
        /// Machine minimization. EnableEvents must be used before to enable this method.
        /// </summary>
        public void Minimize() {
            bool hasMinimization = false;

            #region Minimization 

            // find minimizable states
            Dictionary<string, List<string>> minimizableStates = new Dictionary<string, List<string>>();


            foreach (State s in this.States) {
                foreach (State ss in this.States) {
                    if (ss.Name != s.Name) {
                        if (this.IsMirrorState(ss, s)) {
                            if (minimizableStates.Keys.Contains(s.Name))
                                minimizableStates[s.Name].Add(ss.Name);
                            else
                                minimizableStates.Add(s.Name, new List<string>() { ss.Name });
                        }
                    }
                }
            }

            //update transitions
            for (int i = 0; i < minimizableStates.Count - 1; i++) {

                string stateName = minimizableStates.ElementAt(i).Key;
                List<string> stateEquiv = minimizableStates.ElementAt(i).Value;

                string newStateName = stateName;
                foreach (string s in stateEquiv)
                    newStateName += "|" + s;

                State mergedState = new State(newStateName);
                minimizableStates.ElementAt(i).Value.Add(stateName);

                foreach (string s in stateEquiv) {
                    minimizableStates.Remove(s);
                    State toRemove = (from State ss in this.States where ss.Name == s select ss).First();
                    
                    foreach (Transition t in this.Transitions){
                        if (t.SourceState.Equals(toRemove))
                            t.SourceState = mergedState;
                        if (t.TargetState.Equals(toRemove))
                            t.TargetState = mergedState;
                    }
                    this.States.Remove(toRemove);
                }

                

                hasMinimization = true;
            }
            List<Transition> newTransitions = new List<Transition>();
            foreach (Transition t in this.Transitions)
                if (!newTransitions.Contains(t))
                    newTransitions.Add(t);

            this.Transitions = newTransitions;
            #endregion

            //Re-apply minimination if machine has been modified
            if (hasMinimization)
                this.Minimize();
        }
        /// <summary>
        /// Check if two states are minimizable.
        /// </summary>
        /// <param name="ss"></param>
        /// <param name="s"></param>
        /// <returns></returns>
        private bool IsMirrorState(State r, State s) {
            List<Transition> rTrans = this.GetOutTransitions(r);
            List<Transition> sTrans = this.GetOutTransitions(s);

            rTrans.Sort();
            sTrans.Sort();

            if(rTrans.Count != sTrans.Count)
                return false;

            for (int i = 0; i < rTrans.Count - 1; i++) {

                if (rTrans[i].TargetState.Name != sTrans[i].TargetState.Name ||
                    rTrans[i].Input != sTrans[i].Input ||
                    rTrans[i].Output != sTrans[i].Output) {

                    if(rTrans[i].SourceState != rTrans[i].TargetState ||
                        sTrans[i].SourceState != sTrans[i].TargetState ||
                        rTrans[i].Input != sTrans[i].Input ||
                        rTrans[i].Output != sTrans[i].Output)
                        return false;
                }                    
            }
            return true;
        }
        /// <summary>
        /// Out transitions implementation.
        /// </summary>
        /// <param name="t"></param>
        /// <returns></returns>
        public List<Transition> GetOutTransitions(State tt) {
            IEnumerable<Transition> x = from Transition t in this.Transitions
                                     where t.SourceState.Name == tt.Name
                                     select t;
            return x.ToList();
        }
        /// <summary>
        /// im transitions list.
        /// </summary>
        /// <param name="t"></param>
        /// <returns></returns>
        public List<Transition> GetInTransitions(State tt) {
            IEnumerable<Transition> x = from Transition t in this.Transitions
                                        where t.TargetState.Name == tt.Name
                                        select t;
            return x.ToList();
        }
        #endregion
    }
}

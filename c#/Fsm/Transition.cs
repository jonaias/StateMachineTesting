using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace MethodGroup1.Fsm
{
    /// <summary>
    /// Represents a transition into a fsm.
    /// </summary>
    [Serializable]
    public class Transition :IComparable
    {
        /// <summary>
        /// Equals implementation.
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
        public override bool Equals(object obj) {
            Transition t = (Transition)obj;
            return (
                this.TargetState.Equals(t.TargetState) &&
                this.SourceState.Equals(t.SourceState) &&
                this.Input.Equals(t.Input) &&
                this.Output.Equals(t.Output));
        }
        /// <summary>
        /// Removes warning when overriding Equals without GetHashCode
        /// </summary>
        /// <returns></returns>
        public override int GetHashCode() {
            return base.GetHashCode();
        }

        /// <summary>
        /// From where the transition comes.
        /// </summary>
        [XmlElement()]
        public State SourceState
        {
            get;
            set;
        }
        /// <summary>
        /// To where the transitions goes.
        /// </summary>
        [XmlElement()]
        public State TargetState
        {
            get;
            set;
        }
        /// <summary>
        /// Input property.
        /// </summary>
        [XmlAttribute()]
        public string Input
        {
            get;
            set;
        }
        /// <summary>
        /// Output property.
        /// </summary>
        [XmlAttribute()]
        public string Output
        {
            get;
            set;
        }
        #region Constructors
        /// <summary>
        /// Default constructor.
        /// </summary>
        public Transition(State sourceState, State targetState, string input, string output){
            SourceState = sourceState;
            TargetState = targetState;
            Input = input;
            Output = output;
        }
        /// <summary>
        /// Parameterless constructor. Used by serializer.
        /// </summary>
        public Transition(){}
        #endregion
        /// <summary>
        /// CompareTo implementation.
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
        public int CompareTo(object obj)
        {
            Transition t = (Transition)obj;
            if (this.Equals(t))
                return 0;
            
            if(!this.SourceState.Equals(t.SourceState))
                return this.SourceState.CompareTo(t.SourceState);
            else if(!this.TargetState.Equals(t.TargetState))
                return this.TargetState.CompareTo(t.TargetState);
            else if(!this.Input.Equals(t.Input))
                return this.Input.CompareTo(t.Input);
            else 
                return this.Output.CompareTo(t.Output);
        }

        public override string ToString() {
            return SourceState.Name + " -- " + Input + " / " + Output + " -> " + TargetState.Name;
        }
    }
}

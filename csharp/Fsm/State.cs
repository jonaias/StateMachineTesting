using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace MethodGroup1.Fsm
{
    /// <summary>
    /// Represents a finite state machine state model.
    /// </summary>
    [Serializable]
    public class State: IComparable
    {
        #region Constructors
        /// <summary>
        /// Parameterless constructor. Used by XmlSerializer.
        /// </summary>
        public State()
        {

        }
        /// <summary>
        /// Default constructor.
        /// </summary>
        public State(String name)
        {
            this.Name = name;
        }
        #endregion

        /// <summary>
        /// Identifies the state on a fsm.
        /// </summary>
        [XmlAttribute()]
        public int Id
        {
            get;
            set;
        }
        /// <summary>
        /// Name property.
        /// </summary>
        [XmlAttribute()]
        public String Name
        {
            get;
            set;
        }
        #region Methods
        /// <summary>
        /// Returns true if this state is the initial state of given finite state machine.
        /// </summary>
        /// <param name="fsm"></param>
        /// <returns></returns>
        public Boolean IsInitialState(FiniteStateMachine fsm)
        {
            return (fsm.InitialState.Equals(this));
        }
        /// <summary>
        /// Compare to implementation.
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
        public int CompareTo(object obj)
        {
            if (obj.GetType().Name.Equals(this.GetType().Name))
            {
                State s = (State)obj;
                if(this.Name.Equals(s.Name))
                    return 0;
                return 1;
            }
            else{
                return -1;
            }
        }

        #endregion

        /// <summary>
        /// Equals implementation.
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
        public override bool Equals(object obj) {
            try {
                return this.Name == ((State)obj).Name;
            } catch (Exception ee) {
                ee.GetType();
                return false;
            }
        }
        /// <summary>
        /// prevents warning when not overriding GetHashCode.
        /// </summary>
        /// <returns></returns>
        public override int GetHashCode() {
            return base.GetHashCode();
        }

        int IComparable.CompareTo(object obj) {
            State x = (State)obj;
            if (this.Equals(x))
                return 0;
            else
                return this.Name.CompareTo(x.Name);
        }
    }
}

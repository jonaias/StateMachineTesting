using System;
/// <summary>
/// provides a relatioship between two states
/// </summary>
/// 
namespace MethodGroup1.Fsm
{
    public class StatePair : ICloneable, IComparable
    {
        public State A { get; set; }
        public State B { get; set; }
        public StatePair(State a, State b)
        {
            this.A = a;
            this.B = b;
        }

        public object Clone()
        {
            return this.MemberwiseClone();
        }

        public int CompareTo(object obj)
        {
            StatePair pair = (StatePair)obj;
            if (
                (pair.A.Name.Equals(this.A.Name) && pair.B.Name.Equals(this.B.Name)) ^
                (pair.A.Name.Equals(this.B.Name) && pair.B.Name.Equals(this.A.Name))
            )
            {
                return 0;
            }
            else
            {
                return -1;
            }
        }

        public bool IsAmbiguous()
        {
            return (A.Name.Equals(B.Name));
        }

        /// <summary>
        /// checks if the pair constains the given state.
        /// </summary>
        /// <param name="state"></param>
        /// <returns></returns>
        public bool Contains(State state)
        {
            return (state.Name.Equals(A.Name) || state.Name.Equals(B.Name));
        }
    }
}
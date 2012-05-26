module FSM
    where

import Data.List

----------- State ------------
data State = State String

instance Show State where
    show (State s) = s

instance Eq State where
    (State s) == (State s') = s == s'

--------- Transition ---------
type Input = String

type Output = String

type IOPair = (Input, Output)

data Transition = Transition State State Input Output

instance Show Transition where
    show (Transition s0 s1 _in _out) = "[" ++ (show s0) ++ ", " ++ (show s1) ++ ", " ++ _in ++ ", " ++ _out ++ "]"

instance Eq Transition where
    (Transition s0 s1 _in _out) == (Transition s0' s1' _in' _out') = ((s0 == s0') && (s1 == s1') && (_in == _in') && (_out == _out'))

------------- FSM ------------
data FSM = FSM [State] [Transition]

instance Show FSM where
    show (FSM states transitions) = "FSM: { States:  " ++ (show states) ++ " , Transitions: " ++ (show transitions) ++ " }"

---------- FUNCTIONS ---------
emptyFSM :: FSM
emptyFSM = FSM [] []

addState :: FSM -> State -> FSM
addState (FSM states transitions) newState = FSM (newState:states) transitions

addTransition :: FSM -> Transition -> FSM
addTransition (FSM states transitions) newTransition = FSM states (newTransition:transitions)

getStates :: FSM -> [State]
getStates (FSM states _) = states

getTransitions :: FSM -> [Transition]
getTransitions (FSM _ transitions) = transitions

getInputs :: FSM -> [Input]
getInputs (FSM _ transitions) = nub [i | i <- map (\(Transition _ _ i' _) -> i') transitions]

getOutputs :: FSM -> [Output]
getOutputs (FSM _ transitions) = nub [i | i <- map (\(Transition _ _ _ o') -> o') transitions]

getTransitionsExiting :: FSM -> State -> [Transition]
getTransitionsExiting (FSM _ transitions) s = getTransitionsExiting' transitions s

getTransitionsExiting' :: [Transition] -> State -> [Transition]
getTransitionsExiting' transitions s = filter ((\s0 (Transition s0' _ _ _) -> s0 == s0') s) transitions

getActualInputs :: FSM -> State -> [Input]
getActualInputs m state =   map f (getTransitionsExiting m state)
                            where f (Transition _ _ input _) = input

getActualInputsL :: FSM -> [State] -> [Input]
getActualInputsL m states = nub (foldl (++) [] [(getActualInputs m state) | state <- states])

getNextStates :: FSM -> State -> [State]
getNextStates (FSM states transitions) s = [s' | s' <- map (\(Transition _ s1 _ _) -> s1) (getTransitionsExiting' transitions s)]

getNextStatesOnInput :: FSM -> State -> Input -> [State]
getNextStatesOnInput (FSM states transitions) s input = [s' | s' <- map (\(Transition _ s1 _ _) -> s1) (filter (\(Transition _ _ input' _) -> input' == input) (getTransitionsExiting' transitions s))]

getNextStatesOnInputL :: FSM -> [State] -> Input -> [State]
getNextStatesOnInputL m states input = nub (foldl (++) [] [(getNextStatesOnInput m state input) | state <- states])

getInitialState :: FSM -> State
getInitialState (FSM states _) = last states

getFinalState :: FSM -> State -> [Input] -> State
getFinalState (FSM states transitions) state (i:is) | ((i:is) == []) = state
                                                    | otherwise = (getFinalState (FSM states transitions) (head (getNextStatesOnInput (FSM states transitions) state i)) is)

getSynchronizationSequence :: FSM -> State -> [Input]
getSynchronizationSequence (FSM states transitions) targetState = getSynchronizationSequence' (FSM states transitions) states [] targetState []

getSynchronizationSequence' m states inputs targetState visited | states `elem` visited = []
                                                                | (states == [targetState]) = inputs
                                                                | ((states /= [targetState]) && ((length states) == 1)) = []
                                                                | otherwise = (\l -> if null l then [] else head l) (filter (\l -> l /= []) [(getSynchronizationSequence' m (getNextStatesOnInputL m states i) (i:inputs) targetState (states:visited)) | i <- sort (getActualInputsL m states)])

getSeparatingSequence :: FSM -> State -> State -> [Input]
getSeparatingSequence _ _ _ = []

getReachingSequence :: FSM -> State -> State -> [IOPair]
getReachingSequence _ _ _ = []

getSetSequence :: FSM -> State -> [IOPair]
getSetSequence _ _ = []

getStatusSequence :: FSM -> State -> [IOPair]
getStatusSequence _ _ = []

getResetSequence :: FSM -> [IOPair]
---- getSyncSeq com o último da lista de estados

getResetSequence _ = []

getTestSequence :: FSM -> [IOPair]
getTestSequence _ = []

readFromFile :: String -> IO FSM
readFromFile path = do
    let s0 = State "1"
    let s1 = State "2"
    let s2 = State "3"
    let s3 = State "4"
    let t0 = Transition s0 s1 "b" "0"
    let t1 = Transition s0 s2 "a" "1"
    let t2 = Transition s1 s3 "b" "0"
    let t3 = Transition s1 s0 "a" "1"
    let t4 = Transition s2 s3 "b" "1"
    let t5 = Transition s2 s2 "a" "1"
    let t6 = Transition s3 s3 "b" "0"
    let t7 = Transition s3 s2 "a" "0"
    let m = FSM [s0, s1, s2, s3] [t0, t1, t2, t3, t4, t5, t6, t7]
    return m
    --putStrLn (show m)
    --putStrLn (show (getNextStates m s0))

main :: IO ()
main = do
    m <- readFromFile ""
    (putStrLn.show) (getSynchronizationSequence m (State "1"))
    (putStrLn.show) (getSynchronizationSequence m (State "2"))
    (putStrLn.show) (getSynchronizationSequence m (State "3"))
    (putStrLn.show) (getSynchronizationSequence m (State "4"))

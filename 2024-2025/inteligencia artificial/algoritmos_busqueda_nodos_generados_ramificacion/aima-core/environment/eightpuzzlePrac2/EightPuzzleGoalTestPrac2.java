package aima.core.environment.eightpuzzlePrac2;

import aima.core.search.framework.GoalTest;


//public static

public class EightPuzzleGoalTestPrac2 implements GoalTest {
	
	EightPuzzleBoardPrac2 goal;

	public EightPuzzleGoalTestPrac2() {}
	
	public EightPuzzleGoalTestPrac2(EightPuzzleBoardPrac2 goalstate) {
		this.goal = goalstate;
	}

	public boolean isGoalState(Object state) {
		return ((EightPuzzleBoardPrac2) state).equals(goal);
	}
}
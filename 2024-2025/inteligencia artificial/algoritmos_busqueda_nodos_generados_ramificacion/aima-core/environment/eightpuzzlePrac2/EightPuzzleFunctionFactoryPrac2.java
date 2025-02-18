package aima.core.environment.eightpuzzlePrac2;

import java.util.LinkedHashSet;
import java.util.Set;

import aima.core.agent.Action;
import aima.core.search.framework.ActionsFunction;
import aima.core.search.framework.ResultFunction;

/**
 * @author Ravi Mohan
 * @author Ciaran O'Reilly
 */
public class EightPuzzleFunctionFactoryPrac2 {
	private static ActionsFunction _actionsFunction = null;
	private static ResultFunction _resultFunction = null;

	public static ActionsFunction getActionsFunction() {
		if (null == _actionsFunction) {
			_actionsFunction = new EPActionsFunction();
		}
		return _actionsFunction;
	}

	public static ResultFunction getResultFunction() {
		if (null == _resultFunction) {
			_resultFunction = new EPResultFunction();
		}
		return _resultFunction;
	}

	private static class EPActionsFunction implements ActionsFunction {
		public Set<Action> actions(Object state) {
			EightPuzzleBoardPrac2 board = (EightPuzzleBoardPrac2) state;

			Set<Action> actions = new LinkedHashSet<Action>();

			if (board.canMoveGap(EightPuzzleBoardPrac2.UP)) {
				actions.add(EightPuzzleBoardPrac2.UP);
			}
			if (board.canMoveGap(EightPuzzleBoardPrac2.DOWN)) {
				actions.add(EightPuzzleBoardPrac2.DOWN);
			}
			if (board.canMoveGap(EightPuzzleBoardPrac2.LEFT)) {
				actions.add(EightPuzzleBoardPrac2.LEFT);
			}
			if (board.canMoveGap(EightPuzzleBoardPrac2.RIGHT)) {
				actions.add(EightPuzzleBoardPrac2.RIGHT);
			}

			return actions;
		}
	}

	private static class EPResultFunction implements ResultFunction {
		public Object result(Object s, Action a) {
			EightPuzzleBoardPrac2 board = (EightPuzzleBoardPrac2) s;

			if (EightPuzzleBoardPrac2.UP.equals(a)
					&& board.canMoveGap(EightPuzzleBoardPrac2.UP)) {
				EightPuzzleBoardPrac2 newBoard = new EightPuzzleBoardPrac2(board);
				newBoard.moveGapUp();
				return newBoard;
			} else if (EightPuzzleBoardPrac2.DOWN.equals(a)
					&& board.canMoveGap(EightPuzzleBoardPrac2.DOWN)) {
				EightPuzzleBoardPrac2 newBoard = new EightPuzzleBoardPrac2(board);
				newBoard.moveGapDown();
				return newBoard;
			} else if (EightPuzzleBoardPrac2.LEFT.equals(a)
					&& board.canMoveGap(EightPuzzleBoardPrac2.LEFT)) {
				EightPuzzleBoardPrac2 newBoard = new EightPuzzleBoardPrac2(board);
				newBoard.moveGapLeft();
				return newBoard;
			} else if (EightPuzzleBoardPrac2.RIGHT.equals(a)
					&& board.canMoveGap(EightPuzzleBoardPrac2.RIGHT)) {
				EightPuzzleBoardPrac2 newBoard = new EightPuzzleBoardPrac2(board);
				newBoard.moveGapRight();
				return newBoard;
			}

			// The Action is not understood or is a NoOp
			// the result will be the current state.
			return s;
		}
	}
}
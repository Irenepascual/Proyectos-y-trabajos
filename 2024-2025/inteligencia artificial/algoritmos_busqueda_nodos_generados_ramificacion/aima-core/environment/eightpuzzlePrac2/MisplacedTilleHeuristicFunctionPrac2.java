package aima.core.environment.eightpuzzlePrac2;

import aima.core.search.framework.HeuristicFunction;
import aima.core.util.datastructure.XYLocation;

/**
 * @author Ravi Mohan
 * 
 */
public class MisplacedTilleHeuristicFunctionPrac2 implements HeuristicFunction {

	public EightPuzzleBoardPrac2 fin;
	
	public MisplacedTilleHeuristicFunctionPrac2(EightPuzzleBoardPrac2 EstadoFin) {
		fin=EstadoFin;
	}
	
	public double h(Object state) {
		EightPuzzleBoardPrac2 board = (EightPuzzleBoardPrac2) state;
		return getNumberOfMisplacedTiles(board);
	}

	private int getNumberOfMisplacedTiles(EightPuzzleBoardPrac2 board) {
		int numberOfMisplacedTiles = 0;
		
		for (int i = 0; i < 9; i++) {
			if (!(board.getLocationOf(i).equals(fin.getLocationOf(i)))) {
				numberOfMisplacedTiles++;
			}
		}
		
		if(numberOfMisplacedTiles > 0) {
			numberOfMisplacedTiles--;
		}
		
		return numberOfMisplacedTiles;
	}
}
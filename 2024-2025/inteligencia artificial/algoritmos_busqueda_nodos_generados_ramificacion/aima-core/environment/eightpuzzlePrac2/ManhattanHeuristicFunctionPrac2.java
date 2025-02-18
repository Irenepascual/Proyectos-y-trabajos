package aima.core.environment.eightpuzzlePrac2;

import aima.core.search.framework.HeuristicFunction;
import aima.core.util.datastructure.XYLocation;

/**
 * @author Ravi Mohan
 * 
 */
public class ManhattanHeuristicFunctionPrac2 implements HeuristicFunction {
	
	EightPuzzleBoardPrac2 fin;
	
	public ManhattanHeuristicFunctionPrac2(EightPuzzleBoardPrac2 EstadoFin) {
		fin=EstadoFin;
	}
	
	public double h(Object state) {
		EightPuzzleBoardPrac2 board = (EightPuzzleBoardPrac2) state;
		int retVal = 0;
		XYLocation locEstIni, locEstFin;
		int i;
		for (i = 1; i < 9; i++) {
			locEstIni = board.getLocationOf(i);
			locEstFin = fin.getLocationOf(i);
			retVal += evaluateManhattanDistanceOf(locEstIni, locEstFin);
		}

		return retVal;
	}

	public int evaluateManhattanDistanceOf(XYLocation locEstIni, XYLocation locEstFin) {
		int xpos = locEstIni.getXCoOrdinate();
		int ypos = locEstIni.getYCoOrdinate();
	    int xposFin = locEstFin.getXCoOrdinate();
	    int yposFin = locEstFin.getYCoOrdinate();
		
	    int dist = Math.abs(xpos - xposFin) + Math.abs(ypos - yposFin);

	    return dist;
	}
}
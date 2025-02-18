package aima.gui.demo.search;

import java.util.Iterator;
import java.util.List;
import java.util.Properties;
import aima.core.agent.Action;
import aima.core.environment.eightpuzzle.EightPuzzleBoard;
import aima.core.environment.eightpuzzle.EightPuzzleFunctionFactory;
import aima.core.environment.eightpuzzle.EightPuzzleGoalTest;
import aima.core.search.framework.GraphSearch;
import aima.core.search.framework.Problem;
import aima.core.search.framework.QueueSearch;
import aima.core.search.framework.Search;
import aima.core.search.framework.SearchAgent;
import aima.core.search.framework.TreeSearch;
import aima.core.search.uninformed.BreadthFirstSearch;
import aima.core.search.uninformed.DepthFirstSearch;
import aima.core.search.uninformed.DepthLimitedSearch;
import aima.core.search.uninformed.IterativeDeepeningSearch;
import aima.core.search.uninformed.UniformCostSearch;

/**
 * @author Ravi Mohan
 * 
 */

public class EightPuzzleDemo {
	static EightPuzzleBoard boardWithThreeMoveSolution = new EightPuzzleBoard(
			new int[] { 1, 2, 5, 3, 4, 0, 6, 7, 8 });;		//3
			
	static EightPuzzleBoard random1 = new EightPuzzleBoard(new int[] { 1, 4, 2,
			7, 5, 8, 3, 0, 6 });	//9

	static EightPuzzleBoard extreme = new EightPuzzleBoard(new int[] { 0, 8, 7,
			6, 5, 4, 3, 2, 1 });	//30

	public static void main(String[] args) {
		
		System.out.printf("%8s|%11s|%11s|%11s|%11s|%11s|%11s|%11s|%42s|%42s\n","Problema","Profundidad","Expand","Q.Size","MaxQS","tiempo","Generados","Ritmo","Complejidad temp", "Complejidad esp");
		System.out.printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		eightPuzzleBFSDemo("BFS-G-3",boardWithThreeMoveSolution,new GraphSearch(),true,3);
		eightPuzzleBFSDemo("BFS-T-3",boardWithThreeMoveSolution,new TreeSearch(),true,3);
		eightPuzzleDFSDemo("DFS-G-3",boardWithThreeMoveSolution,new GraphSearch(),true,3);
		eightPuzzleDFSDemo("DFS-T-3",boardWithThreeMoveSolution,new TreeSearch(),false,3);
		eightPuzzleDLSDemo("DLS-9-3",boardWithThreeMoveSolution,9);
		eightPuzzleDLSDemo("DLS-3-3",boardWithThreeMoveSolution,3);
		eightPuzzleIDSDemo("IDS-3",boardWithThreeMoveSolution,9);
		eightPuzzleUCDemo("UCS-G-3",boardWithThreeMoveSolution,new GraphSearch(),3);
		eightPuzzleUCDemo("UCS-T-3",boardWithThreeMoveSolution,new TreeSearch(),3);
		
		eightPuzzleBFSDemo("BFS-G-9",random1,new GraphSearch(),true,30);
		eightPuzzleBFSDemo("BFS-T-9",random1,new TreeSearch(),true,30);
		eightPuzzleDFSDemo("DFS-G-9",random1,new GraphSearch(),true,9);
		eightPuzzleDFSDemo("DFS-T-9",random1,new TreeSearch(),false,9);
		eightPuzzleDLSDemo("DLS-9-9",random1,9);
		eightPuzzleDLSDemo("DLS-3-9",random1,3);
		eightPuzzleIDSDemo("IDS-9",random1,9);
		eightPuzzleUCDemo("UCS-G-9",random1,new GraphSearch(),9);
		eightPuzzleUCDemo("UCS-T-9",random1,new TreeSearch(),9);
		
		eightPuzzleBFSDemo("BFS-G-30",extreme,new GraphSearch(),true,30);
		eightPuzzleBFSDemo("BFS-T-30",extreme,new TreeSearch(),false,30);
		eightPuzzleDFSDemo("DFS-G-30",extreme,new GraphSearch(),true,30);
		eightPuzzleDFSDemo("DFS-T-30",extreme,new TreeSearch(),false,30);
	}
// getAct	le das un estado y te devuelve las acciones posibles
// getRes	le das un estado y una acción y te devuelve el estado resultante
	private static void eightPuzzleDLSDemo(String text, Object tablero, int profundidad) {
		try {
			Problem problem = new Problem(tablero, EightPuzzleFunctionFactory
					.getActionsFunction(), EightPuzzleFunctionFactory
					.getResultFunction(), new EightPuzzleGoalTest());
			Search search = new DepthLimitedSearch(profundidad);
			long ti = System.currentTimeMillis();
			SearchAgent agent = new SearchAgent(problem, search);
			ti = System.currentTimeMillis() - ti;
			
			int b=4;
			int l=profundidad;
			long compl_temp=(long)Math.pow(b,l);
			long compl_esp=(long)b*l;
			String compl_t="O(b^m) -> ";
			String compl_e="O(b*m) -> ";
			printPractica1(agent,text,ti,compl_t,compl_e, compl_temp, compl_esp);
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static void eightPuzzleIDSDemo(String text, Object tablero,int profundidad) {
		try {
			Problem problem = new Problem(tablero, EightPuzzleFunctionFactory
					.getActionsFunction(), EightPuzzleFunctionFactory
					.getResultFunction(), new EightPuzzleGoalTest());
			Search search = new IterativeDeepeningSearch();
			long ti = System.currentTimeMillis();
			SearchAgent agent = new SearchAgent(problem, search);
			ti = System.currentTimeMillis() - ti;
			
			int b=4;
			int m=profundidad;
			long compl_temp=(long)Math.pow(b, m);
			long compl_esp=(long)b*m;
			String compl_t="O(b^m) -> ";
			String compl_e="O(b*m) -> ";

			printPractica1(agent,text,ti,compl_t,compl_e, compl_temp, compl_esp);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private static void eightPuzzleBFSDemo(String text, Object tablero, QueueSearch grafoArbol, Boolean ejecutar,int profundidad) {
		try {
			if (ejecutar) {
				Problem problem = new Problem(tablero, EightPuzzleFunctionFactory
						.getActionsFunction(), EightPuzzleFunctionFactory
						.getResultFunction(), new EightPuzzleGoalTest());
				Search search = new BreadthFirstSearch(grafoArbol);
				long ti = System.currentTimeMillis();
				SearchAgent agent = new SearchAgent(problem,search);
				ti = System.currentTimeMillis() - ti;
				int b=4;
				int d=profundidad;
				long compl_temp=(long)Math.pow(b,(profundidad+1));
				long compl_esp=(long)Math.pow(b,(profundidad+1));
				String compl_t="O(b^(d) -> ";
				String compl_e="O(b^(d) -> ";
				
				printPractica1(agent,text,ti,compl_t,compl_e, compl_temp, compl_esp);
			}
			else {
				System.out.printf("%8s|%11s|%11s|%11s|%11s|%11s|%11s|%11s|%42s|%42s\n",text,"---","---","---","---","(2)","---","---","---","---");
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private static void eightPuzzleDFSDemo(String text, Object tablero, QueueSearch grafoArbol, Boolean ejecutar,int profundidad) {
		try {
			if (ejecutar) {
				Problem problem = new Problem(tablero, EightPuzzleFunctionFactory
						.getActionsFunction(), EightPuzzleFunctionFactory
						.getResultFunction(), new EightPuzzleGoalTest());
				Search search = new DepthFirstSearch(grafoArbol);
				long ti = System.currentTimeMillis();
				SearchAgent agent = new SearchAgent(problem, search);
				ti = System.currentTimeMillis() - ti;
				int b=4;
				int m=profundidad;
				long compl_temp=(long)Math.pow(b, profundidad);
				long compl_esp=(long)4*3;
				String compl_t="O(b^m) -> ";
				String compl_e="O(b*m) -> ";
				
				printPractica1(agent,text,ti,compl_t,compl_e, compl_temp, compl_esp);
			}
			else {
				System.out.printf("%8s|%11s|%11s|%11s|%11s|%11s|%11s|%11s|%42s|%42s\n",text,"---","---","---","---","(1)","---","---","---","---");
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private static void eightPuzzleUCDemo(String text, Object tablero, QueueSearch grafoArbol, int profundidad) {
		try {
			Problem problem = new Problem(tablero, EightPuzzleFunctionFactory
					.getActionsFunction(), EightPuzzleFunctionFactory
					.getResultFunction(), new EightPuzzleGoalTest());
			Search search = new UniformCostSearch(grafoArbol);
			long ti = System.currentTimeMillis();
			SearchAgent agent = new SearchAgent(problem, search);
			ti = System.currentTimeMillis() - ti;
			int b=4;
			long compl_temp=(long)Math.pow(b,1+(profundidad/1));
			long compl_esp=(long)Math.pow(b,1+(profundidad/1));
			String compl_t="O(b^(1+C*/e)) -> ";
			String compl_e="O(b^(1+C*/e)) -> ";
			
			printPractica1(agent,text,ti,compl_t,compl_e, compl_temp, compl_esp);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}


	private static void printPractica1(SearchAgent agent, String text, long ti, String compl_t, String compl_e, long compl_temp, long compl_esp) {
		int depth, expandedNodes, queueSize, maxQueueSize, generatedNodes;
		String pathcostM =agent.getInstrumentation().getProperty("pathCost");
		
		if (pathcostM==null) {
			depth = 0;
		}
		else depth = (int)Float.parseFloat(pathcostM);

		
		if (agent.getInstrumentation().getProperty("nodesExpanded")==null) {
			expandedNodes= 0;
		}
		else expandedNodes = (int)Float.parseFloat(agent.getInstrumentation().getProperty("nodesExpanded"));
		
		if (agent.getInstrumentation().getProperty("queueSize")==null) {
			queueSize=0;
		}
		else queueSize = (int)Float.parseFloat(agent.getInstrumentation().getProperty("queueSize"));
		
		if (agent.getInstrumentation().getProperty("maxQueueSize")==null) {
			maxQueueSize= 0;
		}
		else maxQueueSize = (int)Float.parseFloat(agent.getInstrumentation().getProperty("maxQueueSize"));
		
		if (agent.getInstrumentation().getProperty("nodesGenerated")==null) {
			generatedNodes= 0;
		}
		else generatedNodes = (int)Float.parseFloat(agent.getInstrumentation().getProperty("nodesGenerated"));
		
		double seg = (double)ti/1000.0;
		double ritmo = (double)generatedNodes / seg;
		
		
		System.out.printf("%8s|%11d|%11d|%11d|%11d|%11d|%11d|%11.2f|%19s%23d|%19s%23d\n",text,depth,expandedNodes,queueSize,maxQueueSize,ti,generatedNodes,ritmo,compl_t,compl_temp, compl_e, compl_esp);
	}

}


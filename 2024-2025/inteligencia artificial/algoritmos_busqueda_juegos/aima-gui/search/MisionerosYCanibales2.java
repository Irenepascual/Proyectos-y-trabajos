package aima.gui.demo.search;
import java.util.List;
import java.util.Iterator;

import aima.core.agent.Action;
import aima.core.environment.canibales.MisionerosYCanibales;
import aima.core.environment.canibales.MisionerosYCanibalesFunctionFactory;
import aima.core.environment.canibales.MisionerosYCanibalesGoalTest;
import aima.core.environment.canibales.ManhattanHeuristicFunction;
import aima.core.environment.canibales.MisplacedTilleHeuristicFunction;
import aima.core.search.framework.GraphSearch;
import aima.core.search.framework.QueueSearch;
import aima.core.search.framework.TreeSearch;
import aima.core.search.framework.Problem;
import aima.core.search.framework.ResultFunction;
import aima.core.search.framework.Search;
import aima.core.search.framework.SearchAgent;
import aima.core.search.informed.AStarSearch;
import aima.core.search.informed.GreedyBestFirstSearch;
import aima.core.search.local.SimulatedAnnealingSearch;
import aima.core.search.uninformed.BreadthFirstSearch;
import aima.core.search.uninformed.DepthLimitedSearch;
import aima.core.search.uninformed.IterativeDeepeningSearch;
import aima.core.search.uninformed.UniformCostSearch;

/**
 * @author Ravi Mohan
 * @author R. Lunde
 */
public class MisionerosYCanibales2 {
	static MisionerosYCanibales estadoInicial = new MisionerosYCanibales(new int[] {3,3,0,0,0});

			public static void executeActions(List<Action> actions, Problem problem) {
				Object initialState = problem.getInitialState();
				ResultFunction resultFunction = problem.getResultFunction();
				Object state = initialState;
				System.out.println("   GOAL STATE");
				System.out.println("   RIBERA-IZQ		--RIO-- BOTE  M M M C C C    RIBERA-DCH");
				System.out.println("   CAMINO ENCONTRADO: ");
				System.out.println("          ESTADO INICIAL:    "+ state +"\n");
				for (Action action : actions) {
					 System.out.printf("    %22s   %22s\n", action.toString(), resultFunction.result(state, action));
				}
			}
			
	public static void main(String[] args) {
		Canibales_BFSdemo("BFS",new GraphSearch());
		Canibales_DLSdemo("DLS");
		Canibales_IDSdemo("IDLS");  
	}

	private static void Canibales_BFSdemo(String text, QueueSearch grafoArbol) {
		System.out.println("\nMisioneros y canibales " + text + "->");
		try {
				Problem problem = new Problem(estadoInicial, MisionerosYCanibalesFunctionFactory
						.getActionsFunction(), MisionerosYCanibalesFunctionFactory
						.getResultFunction(), new MisionerosYCanibalesGoalTest());
				Search search = new BreadthFirstSearch(grafoArbol);
				long ti = System.currentTimeMillis();
				SearchAgent agent = new SearchAgent(problem,search);
				ti = System.currentTimeMillis() - ti;

				Iterator<Object> i = agent.getInstrumentation().keySet().iterator();
				while (i.hasNext()) {
					String dato = (String) i.next();
					String property = agent.getInstrumentation().getProperty(dato);
					System.out.println(dato + " : " + property);
				}
				System.out.println("Tiempo : " + ti +"mls");
				System.out.println("\nSOLUCIÓN:");
				executeActions(agent.getActions(),problem);
				System.out.println("----------------------------------------------------------------------------");
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
	
	private static void Canibales_DLSdemo(String text) {
		System.out.println("\nMisioneros y canibales " + text + "(11) ->");
		try {
			Problem problem = new Problem(estadoInicial, MisionerosYCanibalesFunctionFactory
					.getActionsFunction(), MisionerosYCanibalesFunctionFactory
					.getResultFunction(), new MisionerosYCanibalesGoalTest());
			Search search = new DepthLimitedSearch(11);
			long ti = System.currentTimeMillis();
			SearchAgent agent = new SearchAgent(problem, search);
			ti = System.currentTimeMillis() - ti;

			Iterator<Object> i = agent.getInstrumentation().keySet().iterator();
			while (i.hasNext()) {
				String dato = (String) i.next();
				String property = agent.getInstrumentation().getProperty(dato);
				System.out.println(dato + " : " + property);
			}
			System.out.println("Tiempo : " + ti +"mls");
			System.out.println("\nSOLUCIÓN:");
			executeActions(agent.getActions(),problem);
			System.out.println("----------------------------------------------------------------------------");
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private static void Canibales_IDSdemo(String text) {
		System.out.println("\nMisioneros y canibales " + text + "->");		
		try {
			Problem problem = new Problem(estadoInicial, MisionerosYCanibalesFunctionFactory
					.getActionsFunction(), MisionerosYCanibalesFunctionFactory
					.getResultFunction(), new MisionerosYCanibalesGoalTest());
			Search search = new IterativeDeepeningSearch();
			long ti = System.currentTimeMillis();
			SearchAgent agent = new SearchAgent(problem, search);
			ti = System.currentTimeMillis() - ti;
			
			Iterator<Object> i = agent.getInstrumentation().keySet().iterator();
			while (i.hasNext()) {
				String dato = (String) i.next();
				String property = agent.getInstrumentation().getProperty(dato);
				System.out.println(dato + " : " + property);
			}
			System.out.println("Tiempo : " + ti +"mls");
			System.out.println("\nSOLUCIÓN:");
			executeActions(agent.getActions(),problem);
			System.out.println("----------------------------------------------------------------------------");
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
	
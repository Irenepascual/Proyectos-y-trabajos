package aima.gui.demo.search;

import java.math.BigDecimal;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Properties;
import java.util.Random;
import java.util.Set;

import aima.core.agent.Action;
import aima.core.environment.nqueens.AttackingPairsHeuristic;
import aima.core.environment.nqueens.NQueensBoard;
import aima.core.environment.nqueens.NQueensFitnessFunction;
import aima.core.environment.nqueens.NQueensFunctionFactory;
import aima.core.environment.nqueens.NQueensGoalTest;
import aima.core.search.framework.GraphSearch;
import aima.core.search.framework.Problem;
import aima.core.search.framework.Search;
import aima.core.search.framework.SearchAgent;
import aima.core.search.framework.TreeSearch;
import aima.core.search.local.GeneticAlgorithm;
import aima.core.search.local.HillClimbingSearch;
import aima.core.search.local.Individual;
import aima.core.search.local.Scheduler;
import aima.core.search.local.SimulatedAnnealingSearch;
import aima.core.search.uninformed.BreadthFirstSearch;
import aima.core.search.uninformed.DepthFirstSearch;
import aima.core.search.uninformed.DepthLimitedSearch;
import aima.core.search.uninformed.IterativeDeepeningSearch;
import aima.core.util.datastructure.XYLocation;


/**
 * @author Ravi Mohan
 * 
 */

public class NQueensDemo {

	private static final int _boardSize = 8;
	
	public static void main(String[] args) {

		newNQueensDemo();
	}

	private static void newNQueensDemo() {

		nQueensWithDepthFirstSearch();
		nQueensWithBreadthFirstSearch();
		nQueensWithRecursiveDLS();
		nQueensWithIterativeDeepeningSearch();
		nQueensSimulatedAnnealingSearch();
		nQueensHillClimbingSearch();
		nQueensRandomRestartHillClimbing();
		nQueensGeneticAlgorithmSearch();
	}
	
	public static NQueensBoard generateRandomNqueensBoard (int boardSize) {
        NQueensBoard board = new NQueensBoard(boardSize);
        for (int i = 0; i < boardSize; i++) {
            board.addQueenAt(new XYLocation(i, new Random().nextInt(boardSize)));
        }
        return board;
    }

    public static Set<NQueensBoard> generateSetNqueensBoard(int boardSize, int populationSize) {
        Set<NQueensBoard> setGeneratedNQueens = new HashSet<NQueensBoard>();

        while(setGeneratedNQueens.size()<populationSize) {
            setGeneratedNQueens.add(generateRandomNqueensBoard(_boardSize));
        }
        return setGeneratedNQueens;
    }
	
	
	private static void nQueensWithRecursiveDLS() {
		System.out.println("\nNQueensDemo recursive DLS -->");
		try {
			Problem problem = new Problem(new NQueensBoard(_boardSize),
					NQueensFunctionFactory.getIActionsFunction(),
					NQueensFunctionFactory.getResultFunction(),
					new NQueensGoalTest());
			Search search = new DepthLimitedSearch(_boardSize);
			SearchAgent agent = new SearchAgent(problem, search);
			printActions(agent.getActions());
			printInstrumentation(agent.getInstrumentation());
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	private static void nQueensWithBreadthFirstSearch() {
		try {
			System.out.println("\nNQueensDemo BFS -->");
			Problem problem = new Problem(new NQueensBoard(_boardSize),
					NQueensFunctionFactory.getIActionsFunction(),
					NQueensFunctionFactory.getResultFunction(),
					new NQueensGoalTest());
			Search search = new BreadthFirstSearch(new TreeSearch());
			SearchAgent agent2 = new SearchAgent(problem, search);
			printActions(agent2.getActions());
			printInstrumentation(agent2.getInstrumentation());
		} catch (Exception e1) {

			e1.printStackTrace();
		}
	}

	private static void nQueensWithDepthFirstSearch() {
		System.out.println("\nNQueensDemo DFS -->");
		try {
			Problem problem = new Problem(new NQueensBoard(_boardSize),
					NQueensFunctionFactory.getIActionsFunction(),
					NQueensFunctionFactory.getResultFunction(),
					new NQueensGoalTest());
			Search search = new DepthFirstSearch(new GraphSearch());
			SearchAgent agent = new SearchAgent(problem, search);
			printActions(agent.getActions());
			printInstrumentation(agent.getInstrumentation());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static void nQueensWithIterativeDeepeningSearch() {
		System.out.println("\nNQueensDemo Iterative DS  -->");
		try {
			Problem problem = new Problem(new NQueensBoard(_boardSize),
					NQueensFunctionFactory.getIActionsFunction(),
					NQueensFunctionFactory.getResultFunction(),
					new NQueensGoalTest());
			Search search = new IterativeDeepeningSearch();
			SearchAgent agent = new SearchAgent(problem, search);

			System.out.println();
			printActions(agent.getActions());
			printInstrumentation(agent.getInstrumentation());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static void nQueensSimulatedAnnealingSearch() {
		int num_experimentos=1000;
		System.out.println("\nNQueens SimulatedAnnealing with 1000 different initial states -->");
        double por_fallos = 0.0;
        double media_fallos = 0.0;
        double por_exitos = 0.0;
        double media_exitos = 0.0;
		
        SimulatedAnnealingSearch search = null;
		Scheduler schedule = new Scheduler(10, 0.1, 500);
		System.out.println("Parámetros Scheduler: 10, 0.1, 500");
		for (int i=0; i< num_experimentos; i++) {
			try {
				Problem problem = new Problem(generateRandomNqueensBoard(_boardSize),
						NQueensFunctionFactory.getCActionsFunction(),
						NQueensFunctionFactory.getResultFunction(),
						new NQueensGoalTest());
				search = new SimulatedAnnealingSearch(new
						AttackingPairsHeuristic(), schedule);
				SearchAgent agent = new SearchAgent(problem, search);
				if(search.getOutcome().toString().contentEquals("SOLUTION_FOUND")) {
                    por_exitos++;
                    media_exitos += agent.getActions().size();}
				else {
                    por_fallos++;
                    media_fallos += agent.getActions().size();;
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
        media_fallos = por_fallos == 0 ? 0 : media_fallos / por_fallos;
        media_exitos = por_exitos == 0 ? 0 : media_exitos / por_exitos;
        por_fallos = (por_fallos / (double) num_experimentos) * 100.0;
        por_exitos = (por_exitos / (double) num_experimentos) * 100.0;

        System.out.println("Search Outcome=" + search.getOutcome());
        System.out.println("Final State=\n" + search.getLastSearchState());

        System.out.printf("Fallos: %.2f%%\n", por_fallos);
        System.out.printf("Coste medio fallos: %.2f\n", media_fallos);
        System.out.printf("Éxitos: %.2f%%\n", por_exitos);
        System.out.printf("Coste medio éxitos: %.2f\n", media_exitos);
	} 


	private static void nQueensHillClimbingSearch() {
		System.out.println("\nNQueensDemo HillClimbing con 10000 estados iniciales diferentes-->");

		double por_fallos=0.0;
		double media_fallos=0.0;
		double por_exitos=0.0;
		double media_exitos=0.0;

		int num_experimentos=10000;
		HillClimbingSearch search = null;
	    Set<NQueensBoard> estadosIniciales = generateSetNqueensBoard(_boardSize, num_experimentos);

	    for (NQueensBoard board : estadosIniciales) {
			try {
				Problem problem = new Problem(board,
						NQueensFunctionFactory.getCActionsFunction(),
						NQueensFunctionFactory.getResultFunction(),
						new NQueensGoalTest());
				 search = new HillClimbingSearch(
						new AttackingPairsHeuristic());
				SearchAgent agent = new SearchAgent(problem, search);
				
				if(search.getOutcome().toString().contentEquals("SOLUTION_FOUND"))
				{
					por_exitos++;
					media_exitos += agent.getActions().size();
				}
				else
				{
					por_fallos++;
					media_fallos += agent.getActions().size();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
	    }

		media_fallos=media_fallos/por_fallos;
		media_exitos=media_exitos/por_exitos;
		por_fallos=(por_fallos/(double)num_experimentos)*100.0;
		por_exitos=(por_exitos/(double)num_experimentos)*100.0;
		
		System.out.println("Search Outcome=" + search.getOutcome());
		System.out.println("Final State=\n" + search.getLastSearchState());
		
		System.out.printf("Fallos: %.2f%%\n", por_fallos);
		System.out.printf("Coste medio fallos: %.2f\n", media_fallos);
		System.out.printf("Éxitos: %.2f%%\n", por_exitos);
		System.out.printf("Coste medio éxitos: %.2f\n", media_exitos);
	}
	

	private static void nQueensRandomRestartHillClimbing() {
		System.out.println("\nNQueensDemo HillClimbing, reinicio estado inicial hasta éxito-->");

		Boolean continuar=true;
		NQueensBoard tablero = new NQueensBoard(_boardSize);
		SearchAgent agent;
		HillClimbingSearch search;
		Problem problem;
		double por_fallos=0.0;
		double media_fallos=0.0;
		double por_exitos=0.0;
		double media_exitos=0.0;
		int num_intentos=0;

		while(continuar) {
			tablero=generateRandomNqueensBoard(_boardSize);
			try {
				problem = new Problem(tablero, NQueensFunctionFactory.getCActionsFunction(), NQueensFunctionFactory.getResultFunction(), new NQueensGoalTest());
				search = new HillClimbingSearch(new AttackingPairsHeuristic());
				agent = new SearchAgent(problem, search);
				
				if(search.getOutcome().toString().contentEquals("SOLUTION_FOUND")) {
					continuar=false;
					por_exitos++;
					num_intentos++;
					media_exitos += agent.getActions().size();
					
					System.out.println("Search Outcome=" + search.getOutcome());

					System.out.println("Final State=\n" + search.getLastSearchState());
					printInstrumentation(agent.getInstrumentation());
					
					
					media_fallos=media_fallos/por_fallos;
					media_exitos=media_exitos/por_exitos;
					por_fallos=(por_fallos/(double)num_intentos)*100.0;
					por_exitos=(por_exitos/(double)num_intentos)*100.0;

					System.out.println("Número de reintentos: " + num_intentos);
					System.out.printf("Fallos: %.2f%%\n", por_fallos);
					System.out.printf("Coste medio fallos: %.2f\n", media_fallos);
					System.out.printf("Éxitos: %.2f%%\n", por_exitos);
					System.out.printf("Coste medio éxitos: %.2f\n", media_exitos);	
				}
				else {
					num_intentos++;
					por_fallos++;
					media_fallos += agent.getActions().size();					
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
						
		}
	}

	public static void nQueensGeneticAlgorithmSearch() {
		System.out.println("\nNQueensDemo GeneticAlgorithm  -->");
		try {
			NQueensFitnessFunction fitnessFunction = new NQueensFitnessFunction();
			// Generate an initial population
			Set<Individual<Integer>> population = new HashSet<Individual<Integer>>();
			for (int i = 0; i < 50; i++) {
				population.add(fitnessFunction.generateRandomIndividual(_boardSize));
			}

			GeneticAlgorithm<Integer> ga = new GeneticAlgorithm<Integer>(_boardSize,fitnessFunction.getFiniteAlphabetForBoardOfSize(_boardSize),0.15);

			// Run for a set amount of time
			Individual<Integer> bestIndividual = ga.geneticAlgorithm(population, fitnessFunction, fitnessFunction, 1000L);

			// Run till goal is achieved
			bestIndividual = ga.geneticAlgorithm(population, fitnessFunction,fitnessFunction, 0L);

			System.out.println("Parámetros iniciales:");
			System.out.println("   Población = " + ga.getPopulationSize());
			System.out.println("   Probabilidad mutación: 0'15");
			System.out.println("   Mejor individuo =\n" + fitnessFunction.getBoardForIndividual(bestIndividual));
			System.out.println("   Tamaño tablero = " + _boardSize);
			System.out.println("   Fitness = " + fitnessFunction.getValue(bestIndividual));
			System.out.println("   Es objetivo = " + fitnessFunction.isGoalState(bestIndividual));
			System.out.println("   Tamaño de población = " + ga.getPopulationSize());
			System.out.println("   Iteraciones = " + ga.getIterations());
			System.out.println("   Tiempo = " + ga.getTimeInMilliseconds() + "ms.");
					
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static void printInstrumentation(Properties properties) {
		Iterator<Object> keys = properties.keySet().iterator();
		while (keys.hasNext()) {
			String key = (String) keys.next();
			String property = properties.getProperty(key);
			System.out.println(key + " : " + property);
		}

	}

	private static void printActions(List<Action> actions) {
		for (int i = 0; i < actions.size(); i++) {
			String action = actions.get(i).toString();
			System.out.println(action);
		}
	}

}
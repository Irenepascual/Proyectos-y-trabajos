package aima.gui.demo.search;

import java.util.Iterator;
import java.util.List;
import java.util.Properties;

import aima.core.agent.Action;
import aima.core.environment.eightpuzzlePrac2.EightPuzzleFunctionFactoryPrac2;
import aima.core.environment.eightpuzzlePrac2.EightPuzzleGoalTestPrac2;
import aima.core.environment.eightpuzzlePrac2.EightPuzzleBoardPrac2;
import aima.core.environment.eightpuzzlePrac2.ManhattanHeuristicFunctionPrac2;
import aima.core.environment.eightpuzzlePrac2.MisplacedTilleHeuristicFunctionPrac2;
import aima.core.search.framework.GraphSearch;
import aima.core.search.framework.Problem;
import aima.core.search.framework.QueueSearch;
import aima.core.search.framework.Search;
import aima.core.search.framework.SearchAgent;
import aima.core.search.framework.TreeSearch;
import aima.core.search.informed.AStarSearch;
import aima.core.search.uninformed.BreadthFirstSearch;
import aima.core.search.uninformed.IterativeDeepeningSearch;
import aima.core.util.math.BiseccionPrac2;



/**
 * @author Ravi Mohan
 * 
 */

public class EightPuzzleDemoPrac2 {

    static EightPuzzleBoardPrac2 inicial;
    static EightPuzzleBoardPrac2 fin;

    public static void main(String[] args) {

    	int num_exp=100;
    	
    	int total_BFS=0;
    	int total_IDS=0;
    	int total_MAN=0;
    	int total_DIS=0;
    	
    	double BFS_EST=0.0;
    	double IDS_EST=0.0;
    	double MAN_EST=0.0;
    	double DIS_EST=0.0;
    	
    	int pasos;
    	
        System.out.printf("------------------------------------------------------------------------------------------------------------\n");
        System.out.printf("||                   NODOS GENERADOS                    ||                       b*                       ||\n");
        System.out.printf("------------------------------------------------------------------------------------------------------------\n");
        System.out.printf("||   d ||        BFS|        IDS|     A*h(1)|     A*h(2)||        BFS|        IDS|     A*h(1)|      A*h(2)||\n");
        System.out.printf("------------------------------------------------------------------------------------------------------------\n");
        
        for (pasos = 2; pasos < 25; pasos++) {
        	total_BFS=0;
        	total_IDS=0;
        	total_MAN=0;
        	total_DIS=0;
        	
        	BFS_EST=0.0;
        	IDS_EST=0.0;
        	MAN_EST=0.0;
        	DIS_EST=0.0;
        	
        	
        	for (int i=0; i<num_exp ; i++) {
	        	Problem problem=null;

	        	inicial = GenerateInitialEightPuzzleBoardPrac2.randomIni();

    	        	boolean correcto = false;
    	        	int h;
                	while(!correcto) {
                        fin = GenerateInitialEightPuzzleBoardPrac2.random(pasos, inicial);
                        new EightPuzzleGoalTestPrac2(fin);
                        
        	        	problem = new Problem(inicial, EightPuzzleFunctionFactoryPrac2.getActionsFunction(),
                                EightPuzzleFunctionFactoryPrac2.getResultFunction(), new EightPuzzleGoalTestPrac2(fin));
        	        	
        	            try {
        	            	SearchAgent agent = new SearchAgent(problem, new AStarSearch(new GraphSearch(),new ManhattanHeuristicFunctionPrac2(fin)));	            	
            	            h = (int)Float.parseFloat(agent.getInstrumentation().getProperty("pathCost"));
                            // Si la distancia de Manhattan es igual a los pasos que quieres, termina el bucle
                            if (h == pasos) {
                                correcto = true;
                            }

        	            }catch(Exception e) {
        	            	e.printStackTrace();
        	            }	      
                    
                	}
                
	        	int nodosGenerados;
	        	
	        	problem = new Problem(inicial, EightPuzzleFunctionFactoryPrac2.getActionsFunction(),
                        EightPuzzleFunctionFactoryPrac2.getResultFunction(), new EightPuzzleGoalTestPrac2(fin));


	            try {
		            SearchAgent agent = new SearchAgent(problem, new BreadthFirstSearch(new GraphSearch()));
		        	
		            nodosGenerados= ((int)Float.parseFloat(agent.getInstrumentation().getProperty("nodesGenerated")));
		            total_BFS=total_BFS + nodosGenerados;
	            }catch(Exception e) {
	            	e.printStackTrace();
	            }

	            
	            try {
	            	if(pasos<12) {
		            	SearchAgent agent = new SearchAgent(problem, new IterativeDeepeningSearch());
		            	
			            nodosGenerados= ((int)Float.parseFloat(agent.getInstrumentation().getProperty("nodesGenerated")));
			            total_IDS=total_IDS + nodosGenerados;
	            	}
	            }catch(Exception e) {
	            	e.printStackTrace();
	            }
	                   
	            
	            try {
	            	SearchAgent agent = new SearchAgent(problem, new AStarSearch(new GraphSearch(),new ManhattanHeuristicFunctionPrac2(fin)));	            	
		            nodosGenerados= ((int)Float.parseFloat(agent.getInstrumentation().getProperty("nodesGenerated")));
		            total_MAN=total_MAN + nodosGenerados;

	            }catch(Exception e) {
	            	e.printStackTrace();
	            }	      
	            
	        	
	            try {
	            	SearchAgent agent = new SearchAgent(problem, new AStarSearch(new GraphSearch(),new MisplacedTilleHeuristicFunctionPrac2(fin)));
		            nodosGenerados= ((int)Float.parseFloat(agent.getInstrumentation().getProperty("nodesGenerated")));
		            total_DIS=total_DIS + nodosGenerados;
	            }catch(Exception e) {
	            	e.printStackTrace();
	            }	  
        	}
	            
	        total_BFS=total_BFS/num_exp;
	        total_IDS=total_IDS/num_exp;
	        total_MAN=total_MAN/num_exp;
	        total_DIS=total_DIS/num_exp;
	        
	        BFS_EST = BiseccionPrac2.calcularFactorRamificacionEfectivo(total_BFS, pasos, 1.1, 10.0);
	        IDS_EST = BiseccionPrac2.calcularFactorRamificacionEfectivo(total_IDS, pasos, 1.1, 10.0);
	        MAN_EST = BiseccionPrac2.calcularFactorRamificacionEfectivo(total_MAN, pasos, 1.1, 10.0);
	        DIS_EST = BiseccionPrac2.calcularFactorRamificacionEfectivo(total_DIS, pasos, 1.1, 10.0);

	        
	        
	        if(pasos<12) {
	        	System.out.printf("|||%4d||%10d |%10d |%10d |%10d ||%10.2f |%10.2f |%10.2f |%10.2f |||\n",pasos,total_BFS,total_IDS,total_DIS,total_MAN,BFS_EST,IDS_EST,DIS_EST,MAN_EST);
	        }
	        else {
	        	System.out.printf("|||%4d||%10d |%10s |%10d |%10d ||%10.2f |%10s |%10.2f |%10.2f |||\n",pasos,total_BFS,"---",total_DIS,total_MAN,BFS_EST,"---",DIS_EST,MAN_EST);
	        }
        }
        System.out.printf("------------------------------------------------------------------------------------------------------------\n");
    }
}

   


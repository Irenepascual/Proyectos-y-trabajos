# factorOperations.py
# -------------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).

from typing import List, ValuesView
from bayesNet import Factor
import functools
from util import raiseNotDefined

def joinFactorsByVariableWithCallTracking(callTrackingList=None):


    def joinFactorsByVariable(factors: List[Factor], joinVariable: str):
        """
        Input factors is a list of factors.
        Input joinVariable is the variable to join on.

        This function performs a check that the variable that is being joined on 
        appears as an unconditioned variable in only one of the input factors.

        Then, it calls your joinFactors on all of the factors in factors that 
        contain that variable.

        Returns a tuple of 
        (factors not joined, resulting factor from joinFactors)
        """

        if not (callTrackingList is None):
            callTrackingList.append(('join', joinVariable))

        currentFactorsToJoin =    [factor for factor in factors if joinVariable in factor.variablesSet()]
        currentFactorsNotToJoin = [factor for factor in factors if joinVariable not in factor.variablesSet()]

        # typecheck portion
        numVariableOnLeft = len([factor for factor in currentFactorsToJoin if joinVariable in factor.unconditionedVariables()])
        if numVariableOnLeft > 1:
            print("Factors failed joinFactorsByVariable typecheck: ", factors)
            raise ValueError("The joinBy variable can only appear in one factor as an \nunconditioned variable. \n" +  
                               "joinVariable: " + str(joinVariable) + "\n" +
                               ", ".join(map(str, [factor.unconditionedVariables() for factor in currentFactorsToJoin])))
        
        joinedFactor = joinFactors(currentFactorsToJoin)
        return currentFactorsNotToJoin, joinedFactor

    return joinFactorsByVariable

joinFactorsByVariable = joinFactorsByVariableWithCallTracking()

########### ########### ###########
########### QUESTION 2  ###########
########### ########### ###########

def joinFactors(factors: ValuesView[Factor]):
    """
    factors: can iterate over it as if it was a list, and convert to a list.
    
    You should calculate the set of unconditioned variables and conditioned 
    variables for the join of those factors.

    Return a new factor that has those variables and whose probability entries 
    are product of the corresponding rows of the input factors.

    You may assume that the variableDomainsDict for all the input 
    factors are the same, since they come from the same BayesNet.

    joinFactors will only allow unconditionedVariables to appear in 
    one input factor (so their join is well defined).

    Hint: Factor methods that take an assignmentDict as input 
    (such as getProbability and setProbability) can handle 
    assignmentDicts that assign more variables than are in that factor.

    Useful functions:
    Factor.getAllPossibleAssignmentDicts
    Factor.getProbability
    Factor.setProbability
    Factor.unconditionedVariables
    Factor.conditionedVariables
    Factor.variableDomainsDict
    """

    # typecheck portion
    setsOfUnconditioned = [set(factor.unconditionedVariables()) for factor in factors]
    if len(factors) > 1:
        intersect = functools.reduce(lambda x, y: x & y, setsOfUnconditioned)
        if len(intersect) > 0:
            print("Factors failed joinFactors typecheck: ", factors)
            raise ValueError("unconditionedVariables can only appear in one factor. \n"
                    + "unconditionedVariables: " + str(intersect) + 
                    "\nappear in more than one input factor.\n" + 
                    "Input factors: \n" +
                    "\n".join(map(str, factors)))


    setsOfConditioned = [set(factor.conditionedVariables()) for factor in factors]
    if len(factors) > 1:
        intersect2 = functools.reduce(lambda x, y: x & y, setsOfConditioned)

    factors_list = [factor for factor in factors]
    variableDomainsDict = factors_list[0].variableDomainsDict()

    "*** YOUR CODE HERE ***"

    ### UNIÓN DE VARIOS FACTORES PROBABILÍSTICOS EN UNA RED BAYESIANA ###

    #Calculamos las variables incondicionadas y condicionadas
    todasLasVariablesIncondicionadas = set()
    todasLasVariablesCondicionadas = set()

    for factor in factors_list:
        #devuelve las variables no condicionadas de un factor en concreto
        todasLasVariablesIncondicionadas.update(factor.unconditionedVariables())

        #devuelve las variables condicionadas de un factor en concreto
        todasLasVariablesCondicionadas.update(factor.conditionedVariables())


    #Eliminar del conjunto de variables condicionadas aquellas que estén en incondicionadas
    todasLasVariablesCondicionadas -= todasLasVariablesIncondicionadas

    #Crear el nuevo factor con las variables unidas (tanto condicionadas como no condicionadas)
    nuevoFactor = Factor(todasLasVariablesIncondicionadas, todasLasVariablesCondicionadas, variableDomainsDict)

    #Calculamos la probabilidad combinada, gracias a iterar sobre todas las posibles combinaciones
    #de asignaciones para el nuevo factor. Con cada combinación se calcula la probabilidad total
    #multiplicando las probabiliades correspondientes de cada uno de los factores originales.
    for diccionarioAsignacion in nuevoFactor.getAllPossibleAssignmentDicts():
        probabilidad = 1.0
        for factor in factors_list:
            #Cada factor calcula su probabilidad para la asignación dada
            probabilidad = probabilidad * factor.getProbability(diccionarioAsignacion)
        
        #Establecer la probabilidad en el nuevo factor
        nuevoFactor.setProbability(diccionarioAsignacion, probabilidad)

    return nuevoFactor

    "*** END YOUR CODE HERE ***"

########### ########### ###########
########### QUESTION 3  ###########
########### ########### ###########

def eliminateWithCallTracking(callTrackingList=None):

    def eliminate(factor: Factor, eliminationVariable: str):
        """
        Input factor is a single factor.
        Input eliminationVariable is the variable to eliminate from factor.
        eliminationVariable must be an unconditioned variable in factor.
        
        You should calculate the set of unconditioned variables and conditioned 
        variables for the factor obtained by eliminating the variable
        eliminationVariable.

        Return a new factor where all of the rows mentioning
        eliminationVariable are summed with rows that match
        assignments on the other variables.

        Useful functions:
        Factor.getAllPossibleAssignmentDicts
        Factor.getProbability
        Factor.setProbability
        Factor.unconditionedVariables
        Factor.conditionedVariables
        Factor.variableDomainsDict
        """
        # autograder tracking -- don't remove
        if not (callTrackingList is None):
            callTrackingList.append(('eliminate', eliminationVariable))

        # typecheck portion
        if eliminationVariable not in factor.unconditionedVariables():
            print("Factor failed eliminate typecheck: ", factor)
            raise ValueError("Elimination variable is not an unconditioned variable " \
                            + "in this factor\n" + 
                            "eliminationVariable: " + str(eliminationVariable) + \
                            "\nunconditionedVariables:" + str(factor.unconditionedVariables()))
        
        if len(factor.unconditionedVariables()) == 1:
            print("Factor failed eliminate typecheck: ", factor)
            raise ValueError("Factor has only one unconditioned variable, so you " \
                    + "can't eliminate \nthat variable.\n" + \
                    "eliminationVariable:" + str(eliminationVariable) + "\n" +\
                    "unconditionedVariables: " + str(factor.unconditionedVariables()))

        "*** YOUR CODE HERE ***"

        #Determinar los conjuntos de variables no condicionadas y condicionadas para el nuevo factor,
        #excluyendo la variable que se va a eliminar
        variables_no_condicionadas = factor.unconditionedVariables() - {eliminationVariable}
        variables_condicionadas = factor.conditionedVariables()
        dominios_variables = factor.variableDomainsDict()

        #Crear un nuevo factor con las variables no condicionadas y condicionadas actualizadas
        nuevo_factor = Factor(variables_no_condicionadas, variables_condicionadas, dominios_variables)

        #Inicializar una lista para almacenar las asignaciones reducidas y sus probabilidades
        mapa_probabilidades = []  # Una lista para almacenar las asignaciones reducidas y sus probabilidades
        
        #Iterar sobre todas las posibles asignaciones de valores para el factor original
        for asignacion in factor.getAllPossibleAssignmentDicts():
            #Eliminar la variable eliminada de la asignación
            asignacion_reducida = [(k, asignacion[k]) for k in asignacion if k != eliminationVariable]

            #Comprobar si la asignación reducida ya está en el mapa de probabilidades
            encontrada = False
            for i in range(len(mapa_probabilidades)):
                if mapa_probabilidades[i][0] == asignacion_reducida:
                    #Si la asignación ya existe, sumar la probabilidad correspondiente
                    mapa_probabilidades[i][1] += factor.getProbability(asignacion)
                    encontrada = True
                    break

             #Si la asignación no se encuentra, agregar una nueva entrada con su probabilidad
            if not encontrada:
                mapa_probabilidades.append([asignacion_reducida, factor.getProbability(asignacion)])

        #Iterar sobre cada par de asignación reducida y su probabilidad en el mapa de probabilidades
        for asignacion_reducida, probabilidad in mapa_probabilidades:
            #Crear un diccionario vacío para almacenar la asignación convertida
            diccionario_asignacion = {}
            #Agregar las claves y valores de la asignación reducida al diccionario
            for clave, valor in asignacion_reducida:
                diccionario_asignacion[clave] = valor
            
            #Establecer la probabilidad de la asignación en el nuevo factor
            nuevo_factor.setProbability(diccionario_asignacion, probabilidad)
        
        #Retornar el nuevo factor con las probabilidades actualizadas
        return nuevo_factor

        "*** END YOUR CODE HERE ***"

    return eliminate

eliminate = eliminateWithCallTracking()


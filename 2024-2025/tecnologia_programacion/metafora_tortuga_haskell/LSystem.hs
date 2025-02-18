module LSystem where

import SVG
import Turtle
import Reglas

-- Genera, a partir del estado inicial de la tortuga, una lista de puntos que corresponde
-- al gráfico generado por una secuencia de comandos almacenados en una cadena
tplot :: Turtle -> String -> [Position]

-- Caso trivial:
tplot (paso,giro,pos,orn) [] = [pos]
-- Caso general:
tplot (paso,giro,pos,orn) (c:cs)
 | c == '>' = pos : tplot (moveTurtle (paso,giro,pos,orn) Forward) cs
 | c == '+' = pos : tplot (moveTurtle (paso,giro,pos,orn) TurnRight) cs
 | c == '-' = pos : tplot (moveTurtle (paso,giro,pos,orn) TurnLeft) cs
 | c >= 'A' && c <= 'Z' = pos : tplot (moveTurtle (paso,giro,pos,orn) Forward) cs
 | otherwise = pos : tplot (paso,giro,pos,orn) cs



-- Función auxiliar para ir decodificando cada letra por la regla que le corresponde
sustituir :: (Char -> String) -> String -> String

-- Caso trivial:
sustituir _ [] = []
-- Caso general:
sustituir r (s:ss) = r s ++ sustituir r ss


-- Devuelve la cadena resultante de aplicar al axioma inicial la función con el interfaz
-- de rules correspondiente, el número de veces indicado
lsystem :: (Char -> String) -> String -> Int -> String

-- Caso trivial:
lsystem _ s 0 = s
-- Caso general:
lsystem r s n = lsystem r (sustituir r s) (n-1)

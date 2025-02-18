module Reglas where

--reglas para la Curva de Koch
rulesKoch :: Char -> String
rulesKoch c
 | c == 'F' = "F+F--F+F"
 | c == '+' = "+"
 | c == '-' = "-"
 | c == '>' = ">"
 | otherwise = []

--reglas para la Curva de Koch cuadrada
rulesKochCuadrada :: Char -> String
rulesKochCuadrada c
 | c == 'F' = "F+F-F-F+F"
 | c == '+' = "+"
 | c == '-' = "-"
 | c == '>' = ">"
 | otherwise = []

--reglas para Koch Snowflake
rulesSnowflake :: Char -> String
rulesSnowflake c
 | c == 'F' = "F-F++F-F"
 | c == '+' = "+"
 | c == '-' = "-"
 | c == '>' = ">"
 | otherwise = []

--reglas para la Isla de Minkowski 
rulesMinkowski :: Char -> String
rulesMinkowski c
 | c == 'F' = "F+F-F-FF+F+F-F"
 | c == '+' = "+"
 | c == '-' = "-"
 | c == '>' = ">"
 | otherwise = []

--reglas para el triángulo de Sierpinsky
rulesSierpinsky :: Char -> String
rulesSierpinsky c
 | c == 'F' = "F-G+F+G-F"
 | c == 'G' = "GG"
 | c == '+' = "+"
 | c == '-' = "-"
 | c == '>' = ">"
 | otherwise = []

--reglas para Sierpinsky Arrowhead
rulesArrowhead :: Char -> String
rulesArrowhead c
 | c == 'F' = "G-F-G"
 | c == 'G' = "F+G+F"
 | c == '+' = "+"
 | c == '-' = "-"
 | c == '>' = ">"
 | otherwise = []

--reglas para la Curva de Hilbert
rulesHilbert :: Char -> String
rulesHilbert c
 | c == 'f' = "-g>+f>f+>g-"
 | c == 'g' = "+f>-g>g->f+"
 | c == '+' = "+"
 | c == '-' = "-"
 | c == '>' = ">"
 | otherwise = []
 

--reglas para la Curva de Gosper
rulesGosper :: Char -> String
rulesGosper c
 | c == 'F' = "F-G--G+F++FF+G-"
 | c == 'G' = "+F-GG--G-F++F+G"
 | c == '+' = "+"
 | c == '-' = "-"
 | c == '>' = ">"
 | otherwise = []
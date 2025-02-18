import LSystem
import SVG
import Turtle
import Reglas

main = do
 let
  triangulo = tplot (1,60,(0,0),0) ">++>++>"
  cuadrado = tplot (1,90,(0,0),90) ">+>+>+>+"
  circulo = tplot (1,1,(0.5,0),0) ">+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>"
  tecprog  = tplot (1,-90,(0,0),270) "->+>>>>->+>+>>>+>+>->>>>->>->>>>>+>>>+>+>>->->+>+>->->>+>+>>>++>>>>->>>>>+>>>+>+>>->>>->>+>+>>>++>>>>>>>->>+>>->>>->>>->->>+>+>+>->->>>>->>>>>->+>>+>+>+>>+>->>>->>>->->>+>+>+>->->>>>->->>+>+>>->>->>>+>>+>+>+>->+>>+>>>+>>>>>+>>>++>>>>->>>>>+>>>+>+>>->>>->->->+>+>>+>>>+>>>"

 savesvg "triangulo" triangulo
 savesvg "cuadrado" cuadrado
 savesvg "circulo" circulo
 savesvg "tecprog" tecprog

 print ("Creados los archivos: triangulo, cuadrado, circulo y tecprog")

 print ("Prueba de: lsystem rulesArrowhead F 0")
 print (lsystem rulesArrowhead "F" 0)
 print ("Prueba de: lsystem rulesArrowhead F 2")
 print (lsystem rulesArrowhead "F" 2)
 
 let 
  figuraKoch = tplot (1,60,(0,0),0) (lsystem rulesKoch "F" 3)
  figuraKochCuadrada = tplot (1,90,(0,0),0) (lsystem rulesKochCuadrada "F" 3)
  figuraSnowflake = tplot (1,60,(0,0),0) (lsystem rulesSnowflake "F++F++F" 3)
  figuraMinkowski = tplot (1,90,(0,0),0) (lsystem rulesMinkowski "F+F+F+F" 2)
  figuraSierpinsky = tplot (1,120,(0,0),180) (lsystem rulesSierpinsky "F-G-G" 4)
  figuraArrowhead = tplot (1,60,(0,0),0) (lsystem rulesArrowhead "F" 6)
  figuraHilbert = tplot (1,90,(0,0),180) (lsystem rulesHilbert "f" 4)
  figuraGosper = tplot (1,-60,(0,0),50) (lsystem rulesGosper "F" 3)

 savesvg "figuraKoch" figuraKoch
 savesvg "figuraKochCuadrada" figuraKochCuadrada
 savesvg "figuraSnowflake" figuraSnowflake
 savesvg "figuraMinkowski" figuraMinkowski
 savesvg "figuraSierpinsky" figuraSierpinsky
 savesvg "figuraArrowhead" figuraArrowhead
 savesvg "figuraHilbert" figuraHilbert
 savesvg "figuraGosper" figuraGosper
 
 print ("Creados los archivos: figuraKoch, figuraKochCuadrada, figuraSnowflake, figuraMinkowski, figuraSierpinsky, figuraArrowhead, figuraHilbert y figuraGosper")
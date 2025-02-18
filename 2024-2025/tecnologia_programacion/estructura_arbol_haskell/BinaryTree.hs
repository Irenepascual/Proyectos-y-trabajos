module BinaryTree where

import Data.List

-- TIPO DE DATOS BinaryTree
data BinaryTree t = Vacio | Leaf t | Branch t (BinaryTree t) (BinaryTree t)

-- ----------------------------------------------------------------------------

-- FUNCIONES: empty, leaf, tree, size
empty :: BinaryTree t
empty = Vacio

leaf :: t -> BinaryTree t
leaf t = Leaf t

tree :: t -> BinaryTree t -> BinaryTree t -> BinaryTree t
tree x lc rc = Branch x lc rc

size :: BinaryTree t -> Int
size (Vacio) = 0
size (Leaf _) = 1
size (Branch _ lc rc) = 1 + (size lc) + (size rc)

-- INSTANCIAS DE LAS CLASES: Show, Eq
instance (Show t) => Show (BinaryTree t) where
 show (Vacio) = "()"
 show (Leaf t) = show t
 show (Branch t lc rc) = show t ++ "\n" ++ showAux "   " lc ++ "\n" ++ showAux "   " rc

showAux :: (Show t) => String -> BinaryTree t -> String
showAux sep (Vacio) = sep ++ "()"
showAux sep (Leaf t) = sep ++ show t
showAux sep (Branch t lc rc) = sep ++ show t ++ "\n" ++ showAux (sep++"   ") lc ++ "\n" ++ showAux (sep++"   ") rc

instance (Eq t) => Eq (BinaryTree t) where
 Vacio == Vacio = True
 Leaf x == Leaf y = x == y
 Branch t t1 t2 == Branch l l1 l2 = (t == l) && (t1 == l1) && (t2 == l2)
 _ == _ = False

-- ----------------------------------------------------------------------------

-- ÁRBOLES BINARIOS DE BÚSQUEDA

add :: (Ord t) => BinaryTree t -> t -> BinaryTree t
add (Vacio) x = leaf x
add (Leaf t) x
 | x < t = tree t (leaf x) empty
 | otherwise = tree t empty (leaf x)
add (Branch t t1 t2) x
 | x < t = tree t (add t1 x) t2
 | otherwise = tree t t1 (add t2 x)

build :: (Ord t) => [t] -> BinaryTree t
build [] = empty
build (x:[]) = leaf x
build (x:xs) = tree x (build (filter (< x) xs)) (build (filter (>= x) xs))

-- ----------------------------------------------------------------------------

-- ÁRBOLES BINARIOS DE BÚSQUEDA EQUILIBRADOS

buildBalanced :: (Ord t) => [t] -> BinaryTree t
buildBalanced [] = empty
buildBalanced [h] = leaf h
buildBalanced x = tree (med) (buildBalanced x1) (buildBalanced x2)
 where
  tam = length x
  med = snd (splitAt (tam `div` 2) (sort x)) !! 0
  x1 = fst (splitAt (tam `div` 2) (sort x))
  x2Aux = snd (splitAt (tam `div` 2) (sort x))
  x2 = delete med x2Aux


balance :: (Ord t) => BinaryTree t -> BinaryTree t
balance t = buildBalanced (inorder t)

-- ----------------------------------------------------------------------------

-- RECORRIDOS

preorder :: BinaryTree t -> [t]
preorder (Vacio) = []
preorder (Leaf t) = [t]
preorder (Branch t t1 t2) = [t] ++ (preorder t1) ++ (preorder t2)

postorder :: BinaryTree t -> [t]
postorder (Vacio) = []
postorder (Leaf t) = [t]
postorder (Branch t t1 t2) = (postorder t1) ++ (postorder t2) ++ [t]

inorder :: BinaryTree t -> [t]
inorder (Vacio) = []
inorder (Leaf t) = [t]
inorder (Branch t t1 t2) = (inorder t1) ++ [t] ++ (inorder t2)

-- ----------------------------------------------------------------------------

-- BÚSQUEDAS
between :: (Ord t) => BinaryTree t -> t -> t -> [t]
between (Vacio) _ _= []
between (Leaf t) xmin xmax
 | xmin > xmax = undefined
 | xmin <= t && t <= xmax = [t]
 | otherwise = []
between (Branch t t1 t2) xmin xmax
 | xmin > xmax = undefined
 | t < xmin = between t2 xmin xmax
 | t == xmin = [t] ++ (between t2 xmin xmax)
 | t > xmax = between t1 xmin xmax
 | t >= xmax = (between t1 xmin xmax) ++ [t]
 | otherwise = (between t1 xmin xmax) ++ [t] ++ (between t2 xmin xmax)

-- ----------------------------------------------------------------------------
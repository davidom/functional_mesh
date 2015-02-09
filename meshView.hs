import Data.List

view :: [Integer] -> [[Integer]] -> [[Integer]]
view delta v = [delta] ++ v

has :: Integer -> [[Integer]] -> Bool
has thing lists = or $ map (thing `elem`) lists

get :: Integer -> [[Integer]] -> [[Integer]]
get thing lists = dropWhile (notElem thing) lists

printIt :: [[Integer]] -> [Integer]
printIt lists = foldl uniqueJoin [] lists

uniqueJoin :: [Integer] -> [Integer] -> [Integer]
uniqueJoin list0 list1 = list0 ++ takeWhile (\x -> notElem x list0) list1

lv :: [[Integer]]
lv = view [0,1] $ view [4,5,6] $ view [2,5] [[]]

module StringSet = Set.Make(String)

type expr_parse_result = bool * StringSet.t (* Ok/Nok, read identifiers *)
type action_parse_result = bool * StringSet.t * StringSet.t (* Ok/Nok, LHS identifiers, RHS identifiers *)
type state_valuation_parse_result = bool * string (* Ok/Nok, LHS identifier *)
    
let int_of_bool = function true -> 1 | false -> 0
  
let string_of_list ~f ~sep l =
  let rec h = function
      [] -> ""
    | [x] -> f x
    | x::xs -> f x ^ sep ^ h xs in
  h l

let is_uppercase c = Char.uppercase_ascii c = c 

let is_lowercase c = Char.lowercase_ascii c = c 

let starts_uppercase s = 
  if String.length s > 0 then is_uppercase (s.[0])
  else false

let starts_lowercase s = 
  if String.length s > 0 then is_lowercase (s.[0])
  else false

let string_of_list ~f ~sep l =
  let rec h = function
      [] -> ""
    | [x] -> f x
    | x::xs -> f x ^ sep ^ h xs in
  h l

let is_ordered (l:int list) = List.sort compare l = l

type type_expr =
| TyEvent
| TyInt of ty_int_annot
| TyBool
  [@@deriving show { with_path=false }]

and ty_int_annot = 
  TA_none
| TA_size of int
| TA_range of int * int (* min, max *)
  [@@deriving show { with_path=false }]

type expr = 
  EInt of int
| EBool of bool
| EVar of string             
| EBinop of string * expr * expr
| ECast of expr * type_expr
| EArr of string * expr
  [@@deriving show { with_path=false }]

type guard = expr list
  [@@deriving show { with_path=false }]

type action =
  | Emit of string
  | Assign of string * expr
  [@@deriving show { with_path=false }]

type actions = action list
  [@@deriving show { with_path=false }]

type stimuli = 
  Periodic of int * int * int
| Sporadic of int list
| ValueChanges of value_change list
  [@@deriving show { with_path=false }]

and value_change = int * value
  [@@deriving show { with_path=false }]

and value = 
| Int of int
| Bool of bool
  [@@deriving show { with_path=false }]

let vars_of_expr e = 
  let module StringSet = Set.Make(String) in
  let rec vars_of e = match e with
    | EVar v -> StringSet.singleton v 
    | EBinop (_,e1,e2) -> StringSet.union (vars_of e1) (vars_of e2)
    | _ -> StringSet.empty in
  e |> vars_of |> StringSet.elements
       

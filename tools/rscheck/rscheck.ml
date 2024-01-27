let usage = "usage: rfsmlint [options] text"

type target = Nothing | CheckExpr | CheckAction
let target = ref Nothing
              
let source_text = ref ""

let anonymous text = source_text := text

let options = [
 "-guard", Arg.Unit (fun () -> target := CheckExpr), "check guard expression";
 "-action", Arg.Unit (fun () -> target := CheckAction), "check action";
]

let parse p s = 
  p Lexer.main (Lexing.from_string s)

let parse_guard s = parse Parsers.expr1 s 
let parse_action s = parse Parsers.action1 s 

let print_result ok lhs_vars rhs_vars =
  let open Misc in
  Printf.fprintf stdout "%d;%s;%s\n" 
    (int_of_bool ok)
    (string_of_list ~f:Fun.id ~sep:"," (StringSet.elements lhs_vars))
    (string_of_list ~f:Fun.id ~sep:"," (StringSet.elements rhs_vars))

let main () =
  let open Misc in
  try
    Sys.catch_break true;
    Arg.parse options anonymous usage;
    match !target with
    | Nothing -> print_result true StringSet.empty StringSet.empty
    | CheckExpr ->
      let r, rhs_vars = parse_guard !source_text in
      print_result r StringSet.empty rhs_vars
    | CheckAction ->
      let r, lhs_vars, rhs_vars = parse_action !source_text in
      print_result r lhs_vars rhs_vars;
    exit 0
  with
  | _ ->
      print_result false StringSet.empty StringSet.empty

let _ = Printexc.print main ()

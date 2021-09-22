let parse f s = 
  let lexbuf =  Lexing.from_string s in
  lexbuf |> f Lexer.main 

let parse_and_print p p' s = 
  let r = parse p s in
  Printf.printf "\"%s\" => %s\n" s (p' r)

let p_guard = parse_and_print Parsers.guard Syntax.show_guard 
let p_actions = parse_and_print Parsers.actions Syntax.show_actions
let p_stimuli = parse_and_print Parsers.stimuli Syntax.show_stimuli

let main () =
  try
  p_guard "x=1";
  p_guard "h.(x=1)";
  p_actions "x:=1+u";
  p_stimuli "Periodic 10 100 200";
  p_stimuli "Sporadic 10 100 200";
  p_stimuli "ValueChanges 0:1 10:2 30:3"
  with 
  | e ->
     Printf.eprintf "%s\n" (Printexc.to_string e);
     flush stderr; exit 1

let _ = Printexc.print main ()

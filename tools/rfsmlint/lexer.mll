{
open Parsers

exception Illegal_character of int * string

(* The table of keywords *)

let keyword_table = [
  "Periodic", PERIODIC;
  "Sporadic", SPORADIC;
  "ValueChanges", VALUECHANGES;
  "int", TYINT;
  "bool", TYBOOL;
  "event", TYEVENT;
]
}

rule main = parse
  | [' ' '\t'] +
      { main lexbuf }
  | ['\010' '\013' ]
      { Lexing.new_line lexbuf; main lexbuf }
  | ['a'-'z' ] ( ['A'-'Z' 'a'-'z' '0'-'9' '_' ] ) *
      { let s = Lexing.lexeme lexbuf  in
        try List.assoc s keyword_table
        with Not_found -> LID s }
  | ['A'-'Z' 'a'-'z' ] ( ['A'-'Z' 'a'-'z' '0'-'9' '_' ] ) *
      { let s = Lexing.lexeme lexbuf  in
        try List.assoc s keyword_table
        with Not_found -> UID s }
  | ['0'-'9']+
      { INT (int_of_string(Lexing.lexeme lexbuf)) }
  | "'0'" { BOOL false }
  | "'1'" { BOOL true }
  (* | ";" { SEMICOLON } *)
  | "." { DOT }
  | "(" { LPAREN }
  | ")" { RPAREN }
  | "[" { LBRACKET }
  | "]" { RBRACKET }
  (* | "," { COMMA } *)
  | ":" { COLON }
  | "::" { COLONCOLON }
  | "=" { EQUAL }
  | ":=" { COLEQ }
  | "!="    { NOTEQUAL }
  | '>'    { GT }
  | '<'    { LT }
  | ">="    { GTE }
  | "<="    { LTE }
  | '+' { PLUS }
  | '-' { MINUS }
  | '*' { TIMES }
  | '/' { DIV }
  | '&' { LAND }
  | "||" { LOR }
  | '^' { LXOR }
  | ">>" { SHR }
  | "<<" { SHL }
  | eof { EOF }
  | _ { raise (Illegal_character (Lexing.lexeme_start lexbuf, Lexing.lexeme lexbuf)) }

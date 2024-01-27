(**********************************************************************)
(*                                                                     *)
(*       This file is part of the RFSM Light software package          *)
(*                                                                     *)
(*  Copyright (c) 2019-present, Jocelyn SEROT (jocelyn.serot@uca.fr)   *)
(*                       All rights reserved.                          *)
(*                                                                     *)
(*    This source code is licensed under the license found in the      *)
(*      LICENSE file in the root directory of this source tree.        *)
(*                                                                     *)
(***********************************************************************)

(* This is copied from files
  - <rfsm>/src/host/lib/lexer.cppo.mll
  - <rfsm>/src/guests/std/bin/guest_kw.mll
  - <rfsm>/src/guests/std/bin/guest_rules.mll
*)

{
open Parsers

exception Lexical_error of string

(* The table of keywords *)

let keyword_table = [
"true",  BOOL(true);
"false", BOOL(false);
]
}

rule main = parse
  | [' ' '\t' '\010' '\013' ] +
      { main lexbuf }
  | ['a'-'z' ] ( ['A'-'Z' 'a'-'z' '0'-'9' '_' ] ) *
      { let s = Lexing.lexeme lexbuf  in
        try List.assoc s keyword_table
        with Not_found -> LID s }
  | ['A'-'Z' 'a'-'z' ] ( ['A'-'Z' 'a'-'z' '0'-'9' '_' ] ) *
      { UID (Lexing.lexeme lexbuf) }
  | ['0'-'9']+
      { INT (int_of_string(Lexing.lexeme lexbuf)) }
  | ['0'-'9']+ ('.' ['0'-'9']*)? (['e' 'E'] ['+' '-']? ['0'-'9']+)?
      { FLOAT (float_of_string(Lexing.lexeme lexbuf)) }
  | "(" { LPAREN }
  | ")" { RPAREN }
  | "<" { LT }
  | ">" { GT }
  | "," { COMMA }
  | ":" { COLON }
  | ":=" { COLEQ }
  | "=" { EQUAL }
  (* | "'" [^ '\\' '\'' '\010' '\013'] "'" *)
  (*     { CHAR(Lexing.lexeme_char lexbuf 1) } *)
  | "[" { LBRACKET }
  | "]" { RBRACKET }
  | "+"  { PLUS }
  | "-"  { MINUS }
  | "*"  { TIMES }
  | "/"  { DIV }
  | "%"  { MOD }
  | "!=" { NOTEQUAL }
  | '%' { MOD }
  | '&' { LAND }
  | "||" { LOR }
  | '^' { LXOR }
  | ">>" { SHR }
  | "<<" { SHL }
  | ">="    { GTE }
  | "<="    { LTE }
  | "+." { FPLUS }
  | "-." { FMINUS }
  | "*." { FTIMES }
  | "/." { FDIV }
  | "?" { QMARK }
  (* | "::" { COLONCOLON } *)
  | eof { EOF }
  | _ { raise (Lexical_error(Lexing.lexeme lexbuf)) }


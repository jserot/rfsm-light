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
  - <rfsm>/src/guests/std/bin/guest_parser.mly
  - <rfsm>/src/host/lib/host_parser.mly
*)

%token <string> LID
%token <string> UID
%token <int> INT
%token RPAREN LPAREN
%token EQUAL
%token LT GT
%token COLON
%token COLEQ
%token COMMA
%token EOF

%token <bool> BOOL
%token <float> FLOAT
/* %token <char> CHAR */
%token PLUS MINUS TIMES DIV MOD
%token FPLUS FMINUS FTIMES FDIV
%token LAND LOR LXOR
%token SHR SHL
%token NOTEQUAL
%token LBRACKET RBRACKET
/* %token ENUM */
/* %token RECORD */
%token QMARK
%token LTE GTE
/* %token COLONCOLON */

%nonassoc QMARK               (* Lowest precedence *)
%left EQUAL NOTEQUAL GT LT GTE LTE
%left SHR SHL
%left LAND LOR LXOR
%left PLUS MINUS
%left TIMES DIV MOD
%left FPLUS FMINUS
%left FTIMES FDIV
/* %nonassoc COLONCOLON */
%nonassoc prec_unary_minus         (* Highest precedence *)

%start <expr_parse_result> expr1
%start <action_parse_result> action1
%start <state_valuation_parse_result> state_valuation1

%{
open Misc

let mk_expr id = (true, StringSet.singleton id)
let mk_empty_expr () = (true, StringSet.empty)
let mk_lhs id = (true, StringSet.singleton id, StringSet.empty)
let to_lhs (ok,rhs) = (ok, StringSet.empty, rhs)
               
let (++) (ok1,rds1) (ok2,rds2) = match ok1, ok2 with
    | true, true -> true, StringSet.union rds1 rds2 
    | _, _ -> false, StringSet.empty
let (+++) (ok1,wrs1,rds1) (ok2,wrs2,rds2) = match ok1, ok2 with
    | true, true -> true, StringSet.union wrs1 wrs2, StringSet.union rds1 rds2 
    | _, _ -> false, StringSet.empty, StringSet.empty
%}

%%

expr1:
  | e = expr EOF { e }

expr:
  | e = simple_expr { e }
  | e1 = expr PLUS e2 = expr { e1 ++ e2 }
  | e1 = expr MINUS e2 = expr { e1 ++ e2 }
  | e1 = expr TIMES e2 = expr { e1 ++ e2 }
  | e1 = expr DIV e2 = expr { e1 ++ e2 }
  | e1 = expr MOD e2 = expr { e1 ++ e2 }
  | e1 = expr EQUAL e2 = expr { e1 ++ e2 } 
  | e1 = expr NOTEQUAL e2 = expr {  e1 ++ e2 } 
  | e1 = expr GT e2 = expr { e1 ++ e2 }  
  | e1 = expr LT e2 = expr { e1 ++ e2 }  
  | e1 = expr GTE e2 = expr { e1 ++ e2 }  
  | e1 = expr LTE e2 = expr { e1 ++ e2 }  
  | e1 = expr LAND e2 = expr { e1 ++ e2 }  
  | e1 = expr LOR e2 = expr { e1 ++ e2 }  
  | e1 = expr LXOR e2 = expr { e1 ++ e2 }  
  | e1 = expr SHR e2 = expr { e1 ++ e2 }  
  | e1 = expr SHL e2 = expr { e1 ++ e2 }  
  | e1 = expr FPLUS e2 = expr { e1 ++ e2 }  
  | e1 = expr FMINUS e2 = expr { e1 ++ e2 }  
  | e1 = expr FTIMES e2 = expr { e1 ++ e2 }  
  | e1 = expr FDIV e2 = expr { e1 ++ e2 }  
  | subtractive e=expr %prec prec_unary_minus { e }
  | a = LID LBRACKET i=expr RBRACKET { mk_expr a ++ i }
  | LID LPAREN args=separated_list(COMMA,expr) RPAREN { List.fold_left ( ++ ) (mk_empty_expr ()) args }
  (* | a = LID DOT f = LID { (true, StringSet.empty) }*)
  | a = LID LBRACKET i1=expr COLON i2=expr RBRACKET { mk_expr a ++ i1 ++ i2 }
  | e1 = expr QMARK e2 = expr COLON e3 = expr { e1 ++ e2 ++ e3 }
  /* | e = expr COLONCOLON t = type_expr { e1 ++ t } */

simple_expr:
  | v = LID { mk_expr v }
  | c = UID { mk_expr c }
  | scalar_const { mk_empty_expr () }
  | LPAREN e = expr RPAREN { e }

subtractive:
  | MINUS                                       { mk_empty_expr () }
  | FMINUS                                      { mk_empty_expr () }

%public scalar_const:
  | INT { mk_empty_expr() }
  | MINUS INT { mk_empty_expr() }
  | BOOL { mk_empty_expr() }
  | FLOAT { mk_empty_expr() }
  | MINUS FLOAT { mk_empty_expr() }
  (* | c = CHAR { mk_empty_expr() } *)

%public lval:
  | v = LID { mk_lhs v }
  | id = LID LBRACKET idx = expr RBRACKET { mk_lhs id +++ to_lhs idx }
  (* | a=LID DOT f=LID { mk ~loc:$sloc (LvalRField (mk_ident a, f)) } *)
  | a=LID LBRACKET hi=expr COLON lo=expr RBRACKET { mk_lhs a +++ to_lhs hi +++ to_lhs lo }

action1:
  | a = action EOF { a }
action:
  | l = lval COLEQ e = expr { l +++ to_lhs e }

state_valuation1:
  | v = state_valuation EOF { v }
state_valuation:
  | l=LID  EQUAL scalar_const { (true, l) }

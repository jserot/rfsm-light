%token <int> INT
%token <bool> BOOL
%token <string> LID
%token <string> UID
%token PERIODIC
%token SPORADIC
%token VALUECHANGES
%token DOT
(* %token COMMA *)
%token COLON
%token COLONCOLON
%token EQUAL
%token NOTEQUAL
%token LPAREN
%token RPAREN
%token LT
%token GT
%token LTE
%token GTE
%token PLUS MINUS TIMES DIV
%token SHL SHR
(* %token LAND LOR LXOR *)
(* %token BARBAR *)
%token COLEQ
%token TYEVENT
%token TYINT
%token TYBOOL
%token LBRACKET
%token RBRACKET
%token EOF

(* Precedences and associativities for expressions *)

%left EQUAL NOTEQUAL GT LT GTE LTE
%left SHR SHL
(* %left LAND LOR LXOR *)
%left PLUS MINUS
%left TIMES DIV
%nonassoc COLONCOLON
(* %nonassoc prec_unary_minus         (\* Highest precedence *\) *)

%start <expr list> guard
%start <action> action
%start <stimuli> stimuli
%start <type_expr> type_expr
(* %start <type_expr> typ *)

%{
open Syntax
%}

%%

expr:
  | e = simple_expr
      { e }
  | e1 = expr SHR e2 = expr
      { EBinop (">>", e1, e2) }
  | e1 = expr SHL e2 = expr
      { EBinop ("<<", e1, e2) }
  (* | e1 = expr LAND e2 = expr
   *     { EBinop ("&", e1, e2) }
   * | e1 = expr LOR e2 = expr
   *     { EBinop ("|", e1, e2) }
   * | e1 = expr LXOR e2 = expr
   *     { EBinop ("^", e1, e2) } *)
  | e1 = expr PLUS e2 = expr
      { EBinop ("+", e1, e2) }
  | e1 = expr MINUS e2 = expr
      { EBinop ("-", e1, e2) }
  | e1 = expr TIMES e2 = expr
      { EBinop ("*", e1, e2) }
  | e1 = expr DIV e2 = expr
      { EBinop ("/", e1, e2) }
  | e1 = expr EQUAL e2 = expr
      { EBinop ("=", e1, e2) }
  | e1 = expr NOTEQUAL e2 = expr
      { EBinop ("!=", e1, e2) }
  | e1 = expr GT e2 = expr
      { EBinop (">", e1, e2) }
  | e1 = expr LT e2 = expr
      { EBinop ("<", e1, e2) }
  | e1 = expr GTE e2 = expr
      { EBinop (">=", e1, e2) }
  | e1 = expr LTE e2 = expr
      { EBinop ("<=", e1, e2) }
  | e = expr COLONCOLON t = type_expr
      { ECast (e, t) }
  | a = LID LBRACKET i=expr RBRACKET 
      { EArr (a,i) }

simple_expr:
  | v = LID
      { EVar v }
  | c = INT
      { EInt c }
  | c = BOOL
      { EBool c }
  | MINUS c=INT
      { EInt (-c) }
  | LPAREN e = expr RPAREN
      { e }

value:
  | v = INT
      { Int v }
  | v = BOOL
      { Bool v }

value_change:
  | t=INT COLON v=value  { (t,v) }

stimuli:
  | PERIODIC p=INT t1=INT t2=INT EOF { Periodic (p,t1,t2) }
  | SPORADIC ts=nonempty_list(INT)  EOF{ Sporadic ts }
  | VALUECHANGES vcs=nonempty_list(value_change) EOF { ValueChanges vcs }

guard: 
  | g=separated_list(DOT, expr) EOF { g }

action:
  | e=LID EOF { Emit e }
  | v=LID COLEQ e=expr EOF { Assign (v,e) }

(* actions: 
 *   | acts=separated_list(COMMA, action) EOF { acts } *)

(* typ:
 *   | t=type_expr EOF { t } *)

type_expr:
  | TYEVENT EOF { TyEvent }
  | TYINT a=int_annot EOF { TyInt a }
  | TYBOOL EOF EOF { TyBool }

int_annot:
    | (* Nothing *)
      { TA_none }
    | LT sz=INT GT
        { TA_size sz }
    | LT lo=INT COLON hi=INT GT
        { TA_range (lo, hi) }

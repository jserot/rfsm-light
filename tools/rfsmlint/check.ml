open Model 

let allowed_types = ["int"; "bit"; "event"; "bool"]

exception Missing_name 
exception Missing_init_state 
exception Invalid_state_name of string
exception Invalid_io_name of string
exception Invalid_io_type of string * string (* name, type *)
exception Missing_event of string (* enclosing transition *)
exception Invalid_event of string * string (* name, enclosing transition *)
exception Invalid_stimuli of string * string * string (* bound io, kind, reason *)
exception Illegal_stimuli of string * string (* bound io, text *)
exception Warning_no_stimuli of string (* bound io *)
exception Undefined_symbol of string * string (* name, enclosing context *)
exception Illegal_output_ref of string * string (* name, enclosing context *)
exception Illegal_event_ref of string * string (* name, enclosing context *)
exception Illegal_notify of string * string (* name, enclosing context *)
exception Illegal_input_ref of string * string (* name, enclosing context *)

exception Parse_error of string * string

let parse what f s = 
  let lexbuf =  Lexing.from_string s in
  try lexbuf |> f Lexer.main 
  with _ -> raise (Parse_error (what,s))

let parse_guard = parse "guard" Parsers.guard 
let parse_action = parse "action" Parsers.action
let parse_stimuli = parse "stimuli" Parsers.stimuli
let parse_type = parse "type" Parsers.type_expr

let check_state s = 
  if not (Misc.starts_uppercase s.state_id) then raise (Invalid_state_name s.state_id)

let check_states m =
  List.iter check_state m.states;
  if not (List.exists (fun s -> s.state_id = "_init") m.states) then raise Missing_init_state

let check_stimuli io =
  if io.io_desc = "" then
    raise (Warning_no_stimuli io.io_name)
  else
    match parse_stimuli io.io_desc with
    | Syntax.Periodic (p,t1,t2) ->
       if t2 > t1 then ()
       else raise (Invalid_stimuli (io.io_name, "periodic", "t1 >= t2"))
    | Syntax.Sporadic ts ->
       if Misc.is_ordered ts then ()
       else raise (Invalid_stimuli (io.io_name, "sporadic", "out of order event dates"))
    | Syntax.ValueChanges vcs ->
       if Misc.is_ordered @@ List.map fst vcs then ()
       else raise (Invalid_stimuli (io.io_name, "value changes", "out of order event dates"))
    | exception e ->
       raise e
       (* raise (Illegal_stimuli (io.io_name, io.io_desc)) *)

let check_type id ty = 
  try ignore (parse_type ty)
  with _ -> raise (Invalid_io_type (id, ty))

let check_io ~withStimuli io = 
  if not (Misc.starts_lowercase io.io_name) then raise (Invalid_io_name io.io_name);
  check_type io.io_name io.io_type;
  if io.io_kind = "in" && withStimuli then check_stimuli io

let check_ios ~withStimuli m = List.iter (check_io ~withStimuli) m.ios

let string_of_transition t = t.t_src_state ^ "->" ^ t.t_dst_state

let check_event ios t e = 
  if e = "" then
    raise (Missing_event (string_of_transition t))
  else
    match List.assoc_opt e ios with
    | Some ("in", "event", _) -> ()
    | _ -> raise (Invalid_event (e,string_of_transition t))

let check_rhs_var ctx ios v = 
  match List.assoc_opt v ios with
  | None -> raise (Undefined_symbol (v,ctx))
  | Some ("out", _, _) -> raise (Illegal_output_ref (v,ctx))
  | Some (_, _, _) -> ()

let check_expr ctx ios e = 
  (* Check that all symbols occuring in an expression refer to an input or a local variable *)
  List.iter (check_rhs_var ctx ios) (Syntax.vars_of_expr e)

let check_guard ios t guard =
  let es = parse_guard guard in
  List.iter (check_expr ("guard expression \"" ^ guard ^ "\"") ios) es

let check_action ios action = 
  let open Syntax in
  let act = parse_action action in
  let ctx = "action \"" ^ action ^ "\"" in
  match act with
  | Emit e -> 
     begin 
       match List.assoc_opt e ios with
       | Some (kind, ty, _) ->
          if ty <> "event" then raise (Illegal_notify (e, ctx));
          if kind <> "out" then raise (Illegal_event_ref (e, ctx))
       | None -> raise (Undefined_symbol (e,ctx))
     end
  | Assign (lhs,e) ->
     let v = lhs_name lhs in
     let ctx' = "the left-hand side of " ^ ctx in
     let ctx'' = "the right-hand side of " ^ ctx in
      begin 
       match List.assoc_opt v ios with
       | Some (kind, ty, _) ->
          if kind = "in" then raise (Illegal_input_ref (v, ctx'));
          check_expr ctx'' ios e
       | None -> raise (Undefined_symbol (v,ctx'))
      end
                                      
let check_actions ios t actions =
  match String.split_on_char ',' actions |> List.filter (function "" -> false | _ -> true) with
    | [] -> ()
    | acts -> List.iter (check_action ios) acts

let check_transition ios t =
  match t.t_src_state with
  | "_init" ->
     check_actions ios t t.t_actions
  | _ ->
     check_event ios t t.t_event;
     check_guard ios t t.t_guard;
     check_actions ios t t.t_actions

let check_transitions ios m = List.iter (check_transition ios) m.transitions

let check_name m = 
  if m.name = "" then raise Missing_name
 
let check_model ?(withStimuli=true) m = 
  check_name m;
  check_states m;
  check_ios ~withStimuli m;
  let ios = List.map (fun io -> io.io_name, (io.io_kind, io.io_type, io.io_desc)) m.ios in
  check_transitions ios m


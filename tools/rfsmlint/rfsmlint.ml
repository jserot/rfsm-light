open Check

type cfg = {
  mutable check_stimuli: bool;
}

let cfg = {
  check_stimuli = true;
  }

let usage = "usage: checker [options...] file"

let source_file = ref "<no source_file>"

let anonymous fname = source_file := fname

let options = [
 "-no_stimuli", Arg.Unit (fun () -> cfg.check_stimuli <- false), "do check stimuli definition when checking syntax"
]

let main () =
  try
    Sys.catch_break true;
    Arg.parse options anonymous usage;
    let m = Model.from_file !source_file in
    check_model ~withStimuli:cfg.check_stimuli m;
    exit 0
  with
  | Check.Parse_error (what,txt) -> Printf.eprintf "Error when parsing %s \"%s\".\n" what txt; exit 1
  | Missing_name -> Printf.eprintf "No name given for model.\n"; exit 1
  | Missing_init_state -> Printf.eprintf "No initial state defined.\n"; exit 1
  | Invalid_state_name n -> Printf.eprintf "Invalid state name: \"%s\" does start with an uppercase letter.\n" n; exit 1
  | Invalid_io_name n -> Printf.eprintf "Invalid io/var name: \"%s\" does not start with a lowercase letter.\n" n; exit 1
  | Invalid_io_type (n,t) ->
     Printf.eprintf "Invalid type \"%s\" for io/var \"%s\".\n" t n; exit 1
  | Missing_event t ->
     Printf.eprintf "No triggering event for transition %s.\n" t; exit 1
  | Invalid_event (e,t) ->
     Printf.eprintf "Invalid event for transition %s: \"%s\" does not refer to an input with type \"event\".\n" t e ; exit 1
  | Invalid_stimuli (io,kind,why) ->
     Printf.eprintf "Invalid %s stimuli description for input \"%s\": %s.\n" kind io why; exit 1
  | Illegal_stimuli (io,txt) ->
     (* let usage =
      *   begin match kind with
      *   | Syntax.Periodic -> "Periodic <period> <start_time> <end_time>"
      *   | Syntax.Sporadic -> "Sporadic <t1> <t2> ..."
      *   | Syntax.ValueChanges -> "Sporadic <t1> <t2> ..."
      *   end in *)
     Printf.eprintf "Syntax error in stimuli description for input \"%s\": \"%s\"\n" io txt; exit 1
  | Warning_no_stimuli io ->
     Printf.eprintf "Warning: no stimuli given for input \"%s\": no simulation can be carried out.\n" io; exit 1
  | Undefined_symbol (n,ctx) ->
     Printf.eprintf "The symbol \"%s\" occuring in %s is undefined.\n" n ctx; exit 1
  | Illegal_output_ref (n,ctx) ->
     Printf.eprintf "The output \"%s\" cannot occur in %s.\n" n ctx; exit 1
  | Illegal_event_ref (n,ctx) ->
     Printf.eprintf "The symbol \"%s\" occuring in %s is not an output event.\n" n ctx; exit 1
  | Illegal_notify (n,ctx) ->
     Printf.eprintf "The symbol \"%s\" occuring in %s is not an event.\n" n ctx; exit 1
  | Illegal_input_ref (n,ctx) ->
     Printf.eprintf "The input \"%s\" cannot occur in %s.\n" n ctx; exit 1
  | End_of_file -> exit 0
  | Sys.Break -> flush stderr; exit 2
  | e ->
     Printf.eprintf "Internal error: %s.\n" (Printexc.to_string e);
     flush stderr; exit 3

let _ = Printexc.print main ()

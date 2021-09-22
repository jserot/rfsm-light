(**********************************************************************)
(*                                                                    *)
(*        This file is part of the RFSM Light software package        *)
(*                     github.com/jserot/fsml                         *)
(*                                                                    *)
(*  Copyright (c) 2020-present, Jocelyn SEROT.  All rights reserved.  *)
(*                                                                    *)
(*  This source code is licensed under the license found in the       *)
(*  LICENSE file in the root directory of this source tree.           *)
(*                                                                    *)
(**********************************************************************)

(* Syntactic and semantic FSM model checker *)

type model = {
  name: string;
  states: state list;
  ios: io list;
  transitions: transition list;
} [@@deriving show {with_path=false}, of_yojson {strict=false}]

and state = {
  state_id: string; [@key "id"]
  state_x: (float [@default 0.0]); [@key "x"]
  state_y: (float [@default 0.0]) [@key "y"]
}

and io = {
  io_desc: string; [@key "desc"]
  io_kind: string; [@key "kind"]
  io_name: string; [@key "name"]
  io_type: string; [@key "type"]
}

and transition = {
  t_actions: string; [@key "actions"]
  t_dst_state: string; [@key "dst_state"]
  t_event: string; [@key "event"]
  t_guard: string; [@key "guard"]
  t_location: (int [@default 0]); [@key "location"]
  t_src_state: string; [@key "src_state"]
}

(* let to_string m =
 *   m |> model_to_yojson |> Yojson.Basic.to_string  *)

(* let from_string s = 
 *   match Yojson.Safe.from_string s |> model_of_yojson with
 *   | Ok v -> v
 *   | Error _ -> Yojson.json_error "Model.from_string: invalid JSON string" *)

(* let to_file fname m = 
 *   m |> model_to_yojson |> Yojson.Basic.to_file fname;
 *   Printf.printf "Wrote file %s\n" fname
 *)   

let from_file fname = 
  match fname |> Yojson.Safe.from_file |> model_of_yojson with
  | Ok v -> v
  | Error _ -> Yojson.json_error "Model.from_file: invalid JSON file"

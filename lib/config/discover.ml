open Base
open Stdio
module C = Configurator

let write_sexp fn sexp =
  Out_channel.write_all fn ~data:(Sexp.to_string sexp)

let () =
  C.main ~name:"mirage-xen-ocaml" (fun c ->
    let default : C.Pkg_config.package_conf =
      { libs   = []
      ; cflags = []
      }
    in
    let conf =
      match C.Pkg_config.get c with
      | None -> default
      | Some pc ->
        Option.value (C.Pkg_config.query pc ~package:"mirage-xen-ocaml") ~default
    in

    write_sexp "c_flags_xen.sexp" (sexp_of_list sexp_of_string conf.cflags))

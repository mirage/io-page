(*
 * Copyright (c) 2011-2012 Anil Madhavapeddy <anil@recoil.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *)

open Bigarray

type t = (char, int8_unsigned_elt, c_layout) Array1.t

type buffer = Cstruct.t

let page_size = 1 lsl 12

let length t = Array1.dim t

external alloc_pages: int -> t = "caml_alloc_pages"

let get n =
  if n < 0
  then raise (Invalid_argument "Io_page.get cannot allocate a -ve number of pages")
  else if n = 0
  then Array1.create char c_layout 0
  else
    try alloc_pages n with _ ->
    Gc.compact ();
    try alloc_pages n with _ -> raise Out_of_memory

let get_order order = get (1 lsl order)

let to_pages t =
  assert(length t mod page_size = 0);
  let rec loop off acc =
    if off < (length t)
    then loop (off + page_size) (Bigarray.Array1.sub t off page_size :: acc)
    else acc in
  List.rev (loop 0 [])

let pages n =
  let rec inner acc n =
    if n > 0 then inner ((get 1)::acc) (n-1) else acc
  in inner [] n

let pages_order order = pages (1 lsl order)

let round_to_page_size n = ((n + page_size - 1) lsr 12) lsl 12

let to_cstruct t = Cstruct.of_bigarray t

let to_string t =
  let result = String.create (length t) in
  for i = 0 to length t - 1 do
    result.[i] <- t.{i}
  done;
  result

let blit src dest = Bigarray.Array1.blit src dest

(* TODO: this is extremely inefficient.  Should use a ocp-endian
   blit rather than a byte-by-byte *)
let string_blit src srcoff dst dstoff len =
  for i = 0 to len - 1 do
    dst.{i+dstoff} <- src.[i+srcoff]
  done

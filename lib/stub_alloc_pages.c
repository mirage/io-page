/*
 * Copyright (C) 2012-2013 Citrix Inc
 * Copyright (c) 2010-2011 Anil Madhavapeddy <anil@recoil.org>
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
 */

#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/signals.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

#define PAGE_SIZE 4096
#include <stdlib.h>

/* Allocate a page-aligned bigarray of length [n_pages] pages.
   Since CAML_BA_MANAGED is set the bigarray C finaliser will
   call free() whenever all sub-bigarrays are unreachable.
 */
CAMLprim value
caml_alloc_pages(value n_pages)
{
  CAMLparam1(n_pages);
  size_t len = Int_val(n_pages) * PAGE_SIZE;
  /* If the allocation fails, return None. The ocaml layer will
     be able to trigger a full GC which just might run finalizers
     of unused bigarrays which will free some memory. */
  void* block = NULL;
  int ret = posix_memalign(&block, PAGE_SIZE, len);

  if (ret < 0) {
    caml_failwith("memalign");
  }
  CAMLreturn(caml_ba_alloc_dims(CAML_BA_CHAR | CAML_BA_C_LAYOUT | CAML_BA_MANAGED, 1, block, len));
}

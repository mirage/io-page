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
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#define PAGE_SIZE 4096
#ifdef _WIN32
#include <malloc.h>
#endif

#include <string.h>

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/bigarray.h>

/* Allocate a page-aligned bigarray of length [n_pages] pages.
   Since CAML_BA_MANAGED is set the bigarray C finaliser will
   call free() whenever all sub-bigarrays are unreachable.
 */
CAMLprim value
mirage_iopage_alloc_pages(value did_gc, value n_pages)
{
  CAMLparam2(did_gc, n_pages);
  size_t len = Int_val(n_pages) * PAGE_SIZE;
  /* If the allocation fails, return None. The ocaml layer will
     be able to trigger a full GC which just might run finalizers
     of unused bigarrays which will free some memory. */
#ifdef _WIN32
  /* NB we can't use _aligned_malloc because we can't get OCaml to
     finalize with _aligned_free. Regular free() will not work. */
  static int printed_warning = 0;
  if (!printed_warning) {
    printed_warning = 1;
    printf("WARNING: Io_page on Windows doesn't guarantee alignment\n");
  }
  void *block = malloc(len);
  if (block == NULL) {
#else
  void* block = NULL;
  int ret = posix_memalign(&block, PAGE_SIZE, len);
  if (ret < 0) {
#endif
    if (Bool_val(did_gc))
      printf("Io_page: memalign(%d, %zu) failed, even after GC.\n", PAGE_SIZE, len);
    caml_raise_out_of_memory();
  }
  /* Explicitly zero the page before returning it */
  memset(block, 0, len);

  CAMLreturn(caml_ba_alloc_dims(CAML_BA_CHAR | CAML_BA_C_LAYOUT | CAML_BA_MANAGED, 1, block, len));
}

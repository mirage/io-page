1.6.1 (13-Jun-2016)
* Fix crash on Win32 when the GC calls the wrong `free` function (#31 from @djs55)
* Add a LICENSE file (#32 from @djs55)

1.6.0 (3-Mar-2016)
* Add support for Win32

1.5.1 (18-Mar-2015)
* Avoid cyclic dependency with `mirage-xen` (#21, patch from @hannesm)

1.5.0 (16-Mar-2015)
* Fix equallity of io-pages (#17, patch from @hannesm)
* Import C stubs from mirage-platform (#18, patch from @hannesm)

1.4.0 (28-Jan-2015):
* Add `of_cstruct_exn` as a safe way to turn a Cstruct back into an `Io_page.t`.
* Expose `page_size` constant in interface.

1.3.0 (26-Jan-2015):
* Make `Io_page.t` type private. Otherwise, any old array of bytes
  can be used as an `Io_page.t`. (#14)
* Switch to using the `Bytes` module instead of `String`.

1.2.0 (21-Nov-2014):
* Add `Io_page.get_buf` which allocates an Io_page
  and immediately turns it into a Cstruct that spans the
  entire page.
* Improve ocamldoc for exported functions.
* Add OPAM 1.2 file for easier local pinning workflow.

1.1.1 (15-Feb-2014):
* Improve portability on *BSD by not including `malloc.h` and
  just using `stdlib.h` instead.

1.1.0 (30-Jan-2014):
* Do not depend directly on the mirage-types signature to help
  break a circular dependency.  The `portable` test still exists
  to check against when changing anything.

1.0.0 (16-Jan-2014):
* Refactor the library into one pure-OCaml library (`io-page`)
  library and the Unix C-bindings (`io-page.unix`).

0.9.9 (07-Dec-2013):
* Add Travis CI scripts.
* Switch to an OASIS build system to fix META and C bindings.

0.1.0 (02-Dec-2013):
* Initial public release, based on mirage/mirage-platform#0.9.8

#!/bin/sh

set -ex

opam install -y mirage
(cd unikernel && mirage configure -t hvt && make depends && mirage build && mirage clean && cd ..) || exit 1

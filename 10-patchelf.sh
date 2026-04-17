#!/bin/bash
#
# Wonderful Toolchain post-installation binary relocator
#
# Copyright (c) 2026 Adrian "asie" Siekierka
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this
# software and associated documentation files (the "Software"), to deal in the Software
# without restriction, including without limitation the rights to use, copy, modify,
# merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

WF_PATH_REL="$(dirname -- "${BASH_SOURCE[0]}")"/../..
WF_PATH="$(realpath -- "$WF_PATH_REL")"

if [ "$WF_PATH" = "/opt/wonderful" ] && [ "$1" != "force" ]; then
	# This is the default path, so nothing has to be done.
	exit 0
fi

WF_INTERP="$(echo "$WF_PATH"/lib/ld-musl-*.so.1)"
find "$WF_PATH" -type f -executable ! -name "libc.so" -exec "$WF_PATH"/bin/wf-patchelf --set-rpath "$WF_PATH/lib" {} \; 2>/dev/null
find "$WF_PATH" -type f -executable ! -name "*.so*" -exec "$WF_PATH"/bin/wf-patchelf --set-interpreter "$WF_INTERP" {} \; 2>/dev/null

cd "$WF_PATH"/bin
for i in `find . -type f -executable`; do
	sed -i '1 s@#!/opt/wonderful@#!'"$WF_PATH"'@' "$i"
done

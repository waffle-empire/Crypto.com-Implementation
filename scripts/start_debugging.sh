#!/bin/bash

./scripts/compile_debug.sh

# starts debugger and forwards arguments to gdb
exec gdb -ex=r --args bin/Debug/CryptoComImpl "$@"
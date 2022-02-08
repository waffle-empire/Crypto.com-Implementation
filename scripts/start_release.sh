#!/bin/bash

./scripts/compile_release.sh

# starts debugger and forwards arguments to gdb
exec bin/Release/CryptoComImpl "$@"
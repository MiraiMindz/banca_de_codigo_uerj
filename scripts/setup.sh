#!/usr/bin/env bash

if [[ ! $(command -v clang) ]]; then
    pacman -Syyy --no-confirm cmake llvm clang lld lldb polly valgrind
fi

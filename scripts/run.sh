#!/usr/bin/env bash
mkdir -p "$(pwd)/.backup"
mv "$(pwd)/build/src/banca_de_codigo_uerj.bin" "$(pwd)/.backup/banca_de_codigo_uerj_$(date +%0d-%0m-%Y_%0H:%0M:%0S_%0N).bin"

sh "$(pwd)/scripts/clean.sh"
sh "$(pwd)/scripts/build.sh"
sh "$(pwd)/scripts/sanatizecheck.sh"
mv "$(pwd)/build/src/banca_de_codigo_uerj" "$(pwd)/build/src/banca_de_codigo_uerj.bin"

LD_PRELOAD=/usr/lib/libasan.so $(pwd)/build/src/banca_de_codigo_uerj.bin

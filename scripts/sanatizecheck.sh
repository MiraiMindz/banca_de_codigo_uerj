#!/usr/bin/env bash

valgrind -s --leak-check=full --track-origins=yes ./build/src/banca_de_codigo_uerj

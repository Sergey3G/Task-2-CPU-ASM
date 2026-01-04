#!/bin/bash
set -e

ROOT_DIR=$(dirname "$(realpath "$0")")

ASM_DIR="$ROOT_DIR/assembler"
CPU_DIR="$ROOT_DIR/virtual_processor"

ASM_BIN="$ASM_DIR/build/asm"
CPU_BIN="$CPU_DIR/build/cpu"

ASM_INPUT="$ASM_DIR/instructions.asm"
BYTECODE="$CPU_DIR/byte_code.bin"

echo "==== Building assembler ===="
cd "$ASM_DIR"
make clean
make

echo "==== Running assembler ===="
"$ASM_BIN" "$ASM_INPUT" "$BYTECODE"

echo "==== Building virtual processor ===="
cd "$CPU_DIR"
make clean
make

echo "==== Running virtual processor ===="
"$CPU_BIN" "$BYTECODE"

echo "==== Done ===="



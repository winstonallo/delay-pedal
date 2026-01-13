#!/usr/bin/env python3

import json
import subprocess
import sys
from pathlib import Path

print("Generating compile_commands.json...")
result = subprocess.run(
    ["pio", "run", "-t", "compiledb"],
)

if result.returncode != 0:
    print("Failed to generate compile_commands.json")
    sys.exit(1)

with open("compile_commands.json", "r") as f:
    compile_commands = json.load(f)


if compile_commands:
    cmd = compile_commands[0]
    compiler = cmd.get("command", "").split()[0]

    toolchain_base = Path(compiler).parent.parent

    gcc_version_dirs = list((toolchain_base / "lib" / "gcc" / "arm-none-eabi").glob("*"))
    if gcc_version_dirs:
        gcc_version = gcc_version_dirs[0].name

        compile_flags = [
            f"-I{toolchain_base}/arm-none-eabi/include",
            f"-I{toolchain_base}/lib/gcc/arm-none-eabi/{gcc_version}/include",
            f"-I{toolchain_base}/lib/gcc/arm-none-eabi/{gcc_version}/include-fixed",
            "-Wall",
            "-Wextra",
            "-Werror",
        ]

        clangd_config = f"""CompileFlags:
  Add:
{chr(10).join(f"    - {inc}" for inc in compile_flags)}
    - -w

Diagnostics:
  Suppress:
      - pp_file_not_found_angled_include_not_fatal
      - pp_file_not_found
"""

        with open(".clangd", "w") as f:
            f.write(clangd_config)

        print(f"Generated .clangd with {len(compile_flags)} include paths")
        print(f"GCC version detected: {gcc_version}")

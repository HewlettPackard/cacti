# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

CACTI 7.0 (with CACTI-3DD extensions) is a cache/memory access time, cycle time, area, leakage, and dynamic power model. Originally from HP Labs, it models SRAM, eDRAM, DRAM, and non-volatile memory for architectural simulation. It is used as a library by McPAT (power modeling tool) and as a standalone tool.

## Build Commands

```bash
make          # Debug build (default), output: ./cacti, objects in obj_dbg/
make dbg      # Same as above
make opt      # Optimized build, objects in obj_opt/
make clean    # Clean all builds
```

The build uses `g++-13 -m64`. Debug builds use `-O0 -DNTHREADS=1`. Optimized builds use `-msse2 -DNTHREADS=8` (configurable via `NTHREADS` env var).

## Running

```bash
./cacti -infile cache.cfg          # Run with a config file
./cacti -infile dram.cfg           # DRAM configuration
```

Sample configs: `cache.cfg`, `dram.cfg`, `ddr3.cfg`, `lpddr.cfg`, and files under `sample_config_files/`. Regression tests are listed in `regression.test` (references configs in `test_configs/`).

## Architecture

**Entry point**: `main.cc` parses CLI args and calls `cacti_interface()` (defined in `cacti_interface.cc`).

**Key interfaces** (`cacti_interface.h`):
- `cacti_interface(string infile)` — config file interface
- `cacti_interface(InputParameter*)` — programmatic interface used by McPAT
- `init_interface(InputParameter*)` — McPAT integration point
- Multiple overloads with 52/54/63 positional args for different memory types (standard, NUCA, 3D DRAM)

**Core modeling pipeline**:
- `InputParameter` (in `cacti_interface.h`) — holds all configuration; `parse_cfg()` reads `.cfg` files
- `technology.cc` + `tech_params/*.dat` — technology parameters per node (16nm–180nm)
- `Ucache.cc` — top-level cache organization search; iterates over array partitioning parameters (Ndwl, Ndbl, Nspd, etc.) to find optimal design
- `uca.cc`/`uca.h` — Unified Cache Architecture modeling
- `bank.cc`/`mat.cc`/`subarray.cc` — hierarchical memory structure: banks contain mats, mats contain subarrays
- `htree2.cc` — H-tree interconnect modeling (address/data routing within banks)
- `decoder.cc` — row/column decoder modeling
- `wire.cc` — wire delay/power models for different wire types (global, semi-global, low-swing, etc.)
- `nuca.cc` — Non-Uniform Cache Architecture modeling with `router.cc`, `crossbar.cc`, `arbiter.cc`

**I/O and DRAM extensions**:
- `extio.cc`/`extio_technology.cc` — external I/O interface modeling (DDR3, DDR4, LPDDR2, WideIO, Serial)
- `memorybus.cc` — memory bus modeling
- `memcad.cc`/`memcad_parameters.cc` — MemCAD memory system design space exploration
- `TSV.cc` — Through-Silicon Via modeling for 3D DRAM (CACTI-3DD)
- `powergating.cc` — power gating support for sleep transistors

**Global state**: `parameter.h` defines `TechnologyParameter` (aliased as `g_tp`) — a large global struct holding all technology-specific electrical parameters, populated from `tech_params/*.dat` files.

**Output types**: `uca_org_t` is the primary result struct containing access_time, cycle_time, area, power, and detailed breakdowns for both tag and data arrays. `IO_org_t` holds I/O interface results.

## Config File Format

Config files use `-key value` syntax with `//` comments. Lines starting with `//` are ignored. Key parameters: `-size (bytes)`, `-block size (bytes)`, `-associativity`, `-technology (u)`, `-cache type` ("cache"/"ram"/"main memory"). See `cache.cfg` for a fully documented example.

## Code Conventions

- C++ without namespaces (uses `using namespace std` globally)
- No formal test framework; validation is done via regression configs in `regression.test`
- Header/source pairs: `foo.h`/`foo.cc` pattern throughout
- Build produces objects in `obj_dbg/` or `obj_opt/` directories

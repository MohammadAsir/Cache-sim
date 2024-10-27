
# Cache Simulator - `mycache.c`

This project, `mycache.c`, is a cache simulator written in C that simulates a cache with a specified trace file. It calculates cache hits, misses, and evictions based on the operations in the trace file. The program uses command-line arguments to set parameters and optionally outputs debug information.

## Features

- **Cache Simulation**: Simulates cache behavior by processing memory access instructions.
- **Verbose Mode**: Outputs detailed debug information if the `-v` flag is set.
- **Command-Line Options**: Flexible setup through command-line flags for cache configuration.

## Requirements

- A Linux system with GCC for compilation
- Standard libraries: `<stdio.h>`, `<stdlib.h>`, `<getopt.h>`

## Parameters

- **s**: Number of set index bits
- **E**: Associativity, or number of lines per set
- **b**: Number of block offset bits
- **t**: Trace file containing the cache operations
- **v**: Verbose flag; if enabled, prints detailed debug information

## Functions

### `main`
- **Purpose**: Simulate a cache based on a provided trace file.
- **Parameters**:
  - `argc` (int): Number of command-line arguments.
  - `argv` (char pointer array): Array of command-line arguments.
- **Output**: Number of hits, misses, and evictions in the cache.

## Usage

1. **Compiling**:
   ```bash
   gcc -o mycache mycache.c
   ```

2. **Running the Program**:
   ```bash
   ./mycache -s <set_index_bits> -E <associativity> -b <block_bits> -t <trace_file> [-v]
   ```

   - **Example**:
     ```bash
     ./mycache -s 4 -E 1 -b 4 -t tracefile.txt -v
     ```

3. **Explanation of Command-Line Arguments**:
   - `-s`: Sets the number of set index bits.
   - `-E`: Defines the associativity or lines per set.
   - `-b`: Sets the number of block offset bits.
   - `-t`: Specifies the path to the trace file.
   - `-v`: Enables verbose output (optional).

4. **Trace File Format**:
   - Each line represents a memory access operation with the format: `<operation> <address>,<size>`
   - Operations include:
     - `L`: Load operation
     - `S`: Store operation
     - `M`: Modify operation (involves a load followed by a store)

5. **Output**:
   - When verbose mode (`-v`) is enabled, the program outputs the details of each operation.
   - At the end, it prints the total hits, misses, and evictions.

### Example Output

1. Command:
   ```bash
   ./mycache -s 4 -E 1 -b 4 -t tracefile.txt -v
   ```

2. Output:
   ```
   L 7ff0000,4 miss 
   S 7ff0004,4 hit 
   M 7ff0008,4 miss eviction hit
   hits:10 misses:5 evictions:2
   ```

## Notes

- Ensure the trace file follows the correct format for accurate simulation.
- Improperly formatted arguments or missing flags may result in incorrect cache simulation or errors.

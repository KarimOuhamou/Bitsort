# Bitsort: Memory-Efficient Integer Sorting

**Authors:** Karim Ouhamou, Giuseppe Didonna

---

## Overview

**Bitsort** is a custom integer sorting algorithm designed to **minimize memory accesses**.  
It uses a **bit-packed array** to store frequencies of values, allowing the sorting process to be performed efficiently in **a single pass**.

This algorithm is especially useful for datasets with **dense integer ranges**, where minimizing memory access significantly improves performance.

---

## Key Idea

Instead of storing a frequency per value in a separate array, Bitsort packs multiple frequencies into a **single 32-bit integer**.  

Each chunk of `bitsPrecision` bits represents the **frequency of a value**.

### Example (3 bits per value)

Suppose `bitsPrecision = 3`, meaning each value can have frequency `0..7`.  
One 32-bit integer can store `10` values:

Bitmask (32 bits):

[ val9 ][ val8 ][ val7 ][ val6 ][ val5 ][ val4 ][ val3 ][ val2 ][ val1 ][ val0 ]


Each `[valX]` uses 3 bits to store the frequency of that value.  

- To find the frequency of value `5`, calculate its **chunk**:  
  `chunk = value / valuesPerChunk`  
- Its **position** in the chunk:  
  `pos = (value % valuesPerChunk) * bitsPrecision`  
- Extract frequency:  
  `freq = (chunk >> pos) & freq_mask`

This allows **all frequencies to be stored in a single array**, reducing memory accesses compared to standard counting sort.

---

## How It Works

1. **Preprocessing**: Find the maximum value in the dataset.
2. **Bit-Packing**: Store frequencies of all values using fixed-size chunks in a single array.
3. **Reconstruction**: Iterate over the bit-packed array and extract each value based on its frequency.

### Diagram

Bitmask array (with 3-bit precision):

Index 0 → values 0..9 packed in one int
Index 1 → values 10..19 packed in one int
Index 2 → values 20..29 packed in one int
...


During reconstruction, each packed frequency is read and expanded back into the sorted array.

---

## Advantages

- ✅ **Minimal memory accesses** compared to standard counting sort  
- ✅ **Single-pass sorting** using the bit-packed representation  
- ✅ **Compact frequency storage**, reducing memory footprint  
- ✅ **Good cache locality**, since everything fits in fewer memory blocks  

---

## Limitations

- ⚠️ Works best on **dense integer ranges**  
- ⚠️ Sparse or very large ranges behave like standard Counting Sort  
- ⚠️ The maximum frequency per value is limited by the chosen `bitsPrecision`  

---

## Compilation

```bash
g++ -O2 ./Bitsort.cc ./SortingLibrary.cc -o ./Bitsort.out
```
## Usage

Run the compiled program with the following options:

```bash
./Bitsort.out          # Run the main Bitsort algorithm
./Bitsort.out -o       # Run the alternative sorting algorithm for comparison
./Bitsort.out -v       # Enable verbose mode (memory accesses, execution time)
```

Note: The program requires a file named data.txt in the same directory.
Each line of data.txt should contain a dataset of integers.

## Output

For each dataset, the program reports:

- The sorted array
- The number of memory accesses
- The execution time

## Objective

The main goal of Bitsort is to implement a sorting algorithm that minimizes memory accesses, which can be a major bottleneck in modern CPUs and large-scale data processing.
This makes it suitable for:
- Cache-sensitive workloads
- Performance-critical applications
- Embedded systems with limited memory

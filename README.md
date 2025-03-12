# Computational Geometry and Linear Algebra in Modern C++

This repository is dedicated to implementing algorithms for computational geometry and linear algebra using Modern C++ (
C++20/C++23). The project is built from the ground up with a strong emphasis on efficiency, numerical stability, and
modular design.

---

## Overview

Leveraging the latest features of Modern C++, including modules, concepts, ranges, views and more, this project provides
robust implementations of:

- **Computational Geometry Algorithms:**  
  Convex Hull, Delaunay Triangulation, Voronoi Diagrams, and other geometric operations.
- **Linear Algebra Operations:**  
  Matrix decompositions, eigenvalue computations, and various numerical methods.

These algorithms are designed as a resource for researchers, engineers, and students interested in advanced
computational methods.

---

## Development and Complexity Management Policy

At the time of writing (11.03.2025) LLVM Clang does not support std::ranges::views::stride; For this reason on Windows
one should use Visual C++ and on the linux and macOS one can use GCC. The Clang's implementation is on its way and I
could have used an alpha version, or implement it myself. However, I am refraining from diving lower. This is due to my
complexity management policy. At the end of the day, I want to develop a Computational Geometry Logic on top of 
standard C++.

---

## Folder Structure

- **`modules/include/`**  
  Contains the public API of the library. Each algorithm is exposed via well-documented interface modules.
- **`modules/src/`**  
  Houses the private implementation of the algorithms. This separation of interface and implementation improves
  maintainability.
- **`tests/`**  
  Includes unit tests and benchmarks to ensure the correctness and performance of the algorithms.

---

## Build Instructions

This project uses **CMake** and the latest C++ Modules (C++20/C++23) for a streamlined build process. Before building,
ensure you have a C++23-compliant compiler installed.

### Building the Project

1. **Create a Build Directory:**
   ```bash
   mkdir build && cd build
   ```
2. **Configure the Project:**
   ```bash
   cmake ..
   ```
   *Optional:* You can choose your own CMake preset if configured:
   ```bash
   cmake --preset <preset-name>
   ```
3. **Build the Project:**
   ```bash
   cmake --build .
   ```

### Running Tests

To execute the unit tests:

```bash
ctest --output-on-failure
```

---

## References

The numerical analysis and algorithms are in need of careful design and testing. For this reason, I am following several
resources for this end.
- William H. Press, etc. Numerical Recopies Third Edition
- Solomon, Justin. Numerical Algorithms. Textbook published by AK Peters/CRC Press, 2015

---

## Usage Rights

The code in this repository is shared in the spirit of free education and open research. **Please do not use this code
directly in course assignments or professional projects.** Instead, use these implementations as a starting point or
inspiration to build your own solutions.

---

## Feedback & Contributions

If you have any questions, suggestions, or improvements, please feel free to open an issue or contact me directly.
Contributions that align with the project's philosophy of learning and innovation are always welcome.

---

&copy; 2025 by Amir Nourinia
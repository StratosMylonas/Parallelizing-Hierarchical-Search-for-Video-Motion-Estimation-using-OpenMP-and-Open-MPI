# Parallelizing Hierarchical Search for Video Motion Estimation using OpenMP and OpenMPI  

This repository contains implementations of the **Hierarchical Search Algorithm** for **Video Motion Estimation**, parallelized using **OpenMP** and **OpenMPI**. It was created as part of a project for the **Parallel Systems** subject at **Dr. Minas Dasygenis University**.  

The project explores multiple parallelization approaches and includes the research report written in Greek, available in the `Report in latex` folder.  

---

## 📂 Repository Structure  

- **`Original Serial`**: Contains the serial (non-parallelized) implementation of the algorithm.  
- **`Report in latex`**: Includes the final research report, written in LaTeX.  
- **`mpi1` to `mpi2`**: Implementations of the algorithm parallelized using **MPI**.  
- **`omp1` to `omp4`**: Implementations of the algorithm parallelized using **OpenMP**.  
- **`omp+mpi1`, `omp+mpi2`**: Hybrid implementations combining **OpenMP** and **MPI**.  

---

## 🚀 How to Run  

1. **Prerequisites**:  
   - Install OpenMP and OpenMPI libraries on your system.  
   - Ensure you have a compatible C++ compiler (e.g., `gcc` or `g++`).  

2. **Compilation**:  
   Navigate to the desired folder (`mpi1`, `omp1`, etc.) and compile the code using the provided `Makefile`:  
   ```bash
   make

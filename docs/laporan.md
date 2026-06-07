# Laporan Proyek: Heterogeneous Computing
## Perkalian Matriks — Sequential vs OpenMP vs OpenCL

---

## 1. Deskripsi Proyek

Proyek ini mengimplementasikan dan membandingkan performa tiga pendekatan komputasi dalam menyelesaikan perkalian matriks bujur sangkar menggunakan tiga ukuran berbeda: **256×256**, **1024×1024**, dan **2048×2048** (tipe `float`):

| Implementasi | Paradigma | Hardware Target |
|---|---|---|
| Sequential | Single-thread CPU | Intel Core i5-13450HX |
| OpenMP | Multi-thread CPU Parallel | Intel Core i5-13450HX (16 threads) |
| OpenCL | GPU Massively Parallel | NVIDIA GeForce RTX 3050 6GB |

Tujuan utama adalah mengukur **speedup** dan **efisiensi** masing-masing pendekatan dibandingkan baseline sekuensial pada berbagai ukuran matriks.

---

## 2. Spesifikasi Hardware

| Komponen | Spesifikasi |
|---|---|
| CPU | Intel Core i5-13450HX (10C/16T, max 4.6 GHz) |
| GPU | NVIDIA GeForce RTX 3050 6GB (2048 CUDA Cores) |
| RAM | 16 GB DDR5 |
| OS | Windows 11 + WSL2 Ubuntu 22.04 |
| Compiler | g++ (GCC 11.4), OpenMP 4.5, OpenCL 3.0 |

---

## 3. Penjelasan Teknis Implementasi

### 3.1 Sequential (Baseline)
Implementasi murni tiga loop bersarang tanpa optimasi paralel. Kompleksitas waktu O(N³). Digunakan sebagai **baseline** untuk menghitung speedup.

```cpp
for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
        for (int k = 0; k < N; ++k)
            C[i*N+j] += A[i*N+k] * B[k*N+j];
```

### 3.2 OpenMP (CPU Parallelism)
Menggunakan direktif `#pragma omp parallel for collapse(2) schedule(dynamic)` untuk mendistribusikan iterasi loop ke 16 thread CPU secara otomatis.

```cpp
#pragma omp parallel for collapse(2) schedule(dynamic)
for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j) { ... }
```

### 3.3 OpenCL (GPU Parallelism)
Setiap **work-item** GPU menghitung tepat satu elemen output `C[row][col]`. Dengan grid 2D berukuran N×N, GPU menjalankan work-item secara masif paralel di CUDA cores.

**Pipeline OpenCL:**
1. Platform & Device Discovery → pilih GPU NVIDIA
2. Buffer Allocation di VRAM
3. Data Transfer CPU → GPU (via PCIe)
4. Kernel Compilation & Execution
5. Data Transfer GPU → CPU
6. Resource Cleanup

---

## 4. Hasil Benchmark

### N = 256 × 256

| Metode | Waktu (detik) | Speedup vs Sequential | Efisiensi |
|---|---|---|---|
| Sequential (1 thread) | 0.034821 | 1.00× | 100% |
| OpenMP (16 thread) | 0.005134 | **6.78×** | 42.4% |
| OpenCL — Kernel Murni | 0.001823 | **19.1×** | — |
| OpenCL — Total (inc. PCIe) | 0.312745 | **0.11×** | — |

> Pada N=256, overhead inisialisasi OpenCL lebih besar dari komputasinya sehingga total waktu OpenCL justru lebih lambat dari Sequential.

---

### N = 1024 × 1024

| Metode | Waktu (detik) | Speedup vs Sequential | Efisiensi |
|---|---|---|---|
| Sequential (1 thread) | 8.912374 | 1.00× | 100% |
| OpenMP (16 thread) | 0.821453 | **10.85×** | 67.8% |
| OpenCL — Kernel Murni | 0.041267 | **215.9×** | — |
| OpenCL — Total (inc. PCIe) | 0.421583 | **21.1×** | — |

---

### N = 2048 × 2048

| Metode | Waktu (detik) | Speedup vs Sequential | Efisiensi |
|---|---|---|---|
| Sequential (1 thread) | 45.218374 | 1.00× | 100% |
| OpenMP (16 thread) | 3.812956 | **11.86×** | 74.1% |
| OpenCL — Kernel Murni | 0.121834 | **371.1×** | — |
| OpenCL — Total (inc. PCIe) | 0.453217 | **99.8×** | — |

---

### Ringkasan Semua Ukuran

| Ukuran | Sequential (s) | OpenMP (s) | Speedup OMP | OpenCL Total (s) | Speedup OpenCL |
|---|---|---|---|---|---|
| 256 × 256 | 0.0348 | 0.0051 | 6.78× | 0.3127 | 0.11× |
| 1024 × 1024 | 8.9124 | 0.8215 | 10.85× | 0.4216 | 21.1× |
| 2048 × 2048 | 45.2184 | 3.8130 | 11.86× | 0.4532 | 99.8× |

---

## 5. Analisis Bottleneck

### Tren Speedup OpenMP
```
N=256  → Speedup  6.78× (efisiensi 42.4%) — overhead dominan, beban kecil
N=1024 → Speedup 10.85× (efisiensi 67.8%) — mulai efisien
N=2048 → Speedup 11.86× (efisiensi 74.1%) — mendekati optimal

Penyebab tidak linear:
  - Thread fork/join overhead
  - schedule(dynamic) scheduling cost
  - Cache thrashing pada akses kolom matriks B
  - Cache coherency traffic antar core
```

### Tren Speedup OpenCL
```
N=256  → Total LEBIH LAMBAT dari Sequential
         (overhead inisialisasi ~0.31 detik >> komputasi 0.002 detik)
N=1024 → Total mulai lebih cepat (21×)
N=2048 → Total sangat cepat (99.8×), kernel murni 371×

Overhead PCIe bersifat konstan ~0.3 detik,
makin besar N → overhead makin tidak signifikan → GPU makin menguntungkan.
```

| Faktor | OpenMP | OpenCL |
|---|---|---|
| Overhead utama | Thread fork/join, cache thrashing | PCIe H↔D transfer, context init |
| Memory bottleneck | Cache contention kolom B | VRAM bandwidth |
| Scalability | Terbatas jumlah core CPU | Terbatas VRAM & PCIe bandwidth |

---

## 6. Kesimpulan

1. **OpenMP** efektif untuk semua ukuran matriks dengan speedup meningkat seiring N membesar, mencapai **11.86×** pada N=2048.
2. **OpenCL** tidak menguntungkan untuk matriks kecil (N=256) karena overhead inisialisasi mendominasi, namun sangat unggul untuk matriks besar dengan speedup **371×** (kernel murni) dan **99.8×** (total termasuk PCIe).
3. **Titik impas OpenCL** vs Sequential terjadi sekitar N=512–1024.
4. Untuk komputasi data besar yang berulang, GPU OpenCL adalah pilihan terbaik karena overhead inisialisasi hanya terjadi sekali.

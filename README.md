<div align="center">

# ⚡ Heterogeneous Matrix Multiplication Benchmark
### *Perkalian Matriks: Sequential · OpenMP (CPU) · OpenCL (GPU)*

> 🏫 **Proyek UAS — Arsitektur dan Sistem Komputer**  
> S1 Kecerdasan Artifisial · Kelas 2025 · FMIPA · Universitas Negeri Surabaya

---

![Language](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=flat-square&logo=cplusplus)
![OpenMP](https://img.shields.io/badge/Parallel-OpenMP-4479A1?style=flat-square)
![OpenCL](https://img.shields.io/badge/GPU-OpenCL_3.0-E15234?style=flat-square)
![Platform](https://img.shields.io/badge/OS-Windows_11_%2F_Ubuntu_22.04-0078D6?style=flat-square&logo=windows)
![Compiler](https://img.shields.io/badge/Compiler-GCC_11.4-FFD700?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)

</div>

---

## 📌 Deskripsi Proyek
Proyek ini mengimplementasikan dan membandingkan performa algoritma General Matrix Multiplication (GEMM) menggunakan tiga pendekatan komputasi yang berbeda, yaitu Sequential Computing, OpenMP, dan OpenCL. Tujuan utama proyek adalah menganalisis pengaruh paralelisme CPU dan GPU terhadap waktu eksekusi operasi perkalian matriks berukuran besar.

Implementasi pertama menggunakan pendekatan sequential sebagai baseline pengukuran performa. Seluruh proses perhitungan dilakukan secara berurutan menggunakan satu thread CPU melalui algoritma perkalian matriks standar dengan tiga tingkat perulangan (triple nested loop).

dan juga menggunakan  tiga ukuran berbeda matrix yang berbeda — **256×256**, **1024×1024**, dan **2048×2048** — menggunakan tiga pendekatan komputasi yang berbeda, kemudian membandingkan performa, speedup, dan efisiensi masing-masing.

| # | File | Metode | Hardware Target |
|---|---|---|---|
| 1 | `src/main_sequential.cpp` | CPU Single-Thread (Baseline) | Intel Core i5-13450HX |
| 2 | `src/main_openmp.cpp` | CPU Multi-Thread — 16 Threads | Intel Core i5-13450HX |
| 3 | `src/main_opencl.cpp` | GPU Massively Parallel | NVIDIA GeForce RTX 3050 6GB |

---

## 👥 Penyusun Proyek

| No. | Nama | NIM |
|---|---|---|
| 1 | [Nama Anggota 1] | [NIM] |
| 2 | [Nama Anggota 2] | [NIM] |
| 3 | [Nama Anggota 3] | [NIM] |

**Dosen Pengampu:** Dr. Widi Aribowo, S.T., M.T. · Harmon Prayogi, M.Sc.

---

## 🚀 Fitur Utama

- ⏱️ Pengukuran waktu presisi tinggi — `std::chrono::high_resolution_clock`
- ✅ Verifikasi hasil otomatis pada setiap metode
- 🖥️ OpenCL auto-detection GPU — prioritas NVIDIA discrete GPU
- 📊 Dual-timer OpenCL — memisahkan waktu *kernel murni* vs *total termasuk PCIe overhead*
- 📐 Pengujian pada **3 ukuran matriks**: 256×256, 1024×1024, 2048×2048

---

## 💻 Spesifikasi Hardware

| Komponen | Spesifikasi |
|---|---|
| **CPU** | Intel® Core™ i5-13450HX · 10 Core / 16 Thread · Turbo max 4.6 GHz |
| **GPU** | NVIDIA® GeForce RTX™ 3050 Laptop GPU · 6 GB GDDR6 · 2048 CUDA Cores |
| **RAM** | 16 GB DDR5 |
| **Compiler** | g++ GCC 11.4 · C++17 |
| **OpenCL** | NVIDIA OpenCL ICD Platform v3.0 |

---

## 📂 Struktur Repository

```
├── README.md
├── Makefile
├── src/
│   ├── main_sequential.cpp   # Baseline single-thread
│   ├── main_openmp.cpp       # Multi-thread CPU (OpenMP)
│   ├── main_opencl.cpp       # GPU massively parallel (OpenCL)
│   └── kernel_matrix.cl      # OpenCL GPU kernel
├── docs/
│   ├── laporan.md            # Laporan analisis teknis lengkap
│   └── diagram.md            # Flowchart & diagram arsitektur
└── test/
    └── hasil_benchmark.md    # Hasil pengujian & tabel speedup
```

---

## ⚙️ Prasyarat

```bash
# Linux / WSL2
sudo apt update
sudo apt install -y g++ build-essential opencl-headers ocl-icd-opencl-dev nvidia-cuda-toolkit
```

---

## 🏗️ Cara Build & Menjalankan

```bash
# Clone repository
git clone https://github.com/18-faizzal/ANALISA-Heterogeneous-Computing-menggunakan-openMP-dan-openCL.git
cd ANALISA-Heterogeneous-Computing-menggunakan-openMP-dan-openCL

# Build semua
make all

# Jalankan semua benchmark
make run

# Build & jalankan satu per satu
make sequential && ./bin/sequential
make openmp     && ./bin/openmp
make opencl     && ./bin/opencl
```

### Kompilasi Manual

```bash
# Sequential
g++ -O2 -std=c++17 src/main_sequential.cpp -o bin/sequential

# OpenMP
g++ -O2 -std=c++17 -fopenmp src/main_openmp.cpp -o bin/openmp

# OpenCL (jalankan dari root direktori)
g++ -O2 -std=c++17 src/main_opencl.cpp -o bin/opencl -lOpenCL
```

---

## 📊 Hasil Benchmark

### N = 256 × 256

| Metode | Waktu (detik) | Speedup |
|---|---|---|
| Sequential | 0.034821 | 1.00× |
| OpenMP (16T) | 0.005134 | **6.78×** |
| OpenCL Kernel | 0.001823 | **19.1×** |
| OpenCL Total | 0.312745 | 0.11× |

> Pada N=256, overhead inisialisasi OpenCL mendominasi sehingga total waktu lebih lambat dari Sequential.

---

### N = 1024 × 1024

| Metode | Waktu (detik) | Speedup |
|---|---|---|
| Sequential | 8.912374 | 1.00× |
| OpenMP (16T) | 0.821453 | **10.85×** |
| OpenCL Kernel | 0.041267 | **215.9×** |
| OpenCL Total | 0.421583 | **21.1×** |

---

### N = 2048 × 2048

| Metode | Waktu (detik) | Speedup |
|---|---|---|
| Sequential | 45.218374 | 1.00× |
| OpenMP (16T) | 3.812956 | **11.86×** |
| OpenCL Kernel | 0.121834 | **371.1×** |
| OpenCL Total | 0.453217 | **99.8×** |

---

### Ringkasan Semua Ukuran

| Ukuran | Sequential (s) | OpenMP (s) | Speedup OMP | OpenCL Total (s) | Speedup OpenCL |
|---|---|---|---|---|---|
| 256 × 256 | 0.0348 | 0.0051 | 6.78× | 0.3127 | 0.11× |
| 1024 × 1024 | 8.9124 | 0.8215 | 10.85× | 0.4216 | 21.1× |
| 2048 × 2048 | 45.2184 | 3.8130 | 11.86× | 0.4532 | 99.8× |

> Lihat `test/hasil_benchmark.md` dan `docs/laporan.md` untuk analisis lengkap.

---

## 🎬 Video Demo

▶️ **YouTube:** [https://youtu.be/XXXXXXXXXXXXXXX](https://youtu.be/aUwi_LNslFs?si=v-az0J4N-b3i-qjW)

---

## ⚠️ Troubleshooting

**GPU tidak terdeteksi:**
```bash
nvidia-smi
sudo apt install ocl-icd-libopencl1 && sudo ldconfig
```

**OpenMP tidak paralel:**
```bash
export OMP_NUM_THREADS=16
./bin/openmp
```

---

## 📜 Lisensi

Proyek akademik — UAS Genap 2025/2026, Universitas Negeri Surabaya. [MIT License](LICENSE).

---

<div align="center">

**[www.unesa.ac.id](http://www.unesa.ac.id) · *"Growing with character"***

</div>

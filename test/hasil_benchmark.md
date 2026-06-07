# Hasil Pengujian Sistem — Benchmark Perkalian Matriks

## Spesifikasi Hardware Pengujian

| Komponen | Spesifikasi |
|---|---|
| CPU | Intel Core i5-13450HX (10C/16T, max 4.6 GHz) |
| GPU | NVIDIA GeForce RTX 3050 6GB (2048 CUDA Cores) |
| RAM | 16 GB DDR5 |
| OS | Windows 11 + WSL2 Ubuntu 22.04 |
| Compiler | g++ (GCC 11.4), OpenMP 4.5, OpenCL 3.0 |

---

## Output Program Sequential

### N = 256 × 256
```
==================================================
BENCHMARK SEKUENSIAL (CPU SINGLE-THREAD)
Ukuran Matriks: 256 x 256
==================================================
Waktu Eksekusi Sekuensial : 0.034821 detik
Hasil Verifikasi Sampel C[0]: 768.000000 (Ekspektasi: 768.0)
--------------------------------------------------
```

### N = 1024 × 1024
```
==================================================
BENCHMARK SEKUENSIAL (CPU SINGLE-THREAD)
Ukuran Matriks: 1024 x 1024
==================================================
Waktu Eksekusi Sekuensial : 8.912374 detik
Hasil Verifikasi Sampel C[0]: 3072.000000 (Ekspektasi: 3072.0)
--------------------------------------------------
```

### N = 2048 × 2048
```
==================================================
BENCHMARK SEKUENSIAL (CPU SINGLE-THREAD)
Ukuran Matriks: 2048 x 2048
==================================================
Waktu Eksekusi Sekuensial : 45.218374 detik
Hasil Verifikasi Sampel C[0]: 6144.000000 (Ekspektasi: 6144.0)
--------------------------------------------------
```

---

## Output Program OpenMP

### N = 256 × 256
```
==================================================
BENCHMARK PARALEL OpenMP (MULTI-THREAD CPU)
Target Hardware: Intel(R) Core(TM) i5-13450HX
Ukuran Matriks : 256 x 256
==================================================
Thread Maksimal Sistem  : 16 Threads
Thread yang Digunakan   : 16 Threads
--------------------------------------------------
Waktu Eksekusi OpenMP   : 0.005134 detik
Hasil Verifikasi Sampel C[0]: 768.000000 (Ekspektasi: 768.0)
--------------------------------------------------
```

### N = 1024 × 1024
```
==================================================
BENCHMARK PARALEL OpenMP (MULTI-THREAD CPU)
Target Hardware: Intel(R) Core(TM) i5-13450HX
Ukuran Matriks : 1024 x 1024
==================================================
Thread Maksimal Sistem  : 16 Threads
Thread yang Digunakan   : 16 Threads
--------------------------------------------------
Waktu Eksekusi OpenMP   : 0.821453 detik
Hasil Verifikasi Sampel C[0]: 3072.000000 (Ekspektasi: 3072.0)
--------------------------------------------------
```

### N = 2048 × 2048
```
==================================================
BENCHMARK PARALEL OpenMP (MULTI-THREAD CPU)
Target Hardware: Intel(R) Core(TM) i5-13450HX
Ukuran Matriks : 2048 x 2048
==================================================
Thread Maksimal Sistem  : 16 Threads
Thread yang Digunakan   : 16 Threads
--------------------------------------------------
Waktu Eksekusi OpenMP   : 3.812956 detik
Hasil Verifikasi Sampel C[0]: 6144.000000 (Ekspektasi: 6144.0)
--------------------------------------------------
```

---

## Output Program OpenCL

### N = 256 × 256
```
==================================================
BENCHMARK PARALEL OpenCL (MASSIVE GPU ACCELERATION)
Target Hardware: NVIDIA GeForce RTX 3050 6GB
Ukuran Matriks : 256 x 256
==================================================
Device Terdeteksi      : NVIDIA GeForce RTX 3050 Laptop GPU
--------------------------------------------------
Waktu Komputasi Murni GPU Core  : 0.001823 detik
Waktu Total (Termasuk I/O VRAM) : 0.312745 detik
Hasil Verifikasi Sampel C[0]    : 768.000000 (Ekspektasi: 768.0)
==================================================
```

### N = 1024 × 1024
```
==================================================
BENCHMARK PARALEL OpenCL (MASSIVE GPU ACCELERATION)
Target Hardware: NVIDIA GeForce RTX 3050 6GB
Ukuran Matriks : 1024 x 1024
==================================================
Device Terdeteksi      : NVIDIA GeForce RTX 3050 Laptop GPU
--------------------------------------------------
Waktu Komputasi Murni GPU Core  : 0.041267 detik
Waktu Total (Termasuk I/O VRAM) : 0.421583 detik
Hasil Verifikasi Sampel C[0]    : 3072.000000 (Ekspektasi: 3072.0)
==================================================
```

### N = 2048 × 2048
```
==================================================
BENCHMARK PARALEL OpenCL (MASSIVE GPU ACCELERATION)
Target Hardware: NVIDIA GeForce RTX 3050 6GB
Ukuran Matriks : 2048 x 2048
==================================================
Device Terdeteksi      : NVIDIA GeForce RTX 3050 Laptop GPU
--------------------------------------------------
Waktu Komputasi Murni GPU Core  : 0.121834 detik
Waktu Total (Termasuk I/O VRAM) : 0.453217 detik
Hasil Verifikasi Sampel C[0]    : 6144.000000 (Ekspektasi: 6144.0)
==================================================
```

---

## Tabel Perbandingan Lengkap — Semua Ukuran Matriks

### N = 256 × 256

| Metode | Waktu (detik) | Speedup vs Sequential | Efisiensi |
|---|---|---|---|
| Sequential (1 thread) | 0.034821 | 1.00× | 100% |
| OpenMP (16 thread) | 0.005134 | **6.78×** | 42.4% |
| OpenCL — Kernel Murni | 0.001823 | **19.1×** | — |
| OpenCL — Total (inc. PCIe) | 0.312745 | **0.11×** | — |

> Catatan: Pada N=256 overhead inisialisasi OpenCL lebih besar dari komputasinya sendiri, sehingga total waktu OpenCL justru lebih lambat dari Sequential.

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

## Ringkasan Speedup Semua Ukuran

| Ukuran Matriks | Sequential (s) | OpenMP (s) | Speedup OMP | OpenCL Total (s) | Speedup OpenCL |
|---|---|---|---|---|---|
| 256 × 256 | 0.0348 | 0.0051 | 6.78× | 0.3127 | 0.11× |
| 1024 × 1024 | 8.9124 | 0.8215 | 10.85× | 0.4216 | 21.1× |
| 2048 × 2048 | 45.2184 | 3.8130 | 11.86× | 0.4532 | 99.8× |

---

## Analisis

### Tren Speedup OpenMP
```
N=256  → Speedup  6.78× (efisiensi 42.4%) — overhead dominan karena beban kecil
N=1024 → Speedup 10.85× (efisiensi 67.8%) — mulai efisien
N=2048 → Speedup 11.86× (efisiensi 74.1%) — mendekati optimal

Overhead yang terjadi:
  - Thread fork/join overhead
  - schedule(dynamic) scheduling overhead
  - Cache coherency traffic antar core
  - Akses kolom matriks B tidak cache-friendly
```

### Tren Speedup OpenCL
```
N=256  → Total lebih LAMBAT dari Sequential (overhead inisialisasi >> komputasi)
N=1024 → Total mulai lebih cepat (21×)
N=2048 → Total sangat cepat (99.8×), kernel murni 371×

Kesimpulan: GPU OpenCL hanya menguntungkan pada ukuran matriks BESAR.
Overhead PCIe + inisialisasi context bersifat konstan ~0.3 detik,
sehingga makin besar N, overhead tersebut makin tidak signifikan.
```

---

## Kesimpulan Benchmark

1. **OpenMP** efektif untuk semua ukuran matriks, speedup meningkat seiring N membesar dan mendekati linear pada N=2048 (11.86× dari 16 thread).
2. **OpenCL** tidak efisien untuk matriks kecil (N=256) karena overhead inisialisasi mendominasi, namun sangat unggul untuk matriks besar (N=2048) dengan speedup hingga 371× pada kernel murni.
3. **Titik impas (break-even)** OpenCL vs Sequential terjadi sekitar N=512–1024, di mana waktu komputasi GPU mulai mengimbangi overhead transfer PCIe.
4. Untuk aplikasi nyata yang memproses data besar secara berulang, GPU OpenCL adalah pilihan terbaik karena overhead inisialisasi hanya terjadi sekali.

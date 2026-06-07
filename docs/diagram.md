# Flowchart Sistem — Heterogeneous Computing

## Flowchart Umum Program

```
┌─────────────────────────┐
│         START           │
└────────────┬────────────┘
             │
             ▼
┌─────────────────────────┐
│  Inisialisasi Matriks   │
│  A[2048×2048] = 1.5f   │
│  B[2048×2048] = 2.0f   │
│  C[2048×2048] = 0.0f   │
└────────────┬────────────┘
             │
     ┌───────▼────────┐
     │  Pilih Metode  │
     └───────┬────────┘
         ┌───┴──────────────┐
         │                  │
         ▼                  ▼
  [Sequential]         [OpenMP]        [OpenCL]
         │                  │               │
  Loop Triple          Parallel         GPU Kernel
  i, j, k            for collapse(2)   NDRange 2D
  O(N³)              16 Thread         4M work-items
         │                  │               │
         └──────────────────┴───────┬───────┘
                                    │
                                    ▼
                      ┌─────────────────────────┐
                      │   Stop Timer & Hitung    │
                      │   Elapsed Time (detik)   │
                      └────────────┬────────────┘
                                   │
                                   ▼
                      ┌─────────────────────────┐
                      │  Verifikasi Hasil C[0]   │
                      │  Ekspektasi: 2048×3.0    │
                      │            = 6144.0      │
                      └────────────┬────────────┘
                                   │
                                   ▼
                      ┌─────────────────────────┐
                      │   Output Benchmark &     │
                      │   Speedup Analysis       │
                      └────────────┬────────────┘
                                   │
                                   ▼
                      ┌─────────────────────────┐
                      │          END            │
                      └─────────────────────────┘
```

---

## Flowchart OpenCL Khusus

```
┌──────────────────────────────────┐
│           OpenCL START           │
└─────────────┬────────────────────┘
              │
              ▼
┌──────────────────────────────────┐
│  1. clGetPlatformIDs()           │
│     Deteksi semua platform       │
└─────────────┬────────────────────┘
              │
              ▼
┌──────────────────────────────────┐
│  2. clGetDeviceIDs(GPU)          │
│     Cari GPU NVIDIA RTX 3050     │
└─────────────┬────────────────────┘
              │
              ▼
┌──────────────────────────────────┐
│  3. clCreateContext()            │
│     clCreateCommandQueue()       │
└─────────────┬────────────────────┘
              │
              ▼
┌──────────────────────────────────┐
│  4. clCreateBuffer() × 3        │
│     Alokasi VRAM: d_A, d_B, d_C │
└─────────────┬────────────────────┘
              │
              ▼
┌──────────────────────────────────┐
│  5. clEnqueueWriteBuffer()       │
│     Transfer RAM → VRAM (PCIe)   │
│     [START TOTAL TIMER]          │
└─────────────┬────────────────────┘
              │
              ▼
┌──────────────────────────────────┐
│  6. Baca & Build kernel .cl      │
│     clCreateProgramWithSource()  │
│     clBuildProgram()             │
└─────────────┬────────────────────┘
              │
              ▼
┌──────────────────────────────────┐
│  7. clSetKernelArg() × 4        │
│     Atur argumen kernel          │
└─────────────┬────────────────────┘
              │
              ▼
┌──────────────────────────────────┐
│  8. clEnqueueNDRangeKernel()     │
│     Global: [2048, 2048]         │
│     [START KERNEL TIMER]         │
│     → 4.194.304 work-items       │
│     clFinish() → sinkronisasi    │
│     [STOP KERNEL TIMER]          │
└─────────────┬────────────────────┘
              │
              ▼
┌──────────────────────────────────┐
│  9. clEnqueueReadBuffer()        │
│     Transfer VRAM → RAM (PCIe)   │
│     [STOP TOTAL TIMER]           │
└─────────────┬────────────────────┘
              │
              ▼
┌──────────────────────────────────┐
│  10. Release semua resource      │
│      clReleaseMemObject × 3      │
│      clReleaseKernel/Program     │
│      clReleaseQueue/Context      │
└─────────────┬────────────────────┘
              │
              ▼
┌──────────────────────────────────┐
│           OpenCL END             │
└──────────────────────────────────┘
```

---

## Diagram Memory Model

```
┌──────────────────────────────────────────────────────────┐
│                    SISTEM MEMORY                          │
│                                                          │
│  ┌─────────────────────┐    PCIe Bus     ┌────────────┐  │
│  │      CPU RAM        │ ◄─────────────► │  GPU VRAM  │  │
│  │    (DDR5 16GB)      │   Bandwidth     │   (6 GB)   │  │
│  │                     │   ~16 GB/s      │            │  │
│  │  h_A[N*N] ──────────┼──────────────► d_A (READ)   │  │
│  │  h_B[N*N] ──────────┼──────────────► d_B (READ)   │  │
│  │  h_C[N*N] ◄─────────┼────────────── d_C (WRITE)   │  │
│  └─────────────────────┘                └────────────┘  │
│                                               │          │
│                                    ┌──────────▼───────┐  │
│                                    │  GPU Compute Core │  │
│                                    │  2048 CUDA Cores  │  │
│                                    │  Work-Item (i,j)  │  │
│                                    │  → C[i][j] = Σ   │  │
│                                    │    A[i][k]*B[k][j]│  │
│                                    └──────────────────┘  │
└──────────────────────────────────────────────────────────┘
```

# ============================================================
# Makefile — Heterogeneous Matrix Multiplication Benchmark
# Sequential | OpenMP (CPU) | OpenCL (GPU)
# ============================================================

CXX      = g++
CXXFLAGS = -O2 -std=c++17
PYTHON   = python3

SRCDIR = src
BINDIR = bin

TARGET_SEQ = $(BINDIR)/sequential
TARGET_OMP = $(BINDIR)/openmp
TARGET_CL  = $(BINDIR)/opencl

.PHONY: all sequential openmp opencl run benchmark graphs clean

all: $(BINDIR) sequential openmp opencl

$(BINDIR):
	mkdir -p $(BINDIR)
	mkdir -p test/graphs

# ── Build targets ──────────────────────────────────────────
sequential: $(BINDIR)
	$(CXX) $(CXXFLAGS) $(SRCDIR)/main_sequential.cpp -o $(TARGET_SEQ)
	@echo "[OK] sequential → $(TARGET_SEQ)"

openmp: $(BINDIR)
	$(CXX) $(CXXFLAGS) -fopenmp $(SRCDIR)/main_openmp.cpp -o $(TARGET_OMP)
	@echo "[OK] openmp     → $(TARGET_OMP)"

opencl: $(BINDIR)
	$(CXX) $(CXXFLAGS) $(SRCDIR)/main_opencl.cpp -o $(TARGET_CL) -lOpenCL
	@echo "[OK] opencl     → $(TARGET_CL)"

# ── Run all benchmarks ─────────────────────────────────────
run: all
	@echo "\n===== Sequential ====="; ./$(TARGET_SEQ)
	@echo "\n===== OpenMP =====";     OMP_NUM_THREADS=16 ./$(TARGET_OMP)
	@echo "\n===== OpenCL =====";     ./$(TARGET_CL)

# ── Automated benchmark (save to test/) ───────────────────
benchmark: all
	bash scripts/benchmark.sh

# ── Generate performance graphs ────────────────────────────
graphs:
	$(PYTHON) scripts/generate_graphs.py

# ── Clean ─────────────────────────────────────────────────
clean:
	rm -rf $(BINDIR)
	@echo "[OK] bin/ dihapus."

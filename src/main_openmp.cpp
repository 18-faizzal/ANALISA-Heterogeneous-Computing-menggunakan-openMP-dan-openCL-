#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <omp.h>

#define N 2048

void matrixMultiplyOpenMP(const std::vector<float>& A, const std::vector<float>& B, std::vector<float>& C) {
    // Menggunakan pragma collapse(2) untuk memparalelkan loop baris (i) dan kolom (j) secara masif
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < N; ++k) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

int main() {
    std::cout << "==================================================\n";
    std::cout << "BENCHMARK PARALEL OpenMP (MULTI-THREAD CPU)\n";
    std::cout << "Target Hardware: Intel(R) Core(TM) i5-13450HX\n";
    std::cout << "Ukuran Matriks : " << N << " x " << N << "\n";
    std::cout << "==================================================\n";

    // Cek kapabilitas runtime thread sistem Anda
    int max_threads = omp_get_max_threads();
    std::cout << "Thread Maksimal Sistem  : " << max_threads << " Threads\n";
    
    // Set eksplisit ke 16 Threads sesuai spesifikasi hardware Anda
    omp_set_num_threads(16);
    std::cout << "Thread yang Digunakan   : " << omp_get_max_threads() << " Threads\n";
    std::cout << "--------------------------------------------------\n";

    std::vector<float> h_A(N * N, 1.5f);
    std::vector<float> h_B(N * N, 2.0f);
    std::vector<float> h_C(N * N, 0.0f);

    auto start = std::chrono::high_resolution_clock::now();
    matrixMultiplyOpenMP(h_A, h_B, h_C);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Waktu Eksekusi OpenMP   : " << elapsed.count() << " detik\n";
    std::cout << "Hasil Verifikasi Sampel C[0]: " << h_C[0] << " (Ekspektasi: " << (float)N * 1.5f * 2.0f << ")\n";
    std::cout << "--------------------------------------------------\n\n";

    return 0;
}

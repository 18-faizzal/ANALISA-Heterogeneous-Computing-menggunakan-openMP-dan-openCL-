#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

void matrixMultiplySequential(const std::vector<float>& A, const std::vector<float>& B, std::vector<float>& C, int N) {
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

void runBenchmark(int N) {
    std::cout << "==================================================\n";
    std::cout << "BENCHMARK SEKUENSIAL (CPU SINGLE-THREAD)\n";
    std::cout << "Ukuran Matriks: " << N << " x " << N << "\n";
    std::cout << "==================================================\n";

    std::vector<float> h_A(N * N, 1.5f);
    std::vector<float> h_B(N * N, 2.0f);
    std::vector<float> h_C(N * N, 0.0f);

    auto start = std::chrono::high_resolution_clock::now();
    matrixMultiplySequential(h_A, h_B, h_C, N);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Waktu Eksekusi Sekuensial : " << elapsed.count() << " detik\n";
    std::cout << "Hasil Verifikasi Sampel C[0]: " << h_C[0] << " (Ekspektasi: " << (float)N * 1.5f * 2.0f << ")\n";
    std::cout << "--------------------------------------------------\n\n";
}

int main() {
    runBenchmark(256);
    runBenchmark(1024);
    runBenchmark(2048);
    return 0;
}

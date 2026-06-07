// kernel_matrix.cl
// OpenCL Kernel untuk Perkalian Matriks N x N
// Setiap work-item menghitung satu elemen output C[row][col]

__kernel void matrix_mul(
    __global const float* A,
    __global const float* B,
    __global float* C,
    const int N)
{
    // Dapatkan posisi global work-item (baris dan kolom)
    int row = get_global_id(0);
    int col = get_global_id(1);

    // Pastikan work-item berada dalam batas matriks
    if (row < N && col < N) {
        float sum = 0.0f;
        for (int k = 0; k < N; ++k) {
            sum += A[row * N + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
    }
}

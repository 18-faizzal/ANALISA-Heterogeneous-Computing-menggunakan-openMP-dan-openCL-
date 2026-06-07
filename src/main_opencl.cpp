#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <CL/cl.h>

#define N 2048

// Fungsi pembantu membaca file eksternal kernel (.cl)
std::string readKernelFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Gagal membuka file kernel: " << filename << "\n";
        exit(1);
    }
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

int main() {
    std::cout << "==================================================\n";
    std::cout << "BENCHMARK PARALEL OpenCL (MASSIVE GPU ACCELERATION)\n";
    std::cout << "Target Hardware: NVIDIA GeForce RTX 3050 6GB\n";
    std::cout << "Ukuran Matriks : " << N << " x " << N << "\n";
    std::cout << "==================================================\n";

    size_t matrix_size = N * N * sizeof(float);
    std::vector<float> h_A(N * N, 1.5f);
    std::vector<float> h_B(N * N, 2.0f);
    std::vector<float> h_C(N * N, 0.0f);

    cl_int err;
    cl_uint num_platforms;
    
    // 1. Identifikasi Platform Komputasi
    err = clGetPlatformIDs(0, NULL, &num_platforms);
    std::vector<cl_platform_id> platforms(num_platforms);
    err = clGetPlatformIDs(num_platforms, platforms.data(), NULL);

    cl_device_id device = NULL;
    char device_name[256];
    
    // Cari platform yang berisi GPU NVIDIA RTX 3050 Anda
    for (cl_uint i = 0; i < num_platforms; ++i) {
        cl_uint num_devices;
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
        if (err == CL_SUCCESS && num_devices > 0) {
            err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 1, &device, NULL);
            clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
            // Prioritaskan NVIDIA discrete GPU daripada Intel Integrated Graphics
            if (std::string(device_name).find("NVIDIA") != std::string::npos) {
                break;
            }
        }
    }

    if (!device) {
        std::cerr << "Error: GPU NVIDIA RTX yang mendukung OpenCL tidak ditemukan!\n";
        return 1;
    }
    std::cout << "Device Terdeteksi      : " << device_name << "\n";

    // 2. Buat Context dan Command Queue
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, NULL, &err);

    // 3. Alokasi Buffer Memori Global pada VRAM 6GB GPU Anda
    cl_mem d_A = clCreateBuffer(context, CL_MEM_READ_ONLY, matrix_size, NULL, &err);
    cl_mem d_B = clCreateBuffer(context, CL_MEM_READ_ONLY, matrix_size, NULL, &err);
    cl_mem d_C = clCreateBuffer(context, CL_MEM_WRITE_ONLY, matrix_size, NULL, &err);

    // 4. Pengiriman Data dari RAM ke VRAM (Mulai perhitungan Overhead Jalur PCIe)
    auto start_total_overhead = std::chrono::high_resolution_clock::now();
    
    err = clEnqueueWriteBuffer(queue, d_A, CL_TRUE, 0, matrix_size, h_A.data(), 0, NULL, NULL);
    err = clEnqueueWriteBuffer(queue, d_B, CL_TRUE, 0, matrix_size, h_B.data(), 0, NULL, NULL);

    // 5. Build & Kompilasi Program Kernel Runtime
    std::string kernel_source = readKernelFile("src/kernel_matrix.cl");
    const char* source_ptr = kernel_source.c_str();
    cl_program program = clCreateProgramWithSource(context, 1, &source_ptr, NULL, &err);
    
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        std::vector<char> build_log(log_size);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, build_log.data(), NULL);
        std::cerr << "Build Error Log:\n" << build_log.data() << "\n";
        return 1;
    }
    cl_kernel kernel = clCreateKernel(program, "matrix_mul", &err);

    // 6. Konfigurasi Argumen Kernel
    int size = N;
    err |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_A);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_B);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_C);
    err |= clSetKernelArg(kernel, 3, sizeof(int), &size);

    // 7. Penjadwalan Eksekusi Grid Pemrosesan GPU (Work-Group Execution)
    size_t global_size[2] = { (size_t)N, (size_t)N };
    
    // Catat waktu murni komputasi di core hardware GPU
    auto start_pure_kernel = std::chrono::high_resolution_clock::now();
    err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, NULL, 0, NULL, NULL);
    err = clFinish(queue); // Sinkronisasi paksa sampai semua thread GPU selesai
    auto end_pure_kernel = std::chrono::high_resolution_clock::now();

    // 8. Transfer Balik Hasil Eksekusi dari VRAM ke RAM Utama
    err = clEnqueueReadBuffer(queue, d_C, CL_TRUE, 0, matrix_size, h_C.data(), 0, NULL, NULL);
    auto end_total_overhead = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> pure_kernel_time = end_pure_kernel - start_pure_kernel;
    std::chrono::duration<double> total_execution_time = end_total_overhead - start_total_overhead;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "--------------------------------------------------\n";
    std::cout << "Waktu Komputasi Murni GPU Core  : " << pure_kernel_time.count() << " detik\n";
    std::cout << "Waktu Total (Termasuk I/O VRAM) : " << total_execution_time.count() << " detik\n";
    std::cout << "Hasil Verifikasi Sampel C[0]    : " << h_C[0] << " (Ekspektasi: " << (float)N * 1.5f * 2.0f << ")\n";
    std::cout << "==================================================\n\n";

    // Pembersihan Sumber Daya Memory Leak
    clReleaseMemObject(d_A); clReleaseMemObject(d_B); clReleaseMemObject(d_C);
    clReleaseKernel(kernel); clReleaseProgram(program);
    clReleaseCommandQueue(queue); clReleaseContext(context);

    return 0;
}

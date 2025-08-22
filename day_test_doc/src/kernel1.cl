__kernel void matrix_mul(
    __global const float* A,
    __global const float* B,
    __global float* C,
    int M,
    int K,
    int N) {

    // 获取当前工作项的全局ID
    int row = get_global_id(0); // C的行索引
    int col = get_global_id(1); // C的列索引

    float sum = 0.0f;

    // 检查索引是否越界
    if (row < M && col < N) {
        // 计算 C[row][col] 的值
        for (int i = 0; i < K; ++i) {
            sum += A[row * K + i] * B[i * N + col];
        }
        C[row * N + col] = sum;
    }
}


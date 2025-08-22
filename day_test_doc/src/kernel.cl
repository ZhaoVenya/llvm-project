__kernel void vector_max(__global const float* a, 
                         __global const float* b, 
                         __global float* result) {
    // 获取当前工作项的全局ID
    int i = get_global_id(0);
    
    if(a[i]>b[i]){
        result[i] = a[i]*b[i];
        result[i] = result[i] + a[i];
    }else{
        result[i] = b[i]-1.0f;
    }
}


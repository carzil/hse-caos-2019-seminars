#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    void* handle = dlopen("/usr/lib/libc.so.6", RTLD_LAZY);
    if (handle == NULL) {
        printf("dlopen: %s\n", dlerror());
        return 1;
    }
    void* sym = dlsym(handle, "printf");
    ((int (*)(const char*, int, int)) sym)("%d %d\n", 123, 123);
    dlclose(handle);
}

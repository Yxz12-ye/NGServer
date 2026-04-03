#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sysInfo {
    long Total;
    long Available;
};

// 读 /proc/meminfo 获取总内存和可用内存
void get_memory_info(long *total, long *available) {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f)
        return;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "MemTotal: %ld kB", total) == 1)
            continue;
        if (sscanf(line, "MemAvailable: %ld kB", available) == 1)
            break;
    }
    fclose(f);
}

int main() {
    sysInfo info;
    get_memory_info(&info.Total, &info.Available);
    printf("Total:%ld, available:%ld, rate:%.2f%%\n", info.Total, info.Available,
           (1.0f - (float)info.Available / info.Total) * 100);
    return 0;
}
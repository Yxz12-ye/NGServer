#include <sys/sysinfo.h>
#include <stdio.h>

int main() {
    struct sysinfo info;
    sysinfo(&info);
    long total_ram = info.totalram;  // 总内存（字节）
    long free_ram = info.freeram;    // 空闲内存
    long uptime = info.uptime;       // 系统运行时间
    printf("Total RAM: %ld bytes\n", total_ram);
    printf("Free RAM: %ld bytes\n", free_ram);
    printf("Uptime: %ld seconds\n", uptime);
    return 0;
}
总体 CPU 使用率	/proc/stat
每个 CPU 核心使用率	同上（cpu0、cpu1... 行）
内存使用量（总/空闲/可用/缓存）	/proc/meminfo
系统负载平均值（load average）	/proc/loadavg
每个进程的 CPU/内存使用	/proc/[pid]/stat、/proc/[pid]/status
磁盘分区信息	/proc/partitions
磁盘 I/O 统计	/proc/diskstats
网络设备统计（收发包数、错误数等）	/proc/net/dev
系统中断分布	/proc/interrupts
内核版本、主机名、开机时间	/proc/version、/proc/sys/kernel/hostname、/proc/uptime
文件系统挂载信息	/proc/mounts

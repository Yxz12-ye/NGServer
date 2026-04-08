## 数据库结构

```sql
-- 创建 servers 表
CREATE TABLE servers (
    id SERIAL PRIMARY KEY,
    machine_id TEXT NOT NULL UNIQUE,
    ip_address INET,
    os_version TEXT,
    cpu_cores INT,
    memory_total_gb NUMERIC(10,2),
    disk_total_gb NUMERIC(10,2),
    created_at TIMESTAMPTZ DEFAULT NOW()
);

-- 插入本机信息 (假设 server_id 为 1)
INSERT INTO servers (hostname, ip_address, os_version, cpu_cores, memory_total_gb, disk_total_gb)
VALUES ('my-server', '127.0.0.1', 'Ubuntu 22.04', 8, 16.0, 512.0);
```

```sql
CREATE TABLE server_metrics (
    id BIGSERIAL PRIMARY KEY,
    server_id INT NOT NULL REFERENCES servers(id) ON DELETE CASCADE,
    collected_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    
    -- CPU 相关
    cpu_usage_percent DECIMAL(5,2),           -- 总使用率 %
    cpu_user_percent DECIMAL(5,2),
    cpu_system_percent DECIMAL(5,2),
    cpu_iowait_percent DECIMAL(5,2),
    
    -- 内存相关 (单位：MB 或 GB，统一即可)
    mem_total_mb INT,
    mem_used_mb INT,
    mem_available_mb INT,
    mem_usage_percent DECIMAL(5,2),
    
    -- 磁盘相关 (根分区或指定分区)
    disk_total_gb DECIMAL(10,2),
    disk_used_gb DECIMAL(10,2),
    disk_free_gb DECIMAL(10,2),
    disk_usage_percent DECIMAL(5,2),
    
    -- 系统负载 (1分钟/5分钟/15分钟)
    load_1min DECIMAL(6,2),
    load_5min DECIMAL(6,2),
    load_15min DECIMAL(6,2),
    
    -- 扩展字段：未来不确定的指标可以放这里（JSONB 类型）
    extra JSONB DEFAULT '{}'
);

-- 索引：最常用的查询条件
CREATE INDEX idx_server_metrics_time ON server_metrics (server_id, collected_at DESC);
CREATE INDEX idx_server_metrics_collected_at ON server_metrics (collected_at DESC);
```

**说明**：

- **宽表**：每个监控指标独立一列，查询时直接 `SELECT cpu_usage_percent, mem_usage_percent` 非常直观。
- **`extra` JSONB 字段**：预留扩展能力。未来如果想加 `gpu_temperature`、`network_rx_bytes` 等新指标，可以直接存 JSON，无需 `ALTER TABLE`。等这些指标稳定了，再考虑转为正式列。
- **数据类型**：`DECIMAL(5,2)` 足够存百分比；`INT` 存 MB/GB 整数；`JSONB` 存半结构化数据。

---

## 📝 插入数据示例（C++ 代码片段）

使用 `libpqxx` 批量插入（5分钟一次，单条插入即可，无需批量优化）：

```cpp
void insert_metrics(pqxx::connection& conn, int server_id, 
                    double cpu_usage, double mem_usage_percent, 
                    double disk_usage_percent) {
    pqxx::work txn(conn);
    txn.exec_params(
        "INSERT INTO server_metrics (server_id, cpu_usage_percent, mem_usage_percent, disk_usage_percent) "
        "VALUES ($1, $2, $3, $4)",
        server_id, cpu_usage, mem_usage_percent, disk_usage_percent
    );
    txn.commit();
}
```

---

## 📈 典型查询

### 查询某台服务器最近24小时的 CPU 和内存使用率

```sql
SELECT collected_at, cpu_usage_percent, mem_usage_percent
FROM server_metrics
WHERE server_id = 1 AND collected_at > NOW() - INTERVAL '1 day'
ORDER BY collected_at;
```

### 查询所有服务器的最新一条记录

```sql
SELECT DISTINCT ON (s.id) 
    s.hostname, 
    m.cpu_usage_percent, 
    m.mem_usage_percent,
    m.collected_at
FROM servers s
LEFT JOIN server_metrics m ON s.id = m.server_id
ORDER BY s.id, m.collected_at DESC;
```

### 清理90天前的数据

```sql
DELETE FROM server_metrics WHERE collected_at < NOW() - INTERVAL '90 days';
```

（数据量小，直接删除即可，无需分区）

## 数据库密码以及IP

IP是Tailscale的虚拟IP

```cpp
pqxx::connection conn("host=100.111.191.10 port=5432 dbname=test_db user=test_user password=test123");
```

## 杂项

```
test_db=> \d servers
                                         Table "public.servers"
     Column      |           Type           | Collation | Nullable |               Default
-----------------+--------------------------+-----------+----------+-------------------------------------
 id              | integer                  |           | not null | nextval('servers_id_seq'::regclass)
 machine_id      | text                     |           | not null |
 ip_address      | inet                     |           |          |
 os_version      | text                     |           |          |
 cpu_cores       | integer                  |           |          |
 memory_total_gb | numeric(10,2)            |           |          |
 disk_total_gb   | numeric(10,2)            |           |          |
 created_at      | timestamp with time zone |           |          | now()
Indexes:
    "servers_pkey" PRIMARY KEY, btree (id)
    "servers_machine_id_key" UNIQUE CONSTRAINT, btree (machine_id)
Referenced by:
    TABLE "server_metrics" CONSTRAINT "server_metrics_server_id_fkey" FOREIGN KEY (server_id) REFERENCES servers(id) ON DELETE CASCADE

test_db=> \d server_metrics
                                           Table "public.server_metrics"
       Column       |           Type           | Collation | Nullable |                  Default
--------------------+--------------------------+-----------+----------+--------------------------------------------
 id                 | bigint                   |           | not null | nextval('server_metrics_id_seq'::regclass)
 server_id          | integer                  |           | not null |
 collected_at       | timestamp with time zone |           | not null | now()
 cpu_usage_percent  | numeric(5,2)             |           |          |
 cpu_user_percent   | numeric(5,2)             |           |          |
 cpu_system_percent | numeric(5,2)             |           |          |
 cpu_iowait_percent | numeric(5,2)             |           |          |
 mem_total_mb       | integer                  |           |          |
 mem_used_mb        | integer                  |           |          |
 mem_available_mb   | integer                  |           |          |
 mem_usage_percent  | numeric(5,2)             |           |          |
 disk_total_gb      | numeric(10,2)            |           |          |
 disk_used_gb       | numeric(10,2)            |           |          |
 disk_free_gb       | numeric(10,2)            |           |          |
 disk_usage_percent | numeric(5,2)             |           |          |
 load_1min          | numeric(6,2)             |           |          |
 load_5min          | numeric(6,2)             |           |          |
 load_15min         | numeric(6,2)             |           |          |
 extra              | jsonb                    |           |          | '{}'::jsonb
Indexes:
    "server_metrics_pkey" PRIMARY KEY, btree (id)
Foreign-key constraints:
    "server_metrics_server_id_fkey" FOREIGN KEY (server_id) REFERENCES servers(id) ON DELETE CASCADE
```
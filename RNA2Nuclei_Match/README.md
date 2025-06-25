# RNA2Nuclei_Match
空间中RNA表达点到核点的匹配问题，算法导论大作业可选案  



## 一、简介

本项目针对空间转录组学实验中获取的RNA spots与细胞核（nuclei）实例的最近邻匹配问题，分别实现了以下三种算法：

1. 朴素暴力匹配（Brute-force）
2. k-d 树加速查询（kd-tree）
3. 局部敏感哈希加速（LSH）

通过对比三种方法在合成与真实数据集上的准确率（recall）和查询时延（latency），以及程序的内存峰值使用情况，验证各算法的性能差异。

## 二、环境要求

* 操作系统：Windows
* 编译器：g++ 9.0 及以上
* Python：3.7 及以上
* 依赖库（Python）：

  * numpy
  * scipy
  * psutil

## 三、文件夹结构

```
RNA2Nuclei_Match/
├── data/                     # 数据生成与存储目录
│   ├── generate_data.py      # Python 脚本：生成带标签的随机或真实数据集
│   ├── nuclei.csv        # 细胞核中心坐标样例
│   └── spots.csv         # RNA spots 坐标样例
├── build/                     # 数据生成与存储目录
│   ├── brute_force.exe     
│   ├── kdtree.exe       
│   └── lsh.exe         
├── src/                      # C++ 源代码目录
│   ├── brute_force.cpp       # 朴素暴力匹配实现
│   ├── kdtree.cpp            # kd-tree 实现与查询
│   ├── lsh.cpp               # LSH 实现与查询
│   └── utils.h               # 通用工具与数据结构定义
├── scripts/                  # Python 分析脚本目录
|   ├── visualize_results.py  # 可视化
│   └── memory_profiler.py    # 运行编译生成程序并分析内存峰值
├── results/                  # 实验结果输出目录
│   ├── logs/                 # 日志文件
│   └── figures/              # 对比图表
└── README.md                 # 项目说明（本文件）
│── run_bench.sh      # 批量执行实验并统计 recall 与 latency
```

## 四、数据生成

使用 `data/generate_data.py` 脚本生成带标签的数据集，示例命令：


生成的 `nuclei.csv` 格式为：

```
id,x,y
0,12.345,67.890
···
```

`spots.csv`：
```
id,x,y,nucleus_id
0,640.2776325142905,40.37848134811401,10
```

## 五、编译与运行

在项目根目录下执行：

```bash
# 创建编译输出目录\ nmkdir -p build
# 编译三种算法
g++ -O3 src/brute_force.cpp -o build/brute_force
g++ -O3 src/kdtree.cpp -o build/kdtree
g++ -O3 src/lsh.cpp -o build/lsh
```

运行示例：

```bash
# 暴力匹配
./build/brute_force --nuclei data/samples/nuclei.csv --spots data/spots.csv --out results/logs/brute_force.txt
# kd-tree 加速
./build/kdtree --nuclei data/samples/nuclei.csv --spots data/spots.csv --out results/logs/kdtree.txt
# LSH 加速
./build/lsh --nuclei data/samples/nuclei.csv --spots data/spots.csv --out results/logs/lsh.txt
```

## 六、实验评估

1. **准确率（Recall）与时延（Latency）**：使用 `run_benchmark.sh` 编译运行对比三种算法的性能。
2. **内存峰值**：使用 `scripts/memory_profiler.py` 分析各算法在执行过程中占用的最大内存。

示例：

```bash
# 性能对比
bash run_bench.sh

# 内存分析
```

## 七、参考文献



---



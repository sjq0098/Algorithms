#!/usr/bin/env python3
import psutil
import time
import argparse
import subprocess
import threading
import matplotlib.pyplot as plt
from pathlib import Path

class MemoryProfiler:
    def __init__(self, interval=0.1):
        """
        初始化内存分析器
        :param interval: 采样间隔（秒）
        """
        self.interval = interval
        self.memory_usage = []
        self.is_running = False
        self.peak_memory = 0
        self.start_time = None
        self.time_points = []

    def _monitor_memory(self, pid):
        """
        监控指定进程的内存使用
        """
        try:
            process = psutil.Process(pid)
            self.start_time = time.time()
            
            while self.is_running:
                try:
                    # 获取内存使用（MB）
                    memory = process.memory_info().rss / 1024 / 1024
                    self.memory_usage.append(memory)
                    self.peak_memory = max(self.peak_memory, memory)
                    self.time_points.append(time.time() - self.start_time)
                    time.sleep(self.interval)
                except (psutil.NoSuchProcess, psutil.AccessDenied):
                    break
        except psutil.NoSuchProcess:
            print("进程不存在或已终止")

    def start_monitoring(self, pid):
        """
        开始监控内存使用
        """
        self.is_running = True
        self.monitor_thread = threading.Thread(target=self._monitor_memory, args=(pid,))
        self.monitor_thread.start()

    def stop_monitoring(self):
        """
        停止监控内存使用
        """
        self.is_running = False
        if hasattr(self, 'monitor_thread'):
            self.monitor_thread.join()

    def plot_memory_usage(self, output_file):
        """
        绘制内存使用曲线
        """
        plt.figure(figsize=(10, 6))
        plt.plot(self.time_points, self.memory_usage)
        plt.title('内存使用情况')
        plt.xlabel('时间 (秒)')
        plt.ylabel('内存使用 (MB)')
        plt.grid(True)
        
        # 标注峰值
        peak_time = self.time_points[self.memory_usage.index(self.peak_memory)]
        plt.annotate(f'峰值: {self.peak_memory:.2f}MB',
                    xy=(peak_time, self.peak_memory),
                    xytext=(10, 10), textcoords='offset points',
                    ha='left',
                    bbox=dict(boxstyle='round,pad=0.5', fc='yellow', alpha=0.5),
                    arrowprops=dict(arrowstyle='->'))
        
        plt.savefig(output_file)
        plt.close()

def run_with_profiling(exec_path, args, output_dir):
    """
    运行程序并分析内存使用
    """
    # 创建输出目录
    Path(output_dir).mkdir(parents=True, exist_ok=True)
    
    # 启动进程
    cmd = [exec_path] + args
    process = subprocess.Popen(cmd)
    
    # 开始内存监控
    profiler = MemoryProfiler()
    profiler.start_monitoring(process.pid)
    
    # 等待进程结束
    process.wait()
    
    # 停止监控
    profiler.stop_monitoring()
    
    # 生成报告
    program_name = Path(exec_path).stem
    plot_file = f"{output_dir}/{program_name}_memory_usage.png"
    profiler.plot_memory_usage(plot_file)
    
    # 输出结果
    print(f"\n内存使用情况:")
    print(f"峰值内存: {profiler.peak_memory:.2f} MB")
    print(f"内存使用曲线已保存到: {plot_file}")

def main():
    parser = argparse.ArgumentParser(description='分析程序内存使用情况')
    parser.add_argument('--exec', type=str, default='build/lsh',
                        help='要分析的可执行文件路径')
    parser.add_argument('--args', type=str, default='--nuclei data/samples/nuclei.csv --spots data/samples/spots.csv',
                        help='传递给可执行文件的参数，用空格分隔')
    parser.add_argument('--output_dir', type=str, default='results/memory',
                        help='输出目录路径')
    
    args = parser.parse_args()
    exec_args = args.args.split() if args.args else []
    
    run_with_profiling(args.exec, exec_args, args.output_dir)

if __name__ == '__main__':
    main() 
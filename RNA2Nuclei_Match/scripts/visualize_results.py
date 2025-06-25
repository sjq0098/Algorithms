#!/usr/bin/env python3
import os
import argparse
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

def calculate_recall(true_matches_file, pred_matches_file):
    """计算预测匹配的准确率"""
    true_df = pd.read_csv(true_matches_file)
    pred_df = pd.read_csv(pred_matches_file)
    
    merged_df = pd.merge(true_df, pred_df, on='spot_id', suffixes=('_true', '_pred'))
    correct_matches = (merged_df['nucleus_id_true'] == merged_df['nucleus_id_pred']).sum()
    
    return correct_matches / len(true_df)

def analyze_results(data_dir, logs_dir, output_dir):
    """分析结果并生成可视化"""
    # 创建输出目录
    Path(output_dir).mkdir(parents=True, exist_ok=True)
    
    # 读取运行时间数据
    timing_df = pd.read_csv(os.path.join(logs_dir, 'timing.csv'),
                           names=['method', 'dataset', 'latency'])
    
    # 计算每个方法在每个数据集上的准确率
    recalls = []
    for _, row in timing_df.iterrows():
        method, dataset = row['method'], row['dataset']
        true_file = os.path.join(data_dir, f'spots_{dataset}.csv')
        pred_file = os.path.join(logs_dir, f'{method}_{dataset}.csv')
        recall = calculate_recall(true_file, pred_file)
        recalls.append(recall)
    
    timing_df['recall'] = recalls
    
    # 绘制性能对比图
    plt.figure(figsize=(12, 5))
    
    # 延迟对比
    plt.subplot(1, 2, 1)
    for method in timing_df['method'].unique():
        method_data = timing_df[timing_df['method'] == method]
        plt.plot(method_data['dataset'], method_data['latency'], 'o-', label=method)
    plt.title('查询延迟对比')
    plt.xlabel('数据集')
    plt.ylabel('延迟 (秒)')
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    
    # 准确率对比
    plt.subplot(1, 2, 2)
    for method in timing_df['method'].unique():
        method_data = timing_df[timing_df['method'] == method]
        plt.plot(method_data['dataset'], method_data['recall'], 'o-', label=method)
    plt.title('准确率对比')
    plt.xlabel('数据集')
    plt.ylabel('Recall')
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    
    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, 'performance_comparison.png'))
    plt.close()
    
    # 保存详细结果
    timing_df.to_csv(os.path.join(output_dir, 'benchmark_results.csv'), index=False)
    
    # 生成汇总统计
    summary = timing_df.groupby('method').agg({
        'latency': ['mean', 'std', 'min', 'max'],
        'recall': ['mean', 'std', 'min', 'max']
    }).round(4)
    
    summary.to_csv(os.path.join(output_dir, 'summary_statistics.csv'))
    
    print("\n结果分析完成！")
    print(f"详细结果已保存到: {os.path.join(output_dir, 'benchmark_results.csv')}")
    print(f"性能对比图已保存到: {os.path.join(output_dir, 'performance_comparison.png')}")
    print(f"统计摘要已保存到: {os.path.join(output_dir, 'summary_statistics.csv')}")

def main():
    parser = argparse.ArgumentParser(description='分析算法性能结果并生成可视化')
    parser.add_argument('--data', type=str, default='data',
                        help='原始数据集目录路径')
    parser.add_argument('--logs', type=str, default='results/logs',
                        help='算法输出日志目录路径')
    parser.add_argument('--output', type=str, default='results/figures',
                        help='可视化结果输出目录路径')
    
    args = parser.parse_args()
    analyze_results(args.data, args.logs, args.output)

if __name__ == '__main__':
    main() 
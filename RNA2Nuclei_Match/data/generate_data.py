import os
import argparse
import numpy as np
import pandas as pd


def generate_data(num_nuclei, num_spots, out_dir,
                  field_size=1000,  # 平面区域边长 (单位: 任意)
                  spot_std=10.0,    # RNA 点相对于细胞核中心的高斯分布标准差
                  noise_ratio=0.0   # 噪声点占总点数的比例
                  ):
    """
    生成细胞核中心和RNA点，并保存至指定目录

    参数:
        num_nuclei (int): 细胞核中心数量
        num_spots (int): RNA 点数量
        out_dir (str): 输出目录
        field_size (float): 区域大小
        spot_std (float): 信号点标准差
        noise_ratio (float): 噪声点比例
    """
    # 创建输出目录
    os.makedirs(out_dir, exist_ok=True)

    # 在区域内均匀生成细胞核中心坐标
    nuclei_x = np.random.uniform(0, field_size, size=num_nuclei)
    nuclei_y = np.random.uniform(0, field_size, size=num_nuclei)
    nuclei = pd.DataFrame({
        'id': np.arange(num_nuclei, dtype=int),
        'x': nuclei_x,
        'y': nuclei_y
    })

    # 计算噪声点数量和信号点数量
    num_noise = int(noise_ratio * num_spots)
    num_signal = num_spots - num_noise

    # 对于信号点: 随机选择对应细胞核并按高斯分布生成坐标
    spot_nucleus_ids = np.random.randint(0, num_nuclei, size=num_signal)
    signal_x = nuclei_x[spot_nucleus_ids] + np.random.normal(0, spot_std, size=num_signal)
    signal_y = nuclei_y[spot_nucleus_ids] + np.random.normal(0, spot_std, size=num_signal)

    # 对于噪声点: 在区域内均匀生成坐标
    noise_x = np.random.uniform(0, field_size, size=num_noise)
    noise_y = np.random.uniform(0, field_size, size=num_noise)
    noise_ids = np.full(num_noise, -1, dtype=int)

    # 合并信号点和噪声点数据
    spots_id = np.arange(num_spots, dtype=int)
    spots_x = np.concatenate([signal_x, noise_x])
    spots_y = np.concatenate([signal_y, noise_y])
    initial_labels = np.concatenate([spot_nucleus_ids, noise_ids])

    # 重新计算每个点最近的细胞核
    # 所有核的坐标
    nuclei_coords = np.vstack([nuclei_x, nuclei_y]).T  # shape: (num_nuclei, 2)
    # 所有点的坐标
    spots_coords = np.vstack([spots_x, spots_y]).T     # shape: (num_spots, 2)
    # 计算距离矩阵并选取最小值索引
    dists = np.linalg.norm(spots_coords[:, None, :] - nuclei_coords[None, :, :], axis=2)
    nearest_ids = np.argmin(dists, axis=1)
    # 对噪声点保留 -1，其他点使用 nearest_ids
    final_labels = np.where(initial_labels < 0, -1, nearest_ids)

    # 构建 spots DataFrame
    spots = pd.DataFrame({
        'id': spots_id,
        'x': spots_x,
        'y': spots_y,
        'nucleus_id': final_labels
    })

    # 保存为 CSV 文件
    nuclei_path = os.path.join(out_dir, 'nuclei.csv')
    spots_path = os.path.join(out_dir, 'spots.csv')
    nuclei.to_csv(nuclei_path, index=False)
    spots.to_csv(spots_path, index=False)

    print(f"已生成 {num_nuclei} 个细胞核 -> {nuclei_path}")
    print(f"已生成 {num_spots} 个RNA点（其中噪声点 {num_noise} 个）-> {spots_path}")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='生成合成的细胞核中心和RNA点数据'
    )
    parser.add_argument('--nuclei', type=int, default=1000,
                        help='生成的细胞核数量')
    parser.add_argument('--spots', type=int, default=100000,
                        help='生成的RNA点数量')
    parser.add_argument('--out_dir', type=str, default='data',
                        help='输出目录')
    parser.add_argument('--field_size', type=float, default=1000.0,
                        help='区域边长 (默认: 1000)')
    parser.add_argument('--spot_std', type=float, default=10.0,
                        help='信号点分布标准差 (默认: 10.0)')
    parser.add_argument('--noise_ratio', type=float, default=0.1,
                        help='噪声点比例 (默认: 0.1)')
    args = parser.parse_args()

    generate_data(
        num_nuclei=args.nuclei,
        num_spots=args.spots,
        out_dir=args.out_dir,
        field_size=args.field_size,
        spot_std=args.spot_std,
        noise_ratio=args.noise_ratio
    )

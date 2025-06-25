#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>

// 定义点结构
struct Point {
    int id;
    double x, y;
    int nucleus_id;  // 仅用于spots点的真实标签

    Point() : id(-1), x(0), y(0), nucleus_id(-1) {}
    Point(int _id, double _x, double _y, int _nucleus_id = -1) 
        : id(_id), x(_x), y(_y), nucleus_id(_nucleus_id) {}
};

// 计算两点之间的欧氏距离
inline double distance(const Point& p1, const Point& p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + 
                     (p1.y - p2.y) * (p1.y - p2.y));
}

// 从CSV文件读取点数据
inline std::vector<Point> readPointsFromCSV(const std::string& filename, bool isSpot = false) {
    std::vector<Point> points;
    std::ifstream file(filename);
    std::string line;
    
    // 跳过表头
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        int id = std::stoi(tokens[0]);
        double x = std::stod(tokens[1]);
        double y = std::stod(tokens[2]);
        int nucleus_id = isSpot ? std::stoi(tokens[3]) : -1;
        
        points.emplace_back(id, x, y, nucleus_id);
    }
    
    return points;
}

// 将匹配结果写入文件
inline void writeResults(const std::string& filename, 
                        const std::vector<std::pair<int, int>>& matches) {
    std::ofstream file(filename);
    file << "spot_id,nucleus_id\n";
    for (const auto& match : matches) {
        file << match.first << "," << match.second << "\n";
    }
}

#endif // UTILS_H
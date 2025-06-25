#include "utils.h"
#include <random>
#include <unordered_map>
#include <chrono>
#include <cmath>

// LSH参数
const int NUM_HASH_TABLES = 10;  // 哈希表数量
const int NUM_HASH_FUNCTIONS = 4;  // 每个哈希表的哈希函数数量
const double W = 4.0;  // LSH的桶宽度

// LSH哈希函数
class LSHFunction {
private:
    std::vector<double> a;  // 随机投影向量
    double b;  // 随机偏移
    
public:
    LSHFunction() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> normal(0, 1);
        
        // 生成2D随机投影向量
        a = {normal(gen), normal(gen)};
        std::uniform_real_distribution<> uniform(0, W);
        b = uniform(gen);
    }
    
    int hash(const Point& p) const {
        double proj = a[0] * p.x + a[1] * p.y;
        return static_cast<int>((proj + b) / W);
    }
};

// LSH哈希表
class LSHTable {
private:
    std::vector<LSHFunction> hashFunctions;
    std::unordered_map<std::string, std::vector<Point>> table;
    
    std::string getHashKey(const Point& p) const {
        std::string key;
        for (const auto& func : hashFunctions) {
            key += std::to_string(func.hash(p)) + "_";
        }
        return key;
    }
    
public:
    LSHTable() {
        for (int i = 0; i < NUM_HASH_FUNCTIONS; ++i) {
            hashFunctions.emplace_back();
        }
    }
    
    void insert(const Point& p) {
        std::string key = getHashKey(p);
        table[key].push_back(p);
    }
    
    std::vector<Point> query(const Point& p) const {
        std::string key = getHashKey(p);
        auto it = table.find(key);
        if (it != table.end()) {
            return it->second;
        }
        return std::vector<Point>();
    }
};

class LSH {
private:
    std::vector<LSHTable> hashTables;
    
public:
    LSH() {
        for (int i = 0; i < NUM_HASH_TABLES; ++i) {
            hashTables.emplace_back();
        }
    }
    
    void insert(const Point& p) {
        for (auto& table : hashTables) {
            table.insert(p);
        }
    }
    
    Point findNearest(const Point& query) const {
        Point nearest;
        double minDist = std::numeric_limits<double>::max();
        
        // 在所有哈希表中查找候选点
        for (const auto& table : hashTables) {
            auto candidates = table.query(query);
            for (const auto& candidate : candidates) {
                double dist = distance(query, candidate);
                if (dist < minDist) {
                    minDist = dist;
                    nearest = candidate;
                }
            }
        }
        
        return nearest;
    }
};

void printUsage() {
    std::cout << "Usage: ./lsh --nuclei <nuclei_file> --spots <spots_file> --out <output_file>" << std::endl;
}

std::vector<std::pair<int, int>> findNearestNucleiLSH(
    const std::vector<Point>& nuclei,
    const std::vector<Point>& spots) {
    
    // 构建LSH索引
    LSH lsh;
    for (const auto& nucleus : nuclei) {
        lsh.insert(nucleus);
    }
    
    std::vector<std::pair<int, int>> matches;
    matches.reserve(spots.size());

    // 对每个spot查找最近的nucleus
    for (const auto& spot : spots) {
        Point nearest = lsh.findNearest(spot);
        matches.emplace_back(spot.id, nearest.id);
    }

    return matches;
}

int main(int argc, char* argv[]) {
    std::string nucleiFile, spotsFile, outputFile;

    // 解析命令行参数
    for (int i = 1; i < argc; i += 2) {
        std::string arg = argv[i];
        if (arg == "--nuclei") nucleiFile = argv[i + 1];
        else if (arg == "--spots") spotsFile = argv[i + 1];
        else if (arg == "--out") outputFile = argv[i + 1];
    }

    if (nucleiFile.empty() || spotsFile.empty() || outputFile.empty()) {
        printUsage();
        return 1;
    }

    // 读取数据
    auto nuclei = readPointsFromCSV(nucleiFile);
    auto spots = readPointsFromCSV(spotsFile, true);

    // 计时开始
    auto start = std::chrono::high_resolution_clock::now();

    // 执行匹配
    auto matches = findNearestNucleiLSH(nuclei, spots);

    // 计时结束
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // 输出执行时间
    std::cout << "Matching completed in " << duration.count() << " ms" << std::endl;
    std::cout << "Total matches: " << matches.size() << std::endl;

    // 保存结果
    writeResults(outputFile, matches);

    return 0;
} 
#include "utils.h"
#include <algorithm>
#include <chrono>

void printUsage() {
    std::cout << "Usage: ./brute_force --nuclei <nuclei_file> --spots <spots_file> --out <output_file>" << std::endl;
}

std::vector<std::pair<int, int>> findNearestNuclei(
    const std::vector<Point>& nuclei,
    const std::vector<Point>& spots) {
    
    std::vector<std::pair<int, int>> matches;
    matches.reserve(spots.size());

    // 对每个spot找到最近的nucleus
    for (const auto& spot : spots) {
        double minDist = std::numeric_limits<double>::max();
        int nearestNucleusId = -1;

        for (const auto& nucleus : nuclei) {
            double dist = distance(spot, nucleus);
            if (dist < minDist) {
                minDist = dist;
                nearestNucleusId = nucleus.id;
            }
        }

        matches.emplace_back(spot.id, nearestNucleusId);
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
    auto matches = findNearestNuclei(nuclei, spots);

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
#include "utils.h"
#include <algorithm>
#include <memory>
#include <chrono>
#include <limits>

struct KDNode {
    Point point;
    std::unique_ptr<KDNode> left, right;
    int axis;  // 0 for x, 1 for y

    KDNode(const Point& p, int ax) : point(p), axis(ax) {}
};

class KDTree {
private:
    std::unique_ptr<KDNode> root;

    std::unique_ptr<KDNode> buildTree(std::vector<Point>& points, int depth, int start, int end) {
        if (start >= end) return nullptr;

        int axis = depth % 2;  // 在x和y轴之间交替
        int mid = (start + end) / 2;

        // 按当前轴排序
        std::nth_element(points.begin() + start, points.begin() + mid,
                        points.begin() + end,
                        [axis](const Point& a, const Point& b) {
                            return axis == 0 ? a.x < b.x : a.y < b.y;
                        });

        auto node = std::make_unique<KDNode>(points[mid], axis);
        node->left = buildTree(points, depth + 1, start, mid);
        node->right = buildTree(points, depth + 1, mid + 1, end);
        return node;
    }

    void findNearest(KDNode* node, const Point& target, Point& best, double& bestDist) const {
        if (!node) return;

        double dist = distance(target, node->point);
        if (dist < bestDist) {
            bestDist = dist;
            best = node->point;
        }

        double axisDist = (node->axis == 0) ? 
                         target.x - node->point.x : 
                         target.y - node->point.y;

        // 递归搜索最可能包含最近点的子树
        KDNode* first = axisDist <= 0 ? node->left.get() : node->right.get();
        KDNode* second = axisDist <= 0 ? node->right.get() : node->left.get();

        findNearest(first, target, best, bestDist);

        // 如果另一边可能有更近的点，也要搜索
        if (axisDist * axisDist < bestDist) {
            findNearest(second, target, best, bestDist);
        }
    }

public:
    KDTree(std::vector<Point>& points) {
        root = buildTree(points, 0, 0, points.size());
    }

    Point findNearest(const Point& target) const {
        Point best;
        double bestDist = std::numeric_limits<double>::max();
        findNearest(root.get(), target, best, bestDist);
        return best;
    }
};

void printUsage() {
    std::cout << "Usage: ./kdtree --nuclei <nuclei_file> --spots <spots_file> --out <output_file>" << std::endl;
}

std::vector<std::pair<int, int>> findNearestNucleiKDTree(
    std::vector<Point>& nuclei,
    const std::vector<Point>& spots) {
    
    // 构建k-d树
    KDTree kdtree(nuclei);
    
    std::vector<std::pair<int, int>> matches;
    matches.reserve(spots.size());

    // 对每个spot查找最近的nucleus
    for (const auto& spot : spots) {
        Point nearest = kdtree.findNearest(spot);
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
    auto matches = findNearestNucleiKDTree(nuclei, spots);

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
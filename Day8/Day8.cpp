#include <vector>
#include <iostream>
#include <cassert>
#include <string>
#include <fstream>
#include <algorithm>

struct Point
{
    int x, y, z;
};

std::vector<Point> parse_points(const char* path)
{
    std::vector<Point> points;

    std::ifstream file(path);
    if (!file)
    {
        std::cerr << "Cannot open file: " << path << std::endl;
        return points;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        int x, y, z;
        if (sscanf_s(line.c_str(), "%d,%d,%d", &x, &y, &z) == 3)
        {
            points.push_back({ x, y, z });
        }
    }

    return points;
}

struct UnionFind {
    std::vector<int> parent, size;

    UnionFind(int n) : parent(n), size(n, 1)
    {
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }

    // Find root with path compression
    int find(int x)
    {
        return parent[x] != x ? parent[x] = find(parent[x]) : x;
    }

    // Union by size, returns true if merged
    bool unite(int x, int y)
    {
        x = find(x); y = find(y);
        if (x == y)
            return false;  // Already same component

        if (size[x] < size[y]) std::swap(x, y);
        parent[y] = x;
        size[x] += size[y];
        return true;
    }

    int get_size(int x)
    {
        return size[find(x)];
    }
};

struct Edge
{
	int u, v; // Indices of the points
    double dist;

	bool operator<(const Edge& other) const  // For sorting edges by distance
    {
        return dist < other.dist;
    }
};

std::vector<Edge> generate_edges(const std::vector<Point>& points)
{
    int n = (int)points.size();
    std::vector<Edge> edges;
    edges.reserve(static_cast<size_t>(n) * (n - 1) / 2);

    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            double dx = points[i].x - points[j].x;
            double dy = points[i].y - points[j].y;
            double dz = points[i].z - points[j].z;
            double dist = std::sqrt(dx * dx + dy * dy + dz * dz);

            edges.push_back({ i, j, dist });
        }
    }

    std::sort(edges.begin(), edges.end());
    return edges;
}

int main() {
    auto points = parse_points("Data//Input.txt");
    std::cout << "Parsed " << points.size() << " points\n";

    auto edges = generate_edges(points);
    UnionFind uf((int)points.size());

	constexpr int NUM_EDGES_TO_CONNECT = 1000;

	// Connect closest points until we have NUM_EDGES_TO_CONNECT connections
    int connected = 0;
    for (const auto& edge : edges)
    {
        //std::cout << "Considering edge between point " << points[edge.u].x << "," << points[edge.u].y << "," << points[edge.u].z << " and point " << points[edge.v].x << "," << points[edge.v].y << "," << points[edge.v].z << "\n";
        uf.unite(edge.u, edge.v);  //we dont care if they were already connected
            
        if (++connected == NUM_EDGES_TO_CONNECT)
            break;
    }

    // Find 3 largest components
    std::vector<int> sizes;
    for (int i = 0; i < points.size(); ++i)
    {
		if (uf.find(i) == i) //we only want to consider root nodes
            sizes.push_back(uf.size[i]);
    }

    std::sort(sizes.rbegin(), sizes.rend());

	// print sizes for debugging
    for (size_t i = 0; i < sizes.size(); ++i)
    {
        std::cout << "Group " << i << " size: " << sizes[i] << "\n";
	}

	unsigned int ThreeLargestProduct = sizes[0] * sizes[1] * sizes[2];

	std::cout << "Part 1 - Product of sizes of three largest components: " << ThreeLargestProduct << "\n";

    return 0;
}

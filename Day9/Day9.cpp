// Day9.cpp
// https://adventofcode.com/2025/day/9

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <assert.h>
#include <unordered_map>
#include <unordered_set>

using namespace std;

constexpr bool IS_PART1 = false;

struct Point
{
    int x, y;

    bool operator==(const Point& o) const
    {
        return x == o.x && y == o.y;
    }
};

// Hash for unordered_set<Point>
struct PointHash
{
    size_t operator()(const Point& p) const
    {
        uint64_t key = p.x * 10000000 + p.y;
        return hash<uint64_t>{}(key);
    }
};

vector<Point> readFileLines(const char* filename)
{
    vector<Point> redTiles;
    ifstream file(filename);
    if (!file)
    {
        cerr << "Cannot open file: " << filename << endl;
        return redTiles;
    }
    string line;
    while (getline(file, line))
    {
		// read pairs of integers separated by a comma (e.g. 87797,20888)
        int x, y;
		int numFound = sscanf_s(line.c_str(), "%d,%d", &x, &y);
        assert(numFound == 2 && "Error reading input file");
		redTiles.emplace_back(x, y);
    }
    return redTiles;
}

void DEBUG_printMap(const vector<Point>& redTiles, const unordered_set<Point, PointHash>& greenTiles)
{
    //find bounds
    int minx = INT_MAX, maxx = INT_MIN, miny = INT_MAX, maxy = INT_MIN;
    for (const auto& p : redTiles)
    {
        minx = min(minx, p.x); maxx = max(maxx, p.x);
        miny = min(miny, p.y); maxy = max(maxy, p.y);
    }
    //reduce bounds a bit for better visualization
    minx -= 1; maxx += 1; miny -= 1; maxy += 1;
    
    //debug:: print the map, # for red, X for green (go from minx to maxx, miny to maxy)
    for (int y = miny; y <= maxy; ++y)
    {
        for (int x = minx; x <= maxx; ++x)
        {
            Point p{x, y};
            if (find(redTiles.begin(), redTiles.end(), p) != redTiles.end())
                cout << '#';
            else if (greenTiles.contains(p))
                cout << 'X';
            else
                cout << '.';
        }
        cout << '\n';
    }
    cout << std::endl;
}

//part 1: find the largest square that can be formed by any two points
static int findLargestSquare(const vector<Point>& points)
{
    //now we want to find the largest Manhattan distance between any two points
    int maxDistance = 0;
    int indexPointA = -1;
    int indexPointB = -1;
    for (size_t i = 0; i < points.size(); ++i)
    {
        for (size_t j = i + 1; j < points.size(); ++j)
        {
            int distance = abs(points[i].x - points[j].x) + abs(points[i].y - points[j].y);
            if (distance > maxDistance)
            {
                maxDistance = distance;
                indexPointA = (int)i;
                indexPointB = (int)j;
            }
        }
    }
    
    cout << "Max Distance: " << maxDistance << " between points "
    << points[indexPointA].x << "," << points[indexPointA].y << " and "
    << points[indexPointB].x << "," << points[indexPointB].y << " the size of the red rectangle is: "
    << abs(points[indexPointB].x - points[indexPointA].x) + 1 << " by " << abs(points[indexPointB].y - points[indexPointA].y) + 1
    << " which is an area of " << (uint64_t)(abs(points[indexPointB].x - points[indexPointA].x) + 1) * (abs(points[indexPointB].y - points[indexPointA].y) + 1);
    
    return maxDistance;
}

// Build all green tiles on the loop between consecutive red points
unordered_set<Point, PointHash> buildGreenEdges(const vector<Point>& red)
{
    unordered_set<Point, PointHash> greenTiles;
    int n = int(red.size());

    for (int i = 0; i < n; ++i)
    {
        Point a = red[i];
        Point b = red[(i + 1) % n];

        if (a.x == b.x)
        {
            int miny = min(a.y, b.y);
            int maxy = max(a.y, b.y);
            for (int y = miny; y <= maxy; ++y)
                greenTiles.insert({a.x, y});
        }
        else if (a.y == b.y)
        {
            int minx = min(a.x, b.x);
            int maxx = max(a.x, b.x);
            for (int x = minx; x <= maxx; ++x)
                greenTiles.insert({x, a.y});
        }
        else
        {
            assert(false && "Problem guarantees horizontal/vertical segments only");
        }
    }

    //CHANGE: not a good idea to scanline fill here, because the shape is very large since the coordinates are large
    // // 2) Find bounds
    // int minx = INT_MAX, maxx = INT_MIN, miny = INT_MAX, maxy = INT_MIN;
    // for (const auto& p : red)
    // {
    //     minx = min(minx, p.x); maxx = max(maxx, p.x);
    //     miny = min(miny, p.y); maxy = max(maxy, p.y);
    // }
    //
    // // 3) Scanline: Go row by row, flip inside/outside when crossing edge
    // for (int y = miny; y <= maxy; ++y)
    // {
    //     bool inside = false;
    //     bool onEdge = false;
    //
    //     for (int x = minx; x <= maxx; ++x)
    //     {
    //         Point p{x, y};
    //         const bool isPerimeter = greenTiles.contains(p);
    //
    //         if (isPerimeter)
    //         {
    //             if (!onEdge)  // Entering an edge
    //             {
    //                 // Check if this edge has vertical component
    //                 Point above{x, y - 1};
    //                 Point below{x, y + 1};
    //                 if (greenTiles.contains(above) || greenTiles.contains(below))
    //                 {
    //                     inside = !inside;
    //                 }
    //                 onEdge = true;
    //             }
    //         }
    //         else  // Not on perimeter
    //         {
    //             onEdge = false;
    //             if (inside)
    //                 greenTiles.insert(p);
    //         }
    //     }
    // }
    
    return greenTiles;
}

struct Rect
{
    Point min, max;

    uint64_t area() const
    {
        return uint64_t(max.x - min.x + 1) * uint64_t(max.y - min.y + 1);
    }
    
    bool operator>(const Rect& other) const
    {
        return area() > other.area();
    }
    
    // bool fullyContains(const Rect& other) const
    // {
    //     return (other.min.x >= min.x && other.max.x <= max.x &&
    //             other.min.y >= min.y && other.max.y <= max.y);
    // }
};

//generate all rectangles defined by any two red tiles (not checked yet if inside green loop), sorted by area descending
vector<Rect> generateAllRectangles(const vector<Point>& redTiles)
{    
    size_t numTiles = redTiles.size();

    vector<Rect> rects;
    rects.reserve(numTiles * (numTiles - 1) / 2);

    for (size_t i = 0; i < numTiles; ++i)
    {
        for (size_t j = i + 1; j < numTiles; ++j)
        {
            const Point& a = redTiles[i];
            const Point& b = redTiles[j];

            Rect r
            {
                {std::min(a.x, b.x), std::min(a.y, b.y)},
                {std::max(a.x, b.x), std::max(a.y, b.y)}
            };

            rects.push_back(r);
        }
    }
    
    std::sort(rects.begin(), rects.end(), greater<Rect>());
    
    return rects;
}

unordered_map<Point, bool, PointHash> pointInPolygonCache;

// Ray casting: count how many times a ray crosses the polygon boundary  //<-- todo: super slow for large polygons, optimize with preccalucate the edges...?
bool isInsidePolygon(const Point& p, const vector<Point>& red)
{
    // Check cache first
    if (const auto it = pointInPolygonCache.find(p); it != pointInPolygonCache.end())
        return it->second;
    
    int n = int(red.size());
    int crossings = 0;

    for (int i = 0; i < n; ++i)
    {
        Point a = red[i];
        Point b = red[(i + 1) % n];
        
        // Only check edges that cross our horizontal ray to the right
        if ((a.y <= p.y && b.y > p.y) || (b.y <= p.y && a.y > p.y))
        {
            // // Compute x-coordinate of intersection
            // double x_intersect = a.x + double(p.y - a.y) * (b.x - a.x) / (b.y - a.y);
            // For vertical/horizontal polygons only
            int x_intersect = a.x;  // No division needed!
            
            if (p.x < x_intersect)
                crossings++;
        }
    }
    
    bool inside = (crossings % 2) == 1;
    pointInPolygonCache.emplace(p, inside);
    return inside;
}

bool isRectangleValid(const Rect& r, const vector<Point>& redTiles, const unordered_set<Point, PointHash>& greenTiles)
{
    // Check all four edges first (fail fast)
    for (int x = r.min.x; x <= r.max.x; ++x)
    {
        Point p{x, r.min.y};
        if (!greenTiles.contains(p) && !isInsidePolygon(p, redTiles))
            return false;
        
        p = {x, r.max.y};
        if (!greenTiles.contains(p) && !isInsidePolygon(p, redTiles))
            return false;
    }
    
    for (int y = r.min.y + 1; y < r.max.y; ++y)
    {
        Point p{r.min.x, y};
        if (!greenTiles.contains(p) && !isInsidePolygon(p, redTiles))
            return false;
        
        p = {r.max.x, y};
        if (!greenTiles.contains(p) && !isInsidePolygon(p, redTiles))
            return false;
    }

    // If perimeter is valid, interior is guaranteed valid by polygon convexity
    // (the problem guarantees a simple polygon)
    return true;
}

uint64_t findLargestValidSquare(const vector<Point>& redTiles, const unordered_set<Point, PointHash>& greenTiles, const vector<Rect>& allRedSquares)
{
    pointInPolygonCache.reserve(1000000); //preallocate some space for caching
    for (const Rect& r : allRedSquares)
    {
        if (isRectangleValid(r, redTiles, greenTiles))
        {
            cout << "Found valid rectangle with area: " << r.area() << " from " << r.min.x << "," << r.min.y << " to " << r.max.x << "," << r.max.y << std::endl;
            return r.area();
        }
    }
    return 0;
}

int main()
{
    auto redTiles = readFileLines("Data//Input.txt"); //positions of the red tiles

    // part1
    if (IS_PART1)
    {
        int maxDistance = findLargestSquare(redTiles);
        return 0;
    }
    
    // part2
    cout << "1. Building green area..." << std::endl;
    auto greenTiles = buildGreenEdges(redTiles);
    cout << "2. Generating all red rectangles..." << std::endl;
    //DEBUG_printMap(redTiles, greenTiles);
    auto allRedSquares = generateAllRectangles(redTiles);    
    cout << "Generated " << allRedSquares.size() << " rectangles." << std::endl;
    cout << "3. Finding largest valid square..." << std::endl;    
    uint64_t best_area = findLargestValidSquare(redTiles, greenTiles, allRedSquares);
    std::cout << "Largest valid square area is: " << best_area << std::endl;

    return 0;
}

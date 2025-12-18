// Day9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <assert.h>

std::vector<std::pair<int,int>> readFileLines(const char* filename)
{
    std::vector<std::pair<int, int>> lines;
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return lines;
    }
    std::string line;
    while (std::getline(file, line))
    {
		// read pairs of integers separated by a comma (e.g. 87797,20888)
        int x, y;
		sscanf_s(line.c_str(), "%d,%d", &x, &y);
		lines.push_back({ x, y });
    }
    return lines;
}

int main()
{
    auto points = readFileLines("Data//Input.txt"); //positions of the red tiles

    //now we want to find the largest Manhattan distance between any two points
    int maxDistance = 0;
    int indexPointA = -1;
    int indexPointB = -1;
    for (size_t i = 0; i < points.size(); ++i)
    {
        for (size_t j = i + 1; j < points.size(); ++j)
        {
            int distance = abs(points[i].first - points[j].first) + abs(points[i].second - points[j].second);
            if (distance > maxDistance)
            {
                maxDistance = distance;
                indexPointA = (int)i;
                indexPointB = (int)j;
            }
        }
    }

    std::cout << "Max Distance: " << maxDistance << " between points "
        << points[indexPointA].first << "," << points[indexPointA].second << " and "
        << points[indexPointB].first << "," << points[indexPointB].second << " the size of the red rectangle is: "
        << abs(points[indexPointB].first - points[indexPointA].first) + 1 << " by " << abs(points[indexPointB].second - points[indexPointA].second) + 1
        << " which is an area of " << (uint64_t)(abs(points[indexPointB].first - points[indexPointA].first) + 1) * (abs(points[indexPointB].second - points[indexPointA].second) + 1);

    return 0;
}

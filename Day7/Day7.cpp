// Day7.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://adventofcode.com/2025/day/7

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <assert.h>
#include <unordered_map>

std::vector<std::string> readFileLines(const char* filename)
{
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return lines;
    }
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    return lines;
}

struct Position
{
    size_t x;
    size_t y;

    bool operator==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }

    struct Hash
    {
        std::size_t operator()(const Position& pos) const
        {
            return std::hash<size_t>{}(pos.x) ^ (std::hash<size_t>{}(pos.y) << 1);
        }
    };
};


//Part 2
// Memoization map
std::unordered_map<Position, uint64_t, Position::Hash> memo;

// Recursive function to calculate timelines
uint64_t countTimelines(const std::vector<std::string>& grid, size_t x, size_t y)
{
    // Base case: if the position is out of bounds (reached bottom)
    if (y >= grid.size())
    {
        return 1; // Reached the end, count as 1 timeline
    }

    assert(x >= 0 && x < grid[y].size() && "out of bounds x");

    Position currentPos = { x, y };

    // Check if already computed
    if (memo.find(currentPos) != memo.end())
    {
        return memo[currentPos];
    }

    uint64_t result = 0;

    // Recursive case: splitter found
    if (grid[y][x] == '^')
    {
        uint64_t leftTimelines = 0;
        uint64_t rightTimelines = 0;

        // Left branch
        if (x > 0 && grid[y][x - 1] != ' ')
        {
            leftTimelines = countTimelines(grid, x - 1, y + 1);
        }

        // Right branch
        if (x + 1 < grid[y].size() && grid[y][x + 1] != ' ')
        {
            rightTimelines = countTimelines(grid, x + 1, y + 1);
        }

        result = leftTimelines + rightTimelines;
    }
    else
    {
        // Default case: continue moving down
        result = countTimelines(grid, x, y + 1);
    }

    // Store in memoization map
    memo[currentPos] = result;
    return result;
}

//Part 1
uint64_t CountBeams(std::vector<Position>& tachyonBeamStartPos, std::vector<std::string>& lines)
{
    int numSplits = 0;
    while (!tachyonBeamStartPos.empty())
    {
        Position startPos = tachyonBeamStartPos.back();
        tachyonBeamStartPos.pop_back(); ////why does pop_back return void??
        size_t x = startPos.x;
        size_t y = startPos.y;
        while (y < lines.size())
        {
            char currentChar = lines[y][x];
            if (currentChar == '|')
            {
                //beam already passed here, we stop this beam
                break;
            }
            if (currentChar == '^')
            {
                numSplits++;
                //splitter found, we stop this beam here and start new beams to the left and right
                if (x > 0 && lines[y][x - 1] == '.') //left beam
                {
                    tachyonBeamStartPos.push_back({ x - 1, y });
                }
                if (x + 1 < lines[y].size() && lines[y][x + 1] == '.') //right beam
                {
                    tachyonBeamStartPos.push_back({ x + 1, y });
                }
                break;
            }
            else
            {
                //mark the beam path
                assert(currentChar == '.');
                lines[y][x] = '|'; //mark the path of the beam
                ++y; //move down
            }
        }
    }

    //print the final state of the grid
    for (const auto& line : lines)
    {
        std::cout << line << std::endl;
    }
	return numSplits;
}

int main()
{
	auto lines = readFileLines("Data\\Input.txt");

	std::vector<Position> tachyonBeamStartPos;  //after each split we store the starting position of the new beams here

	// find the starting position of the packet (marked by 'S')
    for (size_t iLine = 0; iLine < lines.size(); ++iLine)
    {
        const std::string& currLine = lines[iLine];
        for (size_t iChar = 0; iChar < currLine.size(); ++iChar)
        {
            if (currLine[iChar] == 'S')
            {
				tachyonBeamStartPos.push_back({ iChar, iLine+1 });
            }
        }
	}

	uint64_t part2Result = countTimelines(lines, tachyonBeamStartPos[0].x, tachyonBeamStartPos[0].y);
	std::cout << "Part 2 - Number of possible timelines: " << part2Result << std::endl;

	//now we iterate over all starting positions and simulate the beam (going straight until we hit a splitter '^')
    uint64_t numSplits = CountBeams(tachyonBeamStartPos, lines);
    std::cout << "Part 1 - Number of splits: " << numSplits << std::endl;
	return 0;
}



// Day7.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://adventofcode.com/2025/day/7

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <assert.h>

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
};

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

	//now we iterate over all starting positions and simulate the beam (going straight until we hit a splitter '^')
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
    std::cout << "Number of splits: " << numSplits << std::endl;
	return 0;
}

// Day4.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://adventofcode.com/2025/day/4

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <assert.h>

constexpr bool IS_PART2 = true;

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

void addBorder(std::vector<std::string>& inputLines)
{
    size_t width = inputLines[0].size();

    // Add border characters to each existing line
    for (auto& line : inputLines)
    {
        line = '.' + line + '.';
        line.shrink_to_fit();
    }

    // Add top and bottom border lines
    std::string borderLine(width + 2, '.');
    inputLines.insert(inputLines.begin(), borderLine);
    inputLines.push_back(borderLine);
	inputLines.shrink_to_fit();
}

unsigned FindNumberOfReachableRolls(std::vector<std::string>& inputLines, bool removeRolls = true)
{
    unsigned numReachableRolls = 0;
	constexpr int THRESHOLD = 4 * '@' + 4 * '.'; // 4*64 + 4*46 = 440, we sum the ASCII values of the neighbours, if the sum is less than this threshold, at least 4 of the 8 neighbours are '.'

    // Loop from 1 to size-1 to skip the border
    for (size_t iLine = 1; iLine < inputLines.size() - 1; ++iLine)
    {
        const std::string& prevLine = inputLines[iLine - 1];
        const std::string& currLine = inputLines[iLine];
        const std::string& nextLine = inputLines[iLine + 1];
        
        for (size_t iChar = 1; iChar < currLine.size() - 1; ++iChar)
        {
            if (currLine[iChar] == '@')
            {
                const int neighbourSum =
                    prevLine[iChar - 1] + prevLine[iChar] + prevLine[iChar + 1] +
                    currLine[iChar - 1]                   + currLine[iChar + 1] +
                    nextLine[iChar - 1] + nextLine[iChar] + nextLine[iChar + 1];

                if (neighbourSum < THRESHOLD)
                {
                    if (removeRolls)
                        inputLines[iLine][iChar] = '.';
                    numReachableRolls++;
                }
            }
        }
    }
    return numReachableRolls;
}

int main()
{
    //auto start = std::chrono::high_resolution_clock::now();
    auto inputLines = readFileLines("Data\\Input.txt");
	//we add a border of '.' around the input to avoid checking for out of bounds
    addBorder(inputLines);

    unsigned totalReachableRolls = 0;

    unsigned numReachableRolls = FindNumberOfReachableRolls(inputLines, IS_PART2);
    totalReachableRolls += numReachableRolls;

    if (IS_PART2)
    {
        while (numReachableRolls > 0)
        {
            numReachableRolls = FindNumberOfReachableRolls(inputLines, true);
            totalReachableRolls += numReachableRolls;
        }
    }

    std::cout << "Number of reachable rolls: " << totalReachableRolls << "\n";

    //auto end = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> elapsed = end - start;
    //std::cout << "Execution time: " << elapsed.count() << " seconds\n";
}
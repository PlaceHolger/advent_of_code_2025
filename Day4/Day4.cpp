// Day4.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://adventofcode.com/2025/day/4

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <assert.h>

constexpr bool IS_PART2 = false;

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

int FindNumberOfReachableRolls(std::vector<std::string>& inputLines, bool removeRolls = true)
{
    unsigned numReachableRolls = 0;

    // okay, we want to find in everyline the '@' that by less than 4 adjacent '@' characters
    // our first approach is to loop over every character in the line, and for every '@' we find, we check the 8 adjacent characters (and keep the border cases in mind)
    for (int iLine = 0; iLine < inputLines.size(); ++iLine)
    {
        for (int iChar = 0; iChar < inputLines[iLine].size(); ++iChar)
        {
            if (inputLines[iLine][iChar] == '@')
            {
                int adjacentAtCount = 0;
                //check the 8 adjacent characters
                for (int deltaLine = -1; deltaLine <= 1; ++deltaLine)
                {
                    for (int deltaChar = -1; deltaChar <= 1; ++deltaChar)
                    {
                        //skip the center character
                        if (deltaLine == 0 && deltaChar == 0)
                            continue;
                        int checkLine = iLine + deltaLine;
                        int checkChar = iChar + deltaChar;
                        //check bounds
                        if (checkLine >= 0 && checkLine < inputLines.size() &&
                            checkChar >= 0 && checkChar < inputLines[checkLine].size())
                        {
                            if (inputLines[checkLine][checkChar] == '@')
                            {
                                ++adjacentAtCount;
                            }
                        }
                    }
                }
                if (adjacentAtCount < 4)
                {
                    if(removeRolls)
						inputLines[iLine][iChar] = 'x';  // mark as removed when the part was reachable
                    //std::cout << "Found '@' at line " << iLine << ", char " << iChar << " with only " << adjacentAtCount << " adjacent '@' characters.\n";
                    numReachableRolls++;
                }
            }
        }
    }
    return numReachableRolls;
}

int main()
{
    auto inputLines = readFileLines("Data\\Input.txt");

	int totalReachableRolls = 0;

    int numReachableRolls = FindNumberOfReachableRolls(inputLines, IS_PART2);
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
}
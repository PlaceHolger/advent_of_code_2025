// Day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://adventofcode.com/2025/day/3

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

int main()
{
    uint64_t sumOfNumbers = 0;
    constexpr int numDigits = (IS_PART2) ? 12 : 2;

	auto lines = readFileLines("Data\\Input.txt");
    for (const auto& line : lines)
    {
		//we are trying to find the the largest number with "numDigits" digits that appears in the line, without changing the order of numbers
        //for this we want to loop over the input digit string and we are looking for the largest number in the first x numbers, where x it NumOfDigits - "numDigits"
        size_t currentStartPos = 0;
        std::string solution;

        for (size_t currentDigitPos = 0; currentDigitPos < numDigits; ++currentDigitPos)
        {
            char largestCurrentDigit = '0';
            for (size_t i = currentStartPos; i < line.size() - numDigits + currentDigitPos + 1; ++i)
            {
                if (line[i] > largestCurrentDigit)
                {
                    currentStartPos = i + 1;
                    largestCurrentDigit = line[i];
                    //if we found a 9, we cannot do better, so we can stop searching
                    if (largestCurrentDigit == '9')
                        break;
                }
            }
            solution += largestCurrentDigit;
        }

        std::cout << "Line: " << line << " -> largest " << numDigits << "digit number: " << solution << "\n";
        assert(sumOfNumbers < UINT64_MAX - std::stoull(solution));
        //add the found number to our sum
        sumOfNumbers += std::stoull(solution);
    }
    std::cout << "Sum: " << sumOfNumbers;
	return 0;
}


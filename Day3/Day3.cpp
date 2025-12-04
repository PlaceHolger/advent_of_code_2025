// Day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

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
	auto lines = readFileLines("Data\\Input.txt");
    for (const auto& line : lines)
    {
		//we are trying to find the the largest two digit number that appears in the line
		//lets try to first find the largest digit (starting from left going to end-1), and from that position we can look for the next largest digit in the rest of the line
		char largestFirstDigit = '0';
		int positionOfLargestFirstDigit = -1;
        for (int i = 0; i < line.size() - 1; ++i)
        {
            if (line[i] > largestFirstDigit)
            {
                positionOfLargestFirstDigit = i;
                largestFirstDigit = line[i];
                //if we found a 9, we cannot do better, so we can stop searching
                if (largestFirstDigit == '9')
                    break;
            }
        }
		//now we have the largest first digit, we can look for the largest second digit in the rest of the line
        char largestSecondDigit = '0';
        for (int j = positionOfLargestFirstDigit + 1; j < line.size(); ++j)
        {
            if (line[j] > largestSecondDigit)
            {
                largestSecondDigit = line[j];
                //if we found a 9, we cannot do better, so we can stop searching
                if (largestSecondDigit == '9')
                    break;
            }
		}
		std::cout << "Line: " << line << " -> Largest two digit number: " << largestFirstDigit << largestSecondDigit << std::endl;
        //add the found number to our sum
        sumOfNumbers += (largestFirstDigit - '0') * 10 + (largestSecondDigit - '0');
    }
    std::cout << "Sum: " << sumOfNumbers;
	return 0;
}


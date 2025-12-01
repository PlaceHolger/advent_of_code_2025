// AdventOfCodeDay1.cpp : This file contains the 'main' function. Program execution begins and ends there.
// task: https://adventofcode.com/2025/day/1

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define PART_2

std::vector<int> readFileToVector(const std::string& filename) 
{
    std::vector<int> result;
    std::ifstream file(filename);
    if (!file) 
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return result;
    }
    std::string line;
    while (std::getline(file, line)) 
    {
        if (line.empty()) continue;
        char prefix = line[0];
        int value = std::stoi(line.substr(1));
        if (prefix == 'L') 
        {
            value = -value;
        }
        else if (prefix != 'R') 
        {
            // Invalid prefix, skip this line
			std::cerr << "Invalid line prefix: " << prefix << std::endl;
            continue;
        }
        result.push_back(value);
    }
    return result;
}

int main()
{
    std::vector<int> inputLines = readFileToVector("data\\Input.txt");
	int currentValue = 50; // starting value

#if !defined(PART_2)
	unsigned int numTimesZeroReached = 0;
    for (int change : inputLines) 
    {
		currentValue += change;
		if (currentValue % 100 == 0)
        {
            numTimesZeroReached++;
        }
    }
	std::cout << "Final Value: " << numTimesZeroReached << std::endl;
#else
    unsigned int totalZeros = 0;
    for (int change : inputLines)
    {
		totalZeros += std::abs(change) / 100; // Count how many full hundreds are crossed
        int remainder = change % 100;
        currentValue += remainder;

		if (currentValue >= 100) //check if we crossed zero going up
        {
            if (currentValue - remainder != 0) // Avoid double counting if we landed exactly on zero last time
                totalZeros++;
            currentValue -= 100;
        }
		else if (currentValue <= 0) //check if we crossed zero going down
        {
			if (currentValue - remainder != 0) // Avoid double counting if we landed exactly on zero last time
                totalZeros++;            
            if(currentValue < 0)
                currentValue += 100;
		}
        //std::cout << "The dial is rotated " << change << " to point at " << currentValue << ", Total Zeros: " << totalZeros << std::endl;
    }
    std::cout << "Final Value: " << totalZeros << std::endl;
#endif
	return 0;
}
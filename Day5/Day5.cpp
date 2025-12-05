// Day5.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://adventofcode.com/2025/day/5

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <assert.h>
#include <algorithm>

struct Range {
    uint64_t start, end;
};

struct InputData {
    std::vector<Range> ranges;
    std::vector<uint64_t> ingredientIDs;
};


InputData readInputFile(const std::string& filename)
{
    InputData data;
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return data;
    }

    std::string line;

    // First loop: Read ranges until a blank line is encountered
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            // Blank line indicates the end of ranges
            break;
        }

        uint64_t start, end;
        // Use sscanf to parse the range in "start-end" format
        if (sscanf_s(line.c_str(), "%llu-%llu", &start, &end) == 2)
        {
            data.ranges.push_back({ start, end });
        }
        else
        {
            std::cerr << "Invalid range line: \"" << line << "\"\n";
        }
    }

    // Second loop: Read single numbers (ingredientIDs) after the blank line
    while (std::getline(file, line))
    {
        uint64_t value;
        // Use sscanf to parse the single number
        if (sscanf_s(line.c_str(), "%llu", &value) == 1)
        {
            data.ingredientIDs.push_back(value);
        }
        else
        {
            std::cerr << "Invalid number line: \"" << line << "\"\n";
        }
    }

    return data;
}

void optimizeRanges(std::vector<Range>& ranges)
{
    // we want to reduce the number of ranges by finding overlaps

    if (ranges.empty())
        return;
    // First, sort the ranges by their start values
    std::sort(ranges.begin(), ranges.end(), [](const Range& a, const Range& b) {
        return a.start < b.start;
    });
    std::vector<Range> optimized;
    optimized.push_back(ranges[0]);
    for (size_t i = 1; i < ranges.size(); ++i)
    {
        Range& last = optimized.back();
        if (ranges[i].start < last.end) // Overlapping or contiguous ranges
        {
            last.end = std::max(last.end, ranges[i].end);
        }
        else
        {
            optimized.push_back(ranges[i]);
        }
    }
    ranges = std::move(optimized);
}

int main()
{
	auto inputData = readInputFile("Data\\Input.txt");
	
    optimizeRanges(inputData.ranges);

    unsigned numFreshIngredients = 0;
    //now we just iterate over all ingredients and check if these are in one of the ranges
    for (const auto& ingredientID : inputData.ingredientIDs)
    {
        for (const auto& range : inputData.ranges)
        {
            if (ingredientID <= range.end)
            {
                if (ingredientID >= range.start)
                    ++numFreshIngredients;
				break; // No need to check further ranges, since ranges are sorted
            }
        }
    }

	std::cout << "Number of fresh ingredients: " << numFreshIngredients << "\n";

	return 0;
}

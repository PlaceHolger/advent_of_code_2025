// Day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://adventofcode.com/2025/day/2

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <assert.h>

//#define PART_2 1

struct Range {
    uint64_t start, end;
};

constexpr size_t MAX_DIGITS = 19;  // Covers uint64_t range
constexpr uint64_t pow10[MAX_DIGITS] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL
};

int digit_count(uint64_t n) {
    return 1 + (n >= 10) + (n >= 100) + (n >= 1000) + (n >= 10000) +
        (n >= 100000) + (n >= 1000000) + (n >= 10000000) +
        (n >= 100000000) + (n >= 1000000000) +
        (n >= 10000000000ULL) + (n >= 100000000000ULL) +
        (n >= 1000000000000ULL) + (n >= 10000000000000ULL) +
        (n >= 100000000000000ULL) + (n >= 1000000000000000ULL) +
        (n >= 10000000000000000ULL) + (n >= 100000000000000000ULL);
}

Range adjust_to_even_digits(const uint64_t orig_start, const uint64_t orig_end)
{
    uint64_t new_start = orig_start;
    uint64_t new_end = orig_end;

    // Adjust start to next even-digit number if it has odd digits
    int start_digits = digit_count(new_start);
    if (start_digits % 2 == 1)
    {
        new_start = pow10[start_digits];  // Jump to first number with (start_digits + 1) digits
        if (new_start > new_end)
            return { 0, 0 };
    }

    // Adjust end to previous even-digit number if it has odd digits
    int end_digits = digit_count(new_end);
    if (end_digits % 2 == 1)
    {
        new_end = pow10[end_digits - 1] - 1;  // Jump to last number with (end_digits - 1) digits
        if (new_end < new_start)
            return { 0, 0 };
    }

    return { new_start, new_end };
}

std::vector<Range> readRangesFromFile(const std::string& filename)
{
    std::vector<Range> ranges;
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return ranges;
    }

    std::string line;
    if (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string range;
        while (std::getline(ss, range, ','))
        {
            size_t dashPos = range.find('-');
            if (dashPos != std::string::npos)
            {
                uint64_t start = std::stoull(range.substr(0, dashPos));
                uint64_t end = std::stoull(range.substr(dashPos + 1));
                ranges.push_back({ start, end });
            }
        }
    }

	// new we want to check the ranges are valid, since we are looking for 2 times the same pattern inside the ranges we are only interested in even digit numbers
    std::vector<Range> adjustedRanges;
    for (auto& range : ranges)
    {
#if !defined(PART_2)
        auto adjustedRange = adjust_to_even_digits(range.start, range.end);
#else
        // in part 2 we also need uneven counts, but we want to remove single digit numbers
        auto adjustedRange = range;
        if (adjustedRange.start < 10 && adjustedRange.end > 10)
            adjustedRange.start = 10; // minimum 2 digit number
#endif
		if (adjustedRange.start >= 10) // minimum 2 digit number
            adjustedRanges.push_back(adjustedRange);
    }

    return adjustedRanges;
}


#if !defined(PART_2)
//checks if the given number has the pattern where the first half of the digits equals the second half
bool has_repeated_pattern(const uint64_t num) 
{
    const int numDigits = digit_count(num);    
	assert(numDigits % 2 == 0 && "our range should only include even numbers");

    const int half_digits = numDigits / 2;
    const uint64_t half_divisor = pow10[half_digits];  // Direct lookup!

    const uint64_t left = num / half_divisor;
    const uint64_t right = num % half_divisor;
    return left == right;
}
#else
bool is_repeating_pattern(const uint64_t num)
{
    const int total_digits = digit_count(num);
    assert(total_digits > 1 && "our range should only include numbers higher than 10");

    // Try all possible pattern lengths that divide total_digits
    for (int pattern_len = 1; pattern_len < total_digits; ++pattern_len)
    {
        if (total_digits % pattern_len != 0)
            continue;  // Pattern length must divide evenly

        // Extract the pattern (first pattern_len digits)
        uint64_t pattern = num / pow10[total_digits - pattern_len];

        // Check if repeating this pattern gives the original number
        uint64_t reconstructed = 0;
        const int repetitions = total_digits / pattern_len;

        for (int i = 0; i < repetitions; ++i)
        {
            reconstructed = reconstructed * pow10[pattern_len] + pattern;
        }

        if (reconstructed == num)
            return true;
    }

    return false;
}
#endif

int main()
{
	uint64_t sumOfAllFoundNumbers = 0;
    std::vector<Range> ranges = readRangesFromFile("Data\\Input.txt");

    std::cout << "Read " << ranges.size() << " ranges:\n";
    for (auto& curRange : ranges)
    {
		// now we go over each number and check if it has the pattern (first half == second half)
		std::vector<uint64_t> foundNumbers;
        for (uint64_t num = curRange.start; num <= curRange.end; ++num)
        {
#if !defined(PART_2)
            if (has_repeated_pattern(num))
#else
			if (is_repeating_pattern(num))
#endif
            {
                foundNumbers.push_back(num);
				sumOfAllFoundNumbers += num;
            }
        }
        std::cout << "Range " << curRange.start << "-" << curRange.end << " has " << foundNumbers.size() << " numbers with repeated patterns.\n";
        for (auto number : foundNumbers)
        {
            std::cout << number << "\n";
		}                
    }
	std::cout << "Sum of all found numbers: " << sumOfAllFoundNumbers << std::endl;
}
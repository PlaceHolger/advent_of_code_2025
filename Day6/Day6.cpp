// Day6.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://adventofcode.com/2025/day/6

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <assert.h>

#define PART2 true

struct Problem
{
    std::vector<unsigned int> numbers;
    char operation = 'X';
};


#if !defined(PART2)
std::vector<Problem> parseProblems(const std::string& filename)
{
    std::vector<Problem> problems;
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return problems;
    }

    std::string line;
    std::vector<std::vector<unsigned int>> rows;
    std::vector<char> operations;

    // Read all lines
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        if (line[0] != '+' && line[0] != '*') // Check if the line starts with a number
        {
            std::vector<unsigned int> row;
            unsigned int num;
            while (iss >> num)
            {
                row.push_back(num);
            }
            rows.push_back(row);
        }
        else // Last row contains operations
        {
            char op;
            while (iss >> op)
            {
				assert(op == '+' || op == '*', "Invalid operation character");
                operations.push_back(op);
            }
        }
    }

    problems.resize(rows[0].size()); //each column is a problem
    // Combine rows and operations into Problems 
    for (size_t col = 0; col < rows[0].size(); ++col)
    {
        Problem prob;
        for (size_t row = 0; row < rows.size(); ++row)
        {
            prob.numbers.push_back(rows[row][col]);
        }
        prob.operation = operations[col];
        problems[col] = prob;
    }

    return problems;
}
#else
std::vector<Problem> parseProblems(const std::string& filename)
{
	//the parsing here is special, every column is a number, Whenever we find a column where every line has ' ' we have a new problem (seperator column)
    std::vector<Problem> problems;

    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return problems;
    }

    // Read all lines
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
        lines.push_back(line);
    if (lines.size() < 2)
        return problems;

	//check all lines have the same length
    size_t lineLength = lines[0].size();
    for (const auto& l : lines)
        assert(l.size() == lineLength && "All lines must have the same length");

	//now we split the lines into problems, whenever we find a column where every line has ' ' we have a new problem    
	Problem currentProblem;
    std::string currentColumnNumber;
    for (size_t col = 0; col < lines[0].size(); ++col)
    {
        bool isSeparator = true;
        for (size_t row = 0; row < lines.size(); ++row)
        {
            if (lines[row][col] != ' ')
            {
                isSeparator = false;
                break;
            }
        }
        if (isSeparator)
        {
            assert(!currentProblem.numbers.empty() && currentProblem.operation != 'X' && "Problem cannot be empty");
            // Add a new problem directly to the vector
            problems.push_back(std::move(currentProblem));
            // Start a new problem
            currentProblem = Problem(); // Reset currentProblem to a default state
            continue;
        }
        //not a separator, add numbers to current problem
        currentColumnNumber.clear();
        for (size_t row = 0; row < lines.size() - 1; ++row) //last line is operations
        {
            if (std::isdigit(lines[row][col]))
            {
                currentColumnNumber += lines[row][col];
            }
        }
		//convert to number and add to current problem
		currentProblem.numbers.push_back(std::stoul(currentColumnNumber));

        //add operation from last line
        char op = lines[lines.size() - 1][col];
        if(op == '*' || op == '+')
            currentProblem.operation = op;
    }
	problems.push_back(currentProblem); //add last problem

	return problems;

}
#endif


int main()
{
	auto problems = parseProblems("Data\\Input.txt");

	uint64_t totalSum = 0;
    for (const auto& prob : problems)
    {
        unsigned long long result = (prob.operation == '+') ? 0 : 1;
        for (const auto& num : prob.numbers)
        {
            if (prob.operation == '+')
                result += num;
            else if (prob.operation == '*')
                result *= num;
        }
        //std::cout << "Result of operation " << prob.operation << " on numbers: " << result << std::endl;
		totalSum += result;
	}
	std::cout << "Total sum of all results: " << totalSum << std::endl;
}

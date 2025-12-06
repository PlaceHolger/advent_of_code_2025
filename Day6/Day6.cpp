// Day6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <assert.h>

struct Problem
{
    std::vector<unsigned int> numbers;
    char operation;
};

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

    // Combine rows and operations into Problems 
	problems.resize(rows[0].size()); //each column is a problem
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

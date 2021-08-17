#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <string>

std::function<void(const std::string&)> Log;

std::list<std::function<bool()>>* tests = nullptr;
std::function<bool()> AddTest(const std::string& functionName, std::function<void()> testFunction)
{
	if (!tests)
	{
		tests = new std::list<std::function<bool()>>();
	}
	tests->push_back(
		[functionName, testFunction]()
		{
			Log(std::format("Started Test: {}", functionName));
			try
			{
				testFunction();
				Log(std::format("Test Passed: {}", functionName));
				return true;
			}
			catch (...)
			{
				Log(std::format("Test Failed: {}", functionName));
				return false;
			}
		});
	return tests->back();
}

int main()
{
	std::ofstream output;
	output.open("results.txt");
	Log = [&output](const std::string& message) 
	{
		output << message << std::endl;
	};
	size_t successCount = 0;
	size_t failCount = 0;
	if (tests)
	{
		for (auto test : *tests)
		{
			if (test())
			{
				successCount++;
				continue;
			}
			failCount++;
		}
		delete tests;
		tests = nullptr;
	}
	output << std::format("Total Tests: {}", successCount + failCount) << std::endl;
	output << std::format("Total Passed: {}", successCount) << std::endl;
	output << std::format("Total Failed: {}", failCount) << std::endl;
	output.close();
}

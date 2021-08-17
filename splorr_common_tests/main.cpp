#include "Common.RNG.Tests.h"
#include <format>
#include <functional>
#include <iostream>
#include <list>
#include <string>

std::function<void(const std::string&)> Log = [](const std::string& info) 
{
	std::cout << info << std::endl;
};

std::list<std::function<void()>>* tests = nullptr;
std::function<void()> AddTest(const std::string& functionName, std::function<void()> testFunction)
{
	if (!tests)
	{
		tests = new std::list<std::function<void()>>();
	}
	tests->push_back(
		[functionName, testFunction]()
		{
			Log(std::format("Started Test: {}", functionName));
			try
			{
				testFunction();
				Log(std::format("Test Passed: {}", functionName));
			}
			catch (...)
			{
				Log(std::format("Test Failed: {}", functionName));
			}
		});
	return tests->back();
}

int main()
{
	if (tests)
	{
		for (auto test : *tests)
		{
			test();
		}
		delete tests;
		tests = nullptr;
	}
}

#pragma once
#include <unordered_map>
#include <string>
#include <fstream>
#include <exception>
#include <regex>


class IniFile
{
	std::string filePath;
	std::unordered_map<std::string, std::string> container;

	std::ifstream iFile;
public:
	IniFile(const char * _filePath) : filePath(_filePath)
	{
		// ^([A-Z|a-z][^\s]*)\s*=\s*(.*)$
		std::string rawRegex("^([A-Z|a-z][^\\s]*)\\s*=\\s*(.*)$");
		
		std::regex regex(rawRegex);
		

		iFile.open(filePath);
		if (!iFile.is_open())
			throw std::invalid_argument("Can't open file at given location");

		int lineNumber = 0;
		while (!iFile.eof())
		{
			std::string line;
			std::smatch matches;

			std::getline(iFile, line);
			lineNumber++;
			if (std::regex_match(line, matches, regex))
			{
				if (container.find(matches[1].str()) != container.end())
					throw std::logic_error("Value " + matches[1].str() + " already exists!");
				container[matches[1].str()] = matches[2].str();
			}
			else if (line[0] != '#')
			{
				std::cerr << "[IniFile] Cannot parse " << lineNumber << " line in file " << filePath << "\n";
			}
		}
		iFile.close();
	}

	std::string get(std::string key)
	{
		if (container.find(key) == container.end())
			throw std::logic_error("Value " + key + " not found!");
		return container[key];
	}

	bool getBool(std::string key)
	{
		if (container.find(key) == container.end())
			throw std::logic_error("Value " + key + " not found!");
		std::string value(container[key]);
		if (std::regex_search(value, std::regex("^((true)|(1))$", std::regex::icase)))
			return true;
		if (std::regex_search(value, std::regex("^((false)|(-1)|(0))$", std::regex::icase)))
			return false;
		throw std::logic_error(key + " is not boolean");
	}

	int getInt(std::string key)
	{
		return std::stoi(container[key]);
	}
};
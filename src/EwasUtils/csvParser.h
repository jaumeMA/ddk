#pragma once

#include <istream>
#include <string>
#include <vector>

enum class CSVState {
	UnquotedField,
	QuotedField,
	QuotedQuote
};

class csvParser
{
	public:

		// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes"""
		static std::vector<std::vector<std::string>> readCSV(std::istream &in);

	private:

		// Read CSV line, Excel dialect. Accept "quoted fields ""with quotes"""
		static std::vector<std::string> readCSVRow(const std::string &row);

		// Skips the Byte Order Mark (BOM) that defines UTF-8 in some text files.
		static void SkipBOM(std::istream &in);
};

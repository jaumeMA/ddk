#include "csvParser.h"

std::vector<std::string> csvParser::readCSVRow(const std::string &row) 
{
	CSVState state = CSVState::UnquotedField;
	std::vector<std::string> fields{""};

	size_t i = 0; // index of the current field
	for (char c : row) 
	{
		switch (state) 
		{
			case CSVState::UnquotedField:
				switch (c) 
				{
					case ',': // end of field
					case ';': // end of field
						fields.push_back(""); i++;
						break;
					case '"': state = CSVState::QuotedField;
						break;
					default:  fields[i].push_back(c);
						break;
				}
				break;
			case CSVState::QuotedField:
				switch (c) 
				{
					case '"': state = CSVState::QuotedQuote;
						break;
					default:  fields[i].push_back(c);
						break;
				}
				break;
			case CSVState::QuotedQuote:
				switch (c) 
				{
					case ',': // , after closing quote
					case ';': // end of field
						fields.push_back(""); i++;
						state = CSVState::UnquotedField;
						break;
					case '"': // "" -> "
						fields[i].push_back('"');
						state = CSVState::QuotedField;
						break;
					default:  // end of quote
						state = CSVState::UnquotedField;
						break;
				}
				break;
		}
	}
	return fields;
}

// Skips the Byte Order Mark (BOM) that defines UTF-8 in some text files.
void csvParser::SkipBOM(std::istream &in)
{
	char test[3] = { 0 };
	in.read(test, 3);
	if ((unsigned char)test[0] == 0xEF &&
		(unsigned char)test[1] == 0xBB &&
		(unsigned char)test[2] == 0xBF)
	{
		return;
	}
	in.seekg(0);
}

// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes"""
std::vector<std::vector<std::string>> csvParser::readCSV(std::istream &in) 
{
	std::vector<std::vector<std::string>> table;
	std::string row;

	SkipBOM(in);

	while (std::getline(in, row))
	{
		if (in.bad() || in.fail()) 
		{
			break;
		}
		std::vector<std::string> fields = readCSVRow(row);
		table.push_back(fields);
	}

	return table;
}
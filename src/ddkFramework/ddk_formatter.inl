
namespace ddk
{

template<typename T>
T formatter<T>::format(const std::string& i_str)
{
	T res;

	res << i_str;

	return res;
}

template<typename T>
std::string formatter<std::string>::format(const T& i_val)
{
	std::ostringstream convert;

	convert << i_val;

	return convert.str();
}
template<typename T>
std::string formatter<std::string>::format(const std::set<T>& i_val)
{
	std::ostringstream convert;
	bool firstItem = true;

	convert << '[';

	for(const auto& value : i_val)
	{
		if(firstItem == false)
		{
			convert << ",";
		}

		convert << format(value);
		firstItem = false;
	}

	convert << ']';

	return convert.str();
}
template<typename T>
std::string formatter<std::string>::format(const std::vector<T>& i_val)
{
	std::ostringstream convert;
	bool firstItem = true;

	convert << '[';

	for(const auto& value : i_val)
	{
		if(firstItem == false)
		{
			convert << ",";
		}

		convert << format(value);
		firstItem = false;
	}

	convert << ']';

	return convert.str();
}
template<typename T>
std::string formatter<std::string>::format(const std::list<T>& i_val)
{
	std::ostringstream convert;
	bool firstItem = true;

	convert << '[';

	for(const auto& value : i_val)
	{
		if(firstItem == false)
		{
			convert << ",";
		}

		convert << format(value);
		firstItem = false;
	}

	convert << ']';

	return convert.str();
}
template<typename First,typename Second>
std::string formatter<std::string>::format(const std::pair<First,Second>& i_val)
{
	std::ostringstream convert;

	convert << '<' << format(i_val.first) << ":" << format(i_val.second) << '>';

	return convert.str();
}
template<typename Key,typename Value>
std::string formatter<std::string>::format(const std::map<Key,Value>& i_val)
{
	std::ostringstream convert;
	bool firstItem = true;

	convert << '[';

	for(const auto& keyValuePair : i_val)
	{
		if(firstItem == false)
		{
			convert << ";";
		}

		convert << format(keyValuePair);
		firstItem = false;
	}

	convert << ']';

	return convert.str();
}


template<typename First,typename Second>
std::pair<First,Second> formatter<std::pair<First,Second>>::format(std::string i_str)
{
	std::pair<First,Second> res;

	if(i_str.empty() == false)
	{
		if(i_str[0] == '<' && i_str[i_str.size() - 1] == '>')
		{
			i_str = i_str.substr(1,i_str.size() - 2);

			size_t separatorPos = i_str.find(":");
			if(separatorPos != std::string::npos && separatorPos < i_str.size() - 1)
			{
				res.first = formatter<First>::format(i_str.substr(0,separatorPos));
				res.second = formatter<Second>::format(i_str.substr(separatorPos + 1));
			}
			else
			{
				DDK_FAIL("Expected separator not present");
			}
		}
		else
		{
			DDK_FAIL("Expected transport not present");
		}
	}

	return res;
}

template<typename T>
std::vector<T> formatter<std::vector<T>>::format(std::string i_str)
{
	std::vector<T> res;

	if(i_str.empty() == false)
	{
		if(i_str[0] == '[' && i_str[i_str.size() - 1] == ']')
		{
			i_str = i_str.substr(1,i_str.size() - 2);

			do
			{
				const size_t separatorPos = i_str.find(",");
				const std::string leftStr = i_str.substr(0,separatorPos);

				i_str = (separatorPos != std::string::npos) ? i_str.substr(separatorPos + 1) : "";

				res.push_back(formatter<T>::format(leftStr));
			} while(i_str.empty() == false);
		}
		else
		{
			DDK_FAIL("Expected transport not present");
		}
	}

	return res;
}

template<typename T>
std::list<T> formatter<std::list<T>>::format(std::string i_str)
{
	std::list<T> res;

	if(i_str.empty() == false)
	{
		if(i_str[0] == '[' && i_str[i_str.size() - 1] == ']')
		{
			i_str = i_str.substr(1,i_str.size() - 2);

			do
			{
				const size_t separatorPos = i_str.find(",");
				const std::string leftStr = i_str.substr(0,separatorPos);

				i_str = (separatorPos != std::string::npos) ? i_str.substr(separatorPos + 1) : "";

				res.push_back(formatter<T>::format(leftStr));
			} while(i_str.empty() == false);
		}
		else
		{
			DDK_FAIL("Expected transport not present");
		}
	}

	return res;
}

template<typename T>
std::set<T> formatter<std::set<T>>::format(std::string i_str)
{
	std::set<T> res;

	if(i_str.empty() == false)
	{
		if(i_str[0] == '[' && i_str[i_str.size() - 1] == ']')
		{
			i_str = i_str.substr(1,i_str.size() - 2);

			do
			{
				const size_t separatorPos = i_str.find(",");
				const std::string leftStr = i_str.substr(0,separatorPos);

				i_str = (separatorPos != std::string::npos) ? i_str.substr(separatorPos + 1) : "";

				res.insert(formatter<T>::format(leftStr));
			} while(i_str.empty() == false);
		}
		else
		{
			DDK_FAIL("Expected transport not present");
		}
	}

	return res;
}

template<typename Key, typename Value>
std::map<Key,Value> formatter<std::map<Key,Value>>::format(std::string i_str)
{
	std::map<Key,Value> res;

	if(i_str.empty() == false)
	{
		if(i_str[0] == '[' && i_str[i_str.size() - 1] == ']')
		{
			i_str = i_str.substr(1,i_str.size() - 2);

			do
			{
				const size_t separatorPos = i_str.find(";");
				const std::string particleStr = i_str.substr(0,separatorPos);

				i_str = (separatorPos != std::string::npos) ? i_str.substr(separatorPos + 1) : "";

				res.insert(formatter<std::pair<Key,Value>>::format(particleStr));
			} while(i_str.empty() == false);
		}
		else
		{
			DDK_FAIL("Expected transport not present");
		}
	}

	return res;
}

template<typename T>
T format_to(const std::string& i_value)
{
	return formatter<T>::format(i_value);
}

template<typename T>
std::string to_string(T&& i_value)
{
	return formatter<std::string>::format(i_value);
}

template<typename T, typename ... TT>
std::string to_string(T&& i_value, TT&& ... i_values)
{
	std::string res = ddk::to_string(i_value);
	const std::string valueStr[mpl::num_types<TT...>] = { ddk::to_string(i_values) ... };

	for(size_t index=0;index < mpl::num_types<TT...>;index++)
	{
		res += " " + valueStr[index];
	}

	return res;
}

}
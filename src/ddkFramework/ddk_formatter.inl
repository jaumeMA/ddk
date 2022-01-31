
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

	for(const auto& value : i_val)
	{
		if(firstItem == false)
		{
			convert << ",";
		}

		convert << format(value);
		firstItem = false;
	}

	return convert.str();
}
template<typename T>
std::string formatter<std::string>::format(const std::vector<T>& i_val)
{
	std::ostringstream convert;
	bool firstItem = true;

	for(const auto& value : i_val)
	{
		if(firstItem == false)
		{
			convert << ",";
		}

		convert << format(value);
		firstItem = false;
	}

	return convert.str();
}
template<typename T>
std::string formatter<std::string>::format(const std::list<T>& i_val)
{
	std::ostringstream convert;
	bool firstItem = true;

	for(const auto& value : i_val)
	{
		if(firstItem == false)
		{
			convert << ",";
		}

		convert << format(value);
		firstItem = false;
	}

	return convert.str();
}
template<typename First,typename Second>
std::string formatter<std::string>::format(const std::pair<First,Second>& i_val)
{
	std::ostringstream convert;

	convert << format(i_val.first) << ":" << format(i_val.second);

	return convert.str();
}
template<typename Key,typename Value>
std::string formatter<std::string>::format(const std::map<Key,Value>& i_val)
{
	std::ostringstream convert;
	bool firstItem = true;

	for(const auto& keyValuePair : i_val)
	{
		if(firstItem == false)
		{
			convert << ",";
		}

		convert << format(keyValuePair);
		firstItem = false;
	}

	return convert.str();
}


template<typename First,typename Second>
std::pair<First,Second> formatter<std::pair<First,Second>>::format(const std::string& i_str)
{
	std::pair<First,Second> res;

	if(i_str.empty() == false)
	{
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

	return res;
}

template<typename T>
std::vector<T> formatter<std::vector<T>>::format(std::string i_str)
{
	std::vector<T> res;

	if(i_str.empty() == false)
	{
		do
		{
			const size_t separatorPos = i_str.find(",");
			const std::string leftStr = i_str.substr(0,separatorPos);

			i_str = (separatorPos != std::string::npos) ? i_str.substr(separatorPos + 1) : "";

			res.push_back(formatter<T>::format(leftStr));
		} while(i_str.empty() == false);
	}

	return res;
}

template<typename T>
std::list<T> formatter<std::list<T>>::format(std::string i_str)
{
	std::list<T> res;

	if(i_str.empty() == false)
	{
		do
		{
			const size_t separatorPos = i_str.find(",");
			const std::string leftStr = i_str.substr(0,separatorPos);

			i_str = (separatorPos != std::string::npos) ? i_str.substr(separatorPos + 1) : "";

			res.push_back(formatter<T>::format(leftStr));
		} while(i_str.empty() == false);
	}

	return res;
}

template<typename T>
std::set<T> formatter<std::set<T>>::format(std::string i_str)
{
	std::set<T> res;

	if(i_str.empty() == false)
	{
		do
		{
			const size_t separatorPos = i_str.find(",");
			const std::string leftStr = i_str.substr(0,separatorPos);

			i_str = (separatorPos != std::string::npos) ? i_str.substr(separatorPos + 1) : "";

			res.insert(formatter<T>::format(leftStr));
		} while(i_str.empty() == false);
	}

	return res;
}

template<typename Key, typename Value>
std::map<Key,Value> formatter<std::map<Key,Value>>::format(std::string i_str)
{
	std::map<Key,Value> res;

	if(i_str.empty() == false)
	{
		do
		{
			const size_t separatorPos = i_str.find(",");
			const std::string particleStr = i_str.substr(0,separatorPos);

			if(separatorPos != std::string::npos)
			{
				const size_t delimiterPos = particleStr.find(":");
				res.insert(std::make_pair(formatter<Key>::format(particleStr.substr(0,delimiterPos)),formatter<Value>::format(particleStr.substr(delimiterPos + 1))));

				i_str = i_str.substr(separatorPos + 1);
			}
			else
			{
				const size_t delimiterPos = particleStr.find(":");
				res.insert(std::make_pair(formatter<Key>::format(particleStr.substr(0,delimiterPos)),formatter<Value>::format(particleStr.substr(delimiterPos + 1))));

				break;
			}
		} while(i_str.empty() == false);
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

}
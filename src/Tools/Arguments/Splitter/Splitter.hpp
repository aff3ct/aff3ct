#ifndef ARGUMENT_SPLITTER_HPP_
#define ARGUMENT_SPLITTER_HPP_

#include <string>
#include <vector>

namespace aff3ct
{
namespace tools
{

struct Splitter
{
	static std::vector<std::string> split(const std::string& val,
	                               const std::string& head,
	                               const std::string& queue,
	                               const std::string& separator)
	{
		std::vector<std::string> list_vals;

		// try first to find a head
		size_t pos = val.find_first_of(head);

		if (pos == std::string::npos)
			pos = 0;
		else
			pos++;

		while (pos < val.size())
		{
			// try to find any separator to get values
			size_t found_pos = val.find_first_of(separator, pos);

			if (found_pos == std::string::npos)
			{
				// try find a queue character
				found_pos = val.find_first_of(queue, pos);

				if (found_pos != std::string::npos)
					list_vals.push_back(val.substr(pos, found_pos - pos));
				else
					list_vals.push_back(val.substr(pos));

				break;
			}

			list_vals.push_back(val.substr(pos, found_pos - pos));


			pos = found_pos + 1;
		}

		return list_vals;
	}
};

struct Generic_splitter : Splitter
{
	static std::vector<std::string> split(const std::string& val)
	{
		const std::string head      = "{([";
		const std::string queue     = "})]";
		const std::string separator = ";,.|";

		return Splitter::split(val, head, queue, separator);
	}
};





// static std::vector<T> get_list(const std::string& val)
// {
// 	static_assert(std::is_same<T, std::string>::value || std::is_same<T, float>::value || std::is_same<T, int>::value || std::is_same<T, bool>::value,
// 	              "The get_list function is available only with a std::string, a float, a int or a bool type.");

// 	auto list = seperate(val);

// 	std::vector<T> list_T(list.size());

// 	void * p_val = nullptr;

// 	for(unsigned i = 0; i < list.size(); i++)
// 	{
// 		if (std::is_same<T, float>::value)
// 			p_val = (void*)(new float(std::stof(list[i])));

// 		else if (std::is_same<T, int>::value)
// 			p_val = (void*)(new int(std::stoi(list[i])));

// 		else if (std::is_same<T, bool>::value)
// 			p_val = (void*)(new bool(std::stoi(list[i])));

// 		else if (std::is_same<T, std::string>::value)
// 			p_val = (void*)(new std::string(list[i]));

// 		else
// 			p_val = nullptr;


// 		if (p_val == nullptr)
// 			throw std::runtime_error("Couldn't convert value.");

// 		list_T[i] = *(T*)p_val;

// 		delete (T*)p_val;
// 	}

// 	return list_T;
// }

}
}
#endif /* ARGUMENT_SPLITTER_HPP_ */
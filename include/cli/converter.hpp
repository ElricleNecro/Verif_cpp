#ifndef VERIF_OPTION_CONVERTER_HPP
#define VERIF_OPTION_CONVERTER_HPP

#include <string>
#include <sstream>

namespace cli {
	template<typename T>
	struct Converter {
		std::string convert(T& var)
		{
			std::stringstream ss;
			ss << var;
			return ss.str();
		}

		bool decode(std::string str, T& var)
		{
			std::stringstream ss(str);
			ss >> var;
			return true;
		}
	};

	template<>
	std::string Converter<bool>::convert(bool& var)
	{
		if( var )
			return "true";
		return "false";
	}

	template<>
	std::string Converter<std::string>::convert(std::string& var)
	{
		return var;
	}

	template<>
	bool Converter<bool>::decode(std::string str, bool& var)
	{
		var = (str == "true" || str == "True" || str == "on" || str == "On" || str == "1");
		if( var )
			return true;
		return false;
	}

	template<>
	bool Converter<std::string>::decode(std::string str, std::string& var)
	{
		var = str;
		return true;
	}
}

#endif

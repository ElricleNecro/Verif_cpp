#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <exception>

#include <cstdlib>

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

class BadValue : public std::exception
{
	public:
		BadValue(const std::string& msg)
		{
			this->msg = msg;
		}
		virtual ~BadValue(void) throw()
		{
		}

		virtual const char* what(void) const throw()
		{
			return this->msg.c_str();
		}

	private:
		std::string msg;
};

class Argument {
	public:
		Argument(void)
		{
		}

		Argument(const std::string &str)
		: value(str)
		{
		}

		Argument(const Argument& obj)
		{
			this->value = obj.value;
		}

		~Argument(void)
		{
		}

		Argument& operator=(const Argument& obj)
		{
			this->value = obj.value;
			return *this;
		}

		void Set(const std::string& val)
		{
			this->value = val;
		}

		template<typename T>
		operator T()
		{
			return this->as<T>();
		}

		template<typename T>
		T as(void)
		{
			T var;
			Converter<T> b;
			if( b.decode(this->value, var) )
				return var;
			throw BadValue("Decode failed!");
		}

	private:
		std::string value;
};

typedef std::map<std::string, Argument> Config;

class DefaultChain
{
	public:
		std::string operator()(std::string &var)
		{
			return var;
		}

};

template<typename _comparison = DefaultChain>
class ArgumentParser {
	private:
		class Option {
			public:
				Option(const std::string& nm)
				: TakeArgs(false)
				{
					this->names.push_back(nm);
				}
				Option(const Option& opt)
				: TakeArgs(false)
				{
					this->names = opt.names;
					this->help  = opt.help;
				}

				Option(void)
				: TakeArgs(false)
				{
				}

				Option& Set_Name(const std::string& nm)
				{
					this->names.push_back(nm);
					return *this;
				}

				Option& Set_Help(const std::string& hp)
				{
					this->help = hp;
					return *this;
				}

				std::string& Get_Help(void)
				{
					return this->help;
				}

				Option& Alias(const std::string& nm)
				{
					this->names.push_back(nm);
					return *this;
				}

				Option& UseArgs(void)
				{
					this->TakeArgs = true;
					return *this;
				}

				bool In(const std::string& var)
				{
					for( auto x: this->names )
						if( var == x )
							return true;
					return false;
				}

			private:
				std::vector<std::string> names;
				std::string help;
				bool TakeArgs;
				friend class ArgumentParser;
		};

		void add_args(const int argc, const char** argv)
		{
			this->exec_name = argv[0];
			for(int i=1; i<argc; i++)
				this->args.push_back(argv[i]);
		}

		void print_help(void)
		{
			std::cout << this->exec_name << " [options]" << std::endl;
			for( auto *x : this->opt )
			{
				std::cout << x->names[0];
				for( unsigned int i = 1; i < x->names.size(); i++ )
					std::cout << ", " << x->names[i];
				std::cout << "\t" << x->Get_Help() << std::endl;
			}
			std::cout << "-h, --help\tShow this help message." << std::endl;
		}

		std::vector<std::string> args;
		std::vector<Option*> opt;
		std::string exec_name;
		_comparison comp;
		int exit_code;

	public:
		ArgumentParser(void) : exit_code(EXIT_SUCCESS)
		{
		}

		ArgumentParser(const int argc, const char** argv) : exit_code(EXIT_SUCCESS)
		{
			this->add_args(argc, argv);
		}

		~ArgumentParser(void)
		{
			for( auto &x : this->opt )
				delete x;
		}

		Option* Add(std::string name)
		{
			this->opt.push_back(new Option(name));
			return this->opt.back();
		}

		Config Parse(const int argc, const char** argv)
		{
			this->add_args(argc, argv);
			return this->Parse();
		}

		Config Parse(void)
		{
			Config parsed;
			for( unsigned int i = 0; i < this->args.size(); i++ )
			{
				if( this->args[i] == "-h" || this->args[i] == "--help")
				{
					this->print_help();
					std::exit(this->exit_code);
				}
				for( auto dopt: this->opt )
				{
					if( dopt->In(this->args[i]) )
					{
						//std::cout << this->args[i] << " " << dopt->names[0] << " "
							  //<< this->comp(
								//dopt->names[0]
							//) << std::endl;

						if( dopt->TakeArgs )
						{
							parsed[
								this->comp(
									dopt->names[0]
								)
							].Set(this->args[++i]);
						}
						else
						{
							parsed[
								this->comp(
									dopt->names[0]
								)
							].Set("true");
						}
						break;
					}
				}
			}

			return parsed;
		}
};

class MyMap {
	private:
		std::map<std::string, std::string> map;

	public:
		MyMap(void)
		{
			this->map["-c"]         = "config";
			this->map["-G"]         = "G";
			this->map["--activate"] = "bool";
		}
		std::string operator()(std::string& var)
		{
			auto it = this->map.find(var);
			if( it != this->map.end() )
			{
				return this->map[var];
			}
			return var;
		}
};

int main(int argc, char const* argv[])
{
	Config cfg_sys, cfg_user, cfg_args;

	Argument test("10");
	std::cout << test.as<int>() << std::endl;
	std::cout << test.as<double>() << std::endl;
	std::cout << test.as<std::string>() << std::endl;

	ArgumentParser<MyMap> parser(argc, argv);
	parser.Add("-c")->UseArgs().Alias("--config").Set_Help("Configuration file to use.");
	parser.Add("-G")->UseArgs().Alias("--grav-constant").Set_Help("Value of the gravitationnal constant.");
	parser.Add("--activate")->Set_Help("Test de booléan");
	cfg_args = parser.Parse();

	auto it = cfg_args.find("config");
	if( it != cfg_args.end() )
		std::cout << "config " << it->second.as<std::string>() << std::endl;

	it = cfg_args.find("G");
	if( it != cfg_args.end() )
		std::cout << "G " << it->second.as<double>() << std::endl;

	it = cfg_args.find("bool");
	if( it != cfg_args.end() )
		std::cout << "bool " << it->second.as<bool>() << std::endl;

	return 0;
}

/*
#include <string>
#include <iostream>
#include <map>
 
int main()
{
    std::map<std::string,int> my_map;
    my_map["x"] =  11;
    my_map["y"] = 23;
 
    auto it = my_map.find("x");
    if (it != my_map.end()) std::cout << "x: " << it->second << "\n";
 
    it = my_map.find("z");
    if (it != my_map.end()) std::cout << "z1: " << it->second << "\n";
 
    // Accessing a non-existing element creates it
    if (my_map["z"] == 42) std::cout << "Oha!\n";
 
    it = my_map.find("z");
    if (it != my_map.end()) std::cout << "z2: " << it->second << "\n";
}

stdout:
x: 11
z2: 0
*/

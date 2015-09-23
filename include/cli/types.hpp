#ifndef VERIF_OPTION_TYPES_HPP
#define VERIF_OPTION_TYPES_HPP

#include <map>
#include <string>
#include <exception>
#include <yaml-cpp/yaml.h>

#include "cli/converter.hpp"

namespace cli {
	class BadValue : public std::exception
	{
		public:
			BadValue(const std::string& m)
			: msg(m)
			{
				this->msg = msg;
			}
			BadValue(const BadValue& old)
			: msg(old.msg)
			{
				this->msg = old.msg;
			}
			virtual ~BadValue(void) noexcept
			{
			}

			virtual const char* what(void) const noexcept
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
}

namespace YAML {
	template<>
	struct convert<cli::Config> {
		static Node encode(cli::Config& rhs)
		{
			Node node;

			if( rhs.find("G") != rhs.end() )
				node["G"]         = rhs[ "G" ].as<double>();
			if( rhs.find("rayon") != rhs.end() )
				node["rayon"]     = rhs[ "rayon" ].as<double>();
			if( rhs.find("norme") != rhs.end() )
				node["norme"]     = rhs[ "norme" ].as<double>();
			if( rhs.find("opening") != rhs.end() )
				node["opening"]   = rhs[ "opening" ].as<double>();
			if( rhs.find("softening") != rhs.end() )
				node["softening"] = rhs[ "softening" ].as<double>();
			if( rhs.find("pos_conv") != rhs.end() )
				node["pos_units"] = rhs[ "pos_conv" ].as<double>();
			if( rhs.find("vit_conv") != rhs.end() )
				node["vel_units"] = rhs[ "vit_conv" ].as<double>();

			if( rhs.find("leaf") != rhs.end() )
				node["leaf"]      = rhs[ "leaf" ].as<int>();
			if( rhs.find("nb_bin") != rhs.end() )
				node["nb_bin"]    = rhs[ "nb_bin" ].as<int>();
			if( rhs.find("verbosity") != rhs.end() )
				node["verbosity"] = rhs[ "verbosity" ].as<int>();

			if( rhs.find("logfile") != rhs.end() )
				node["logfile"]   = rhs[ "logfile" ].as<std::string>();
			if( rhs.find("outfile") != rhs.end() )
				node["outfile"]   = rhs[ "outfile" ].as<std::string>();
			if( rhs.find("plug-ins") != rhs.end() )
				node["plug-ins"]  = rhs[ "plug-ins" ].as<std::string>();

			return node;
		}
		static bool decode(const Node& node, cli::Config& rhs)
		{
			(void)node;
			(void)rhs;

			return false;
		}
	};
}

#endif /* end of include guard */

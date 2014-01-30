#ifndef YAML_CONFIG_HPP
#define YAML_CONFIG_HPP

#include <yaml-cpp/yaml.h>

namespace cfg {
	struct Config {
			int type;
			int verbosity;
			int leaf;
			int nb_bin;

			bool periodic;
			bool print_exemple;
			bool gravity_instead_density;

			double opening;
			double G;
			double rayon;
			double softening;
			double pos_conv;
			double vit_conv;
			double norme;

			std::string logfile;
			std::string outfile;
			std::string name;
			std::vector<std::string> infile;
	};
}

namespace YAML {
	template<>
	struct convert<cfg::Config> {
		static Node encode(const cfg::Config& rhs)
		{
			Node node;

			node["G"]         = rhs.G;
			node["norme"]     = rhs.norme;
			node["rayon"]     = rhs.rayon;
			node["opening"]   = rhs.opening;
			node["softening"] = rhs.softening;
			node["pos_units"] = rhs.pos_conv;
			node["vel_units"] = rhs.vit_conv;

			node["leaf"]      = rhs.leaf;
			node["nb_bin"]    = rhs.nb_bin;
			node["verbosity"] = rhs.verbosity;

			node["logfile"]   = rhs.logfile;
			node["outfile"]   = rhs.outfile;
			node["plug-ins"]  = rhs.name;

			return node;
		}
		static bool decode(const Node& node, cfg::Config& rhs)
		{
			if( node["G"] )
				rhs.G = node["G"].as<double>();

			if( node["opening"] )
				rhs.opening = node["opening"].as<double>();

			if( node["rayon"] )
				rhs.rayon = node["rayon"].as<double>();

			if( node["softening"] )
				rhs.softening = node["softening"].as<double>();

			if( node["pos_units"] )
				rhs.pos_conv = node["pos_units"].as<double>();

			if( node["vel_units"] )
				rhs.vit_conv = node["vel_units"].as<double>();

			if( node["norme"] )
				rhs.norme = node["norme"].as<double>();

			if( node["logfile"] )
				rhs.logfile = node["logfile"].as<std::string>();

			if( node["outfile"] )
				rhs.outfile = node["outfile"].as<std::string>();

			if( node["plug-ins"] )
				rhs.name = node["plug-ins"].as<std::string>();

			if( node["nb_bin"] )
				rhs.nb_bin = node["nb_bin"].as<int>();

			if( node["verbosity"] )
				rhs.verbosity = node["verbosity"].as<int>();

			if( node["leaf"] )
				rhs.leaf = node["leaf"].as<int>();

			if( node["type"] )
				rhs.type = node["type"].as<int>();

			return true;
		}
	};
}

namespace cfg {
	class ConfigReader {
		public:
			ConfigReader(const std::string& fname)
			{
				YAML::Node config = YAML::LoadFile(fname.c_str());
				this->param       = config.as<Config>();
			}
			void Add(const std::string& fname)
			{
				YAML::Node config = YAML::LoadFile(fname.c_str());
				YAML::convert<Config>::decode(config, this->param);
			}
			void Add(YAML::Node& node)
			{
				YAML::convert<Config>::decode(node, this->param);
			}

			Config Get(void)
			{
				return this->param;
			}
		private:
			Config param;
	};
}

#endif /* end of include guard */

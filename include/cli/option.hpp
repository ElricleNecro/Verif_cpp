#ifndef VERIF_OPTION_USE_HPP
#define VERIF_OPTION_USE_HPP

#include <string>
#include <iostream>
#include <yaml-cpp/yaml.h>

#include "cli/optionparser.hpp"

namespace cli {
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

			~Config(void) {};
	};

	class FromYaml {
		public:
			FromYaml(const std::string &str);
			~FromYaml(void);

			Config Get(void);
		private:
			Config cfg;
	};

	class ArgsParser {
		private:
			OptionParser* parser;
			Config Parameter;

		public:
			int nbbin;
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
			std::vector<std::string> infile;
			std::string outfile;
			std::string name;

			// liste des paramètres simples (par exemple la listes des fichiers à traiter)
			// déjà accessible dans parser, mon on peut souhaiter supprimé l'objer
			// parser après l'analyse.
			std::vector<std::string> params;

		public:
			ArgsParser(int argc, char *argv[]);
			~ArgsParser(void);

			void pre_process_options(void);

			void post_process_options(void);

			std::string get_exemples(void);
			void init_options(void);

			Config GetParameters(void);
	};
}

namespace YAML {
	template<>
	struct convert<cli::Config> {
		static Node encode(const cli::Config& rhs);
		static bool decode(const Node& node, cli::Config& rhs);
	};
}

#endif /* end of include guard */

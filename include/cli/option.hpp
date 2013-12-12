#ifndef VERIF_OPTION_USE_HPP
#define VERIF_OPTION_USE_HPP

#include <string>
#include <iostream>

#include "cli/optionparser.hpp"

namespace cli {
	class DemoOptions  {
		private:
			OptionParser* parser;

		public:
			int nbbin;
			int type;
			int verbosity;
			int leaf;

			bool periodic;
			bool print_exemple;
			bool gravity_instead_density;

			double opening;
			double G;
			double rayon;
			double softening;
			double pos_conv;
			double vit_conv;

			std::string logfile;
			std::vector<std::string> infile;
			std::string outfile;
			std::string name;

			// liste des paramètres simples (par exemple la listes des fichiers à traiter)
			// déjà accessible dans parser, mon on peut souhaiter supprimé l'objer
			// parser après l'analyse.
			std::vector<std::string> params;

		public:
			DemoOptions(int argc, char *argv[]);
			~DemoOptions(void);

			void pre_process_options(void);

			void post_process_options(void);

			std::string get_exemples(void);
			void init_options(void);
	};
}

#endif /* end of include guard */

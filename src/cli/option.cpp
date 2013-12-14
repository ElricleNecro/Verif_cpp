#include "cli/option.hpp"

namespace cli {
	DemoOptions::DemoOptions(int argc, char *argv[])
	{
		parser = new OptionParser(argc, argv);

		this->init_options();

		// Personnalisation éventuelle du comportement : que faire si l'utilisateur
		// passe une option incorrecte ?  Les valeurs possibles sont :
		//      std::exit (par défaut), raise, warn, ignore
		parser->on_error("exit");

		// Permet des post-traitements (vérification de cohérence d'options,...)
		pre_process_options();

		// On peut lancer l'analyse ici ou bien le faire depuis le main() de la
		// du programme principal.
		parser->parse();

		// Permet des post-traitements (vérification de cohérence entre options,...)
		post_process_options();

		// Les attributs options étant affectés, on peut supprimer le parser
		// sauf si on souhaite exploiter quelques-unes de ses méthodes ou attributs
		// e.g parser->print_values() ou parser->params
		//  delete parser;
		//  parser = NULL; // Utile ssi on veut pouvoir tester l'existence de parser !
	}

	DemoOptions::~DemoOptions(void)
	{
	}

	void DemoOptions::pre_process_options(void)
	{
		if (parser->argc <= 1) {
			std::cout << parser->abstract;
			std::exit(1);
		}
	}

	void DemoOptions::post_process_options(void)
	{
		if (this->print_exemple) {
			std::cout << this->get_exemples();
			std::exit(0);
		}

		// Lecture des paramètres supplémentaires (sans clé associée)
		// Paramètre 1 : fichier d'entrée
		//  this->infile = "";
		//  parser->add_string_option(this->infile)
		//        ->set_desc("Fichier d'instance (à traité ou à généré).");
		if (parser->params.size() < 1) {
			this->parser->send_error("You should give an input file!");
			std::exit(1);
		}

		this->infile = parser->params;
	}

	std::string DemoOptions::get_exemples(void) {
		std::stringstream buf;
		buf <<
			"Usage example:\n" +
			parser->argv[0] + " -o 0.5 -G 1.0 -v all --voisin 25 --out test.hdf5 None_500\n"
			;
		return buf.str();
	}

	void DemoOptions::init_options(void) {

		//--------------
		std::stringstream buf;
		buf << "Syntaxe :\n";
		buf << "  "<< parser->exename << " [options...] [--] [fichier_instance [fichier_solution]]\n";
		buf << "  "<< parser->exename << " --help  pour en savoir plus\n";
		parser->abstract = buf.str();

		parser->add_doc("\nDébut des options génériques\n");

		//--------------
		this->type = 63;
		parser->add_int_option("--type", this->type)
			->set_desc("Type of particles to load.")
			->add_alias("-t");

		//--------------
		this->outfile = "simu.out";
		parser->add_string_option("--output", this->outfile)
			->set_desc("Output file. The output format depend of the loaded plug-ins.")
			->add_alias("-o");

		//--------------
		this->print_exemple = false;
		parser->add_switch_option("--exemples", this->print_exemple)
			->set_desc("Affiche quelques exemples d'utilisation).")
			->add_alias("-H");

		parser->add_doc("\nDébut des options spécifiques\n");

		//--------------
		this->opening = 0.5;
		parser->add_double_option("--opening", this->opening)
			->set_desc("Opening angle for the octtree.");

		this->G = 6.67384e-11;
		parser->add_double_option("--gravitation-constant", this->G)
			->set_desc("Value of the gravitationnal constant.")
			->add_alias("-G");

		this->logfile = parser->exename + ".log";
		parser->add_string_option("--logfile", this->logfile)
			->set_desc("Fichier d'enregistrement des messages");

		this->name = "";
		parser->add_string_option("--reader", this->name)
			->set_desc("Reader to use.");

		this->norme  = 1.0;
		parser->add_double_option("--normalisation", this->norme)
			->set_desc("To which value normalise all histogram.");

		this->nb_bin = 100;
		parser->add_int_option("--nb-bin", this->nb_bin)
			->set_desc("Number of bins to use.");

		this->pos_conv = 1.0;
		parser->add_double_option("--pos-conv", this->pos_conv)
			->set_desc("Position units to convert data into.");

		this->vit_conv = 1.0;
		parser->add_double_option("--vit-conv", this->vit_conv)
			->set_desc("Velocity units to convert data into.");

		// Ceci permet de transférer l'attribut params interne à la classe
		// OptionParser vers un attribut client (de même nom ici)
		// Cela permettra de supprimer l'objet parser après analyse et avant
		// l'exécution de l'application proprement dite (si gain de place
		// mémoire nécessaire)
		parser->set_params_vector(this->params);
	};

}

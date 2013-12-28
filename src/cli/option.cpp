#include "cli/option.hpp"

namespace cli {
	ArgsParser::ArgsParser(int argc, char *argv[])
	{
		parser = new OptionParser(argc, argv);

		this->init_options();

		parser->on_error("exit");
		pre_process_options();
		parser->parse();
		post_process_options();
	}

	ArgsParser::~ArgsParser(void)
	{
	}

	void ArgsParser::pre_process_options(void)
	{
		if (parser->argc <= 1) {
			std::cout << parser->abstract;
			std::exit(1);
		}
	}

	void ArgsParser::post_process_options(void)
	{
		if (this->print_exemple) {
			std::cout << this->get_exemples();
			std::exit(0);
		}

		// Lecture des paramètres supplémentaires (sans clé associée)
		// Paramètre 1 : fichier d'entrée
		//this->infile = "";
		//parser->add_string_option(this->infile)
			//->set_desc("Fichier d'instance (à traité ou à généré).");
		if (parser->params.size() < 1) {
			this->parser->send_error("You should give an input file!");
			std::exit(1);
		}

		this->infile = parser->params;
	}

	std::string ArgsParser::get_exemples(void)
	{
		std::stringstream buf;
		buf <<
			"Usage example:\n" +
			parser->argv[0] + " -o 0.5 -G 1.0 -v all --voisin 25 --out test.hdf5 None_500\n"
			;
		return buf.str();
	}

	void ArgsParser::init_options(void)
	{

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
	}

	Config ArgsParser::GetParameters(void)
	{
		return this->Parameter;
	}
}

namespace YAML {
	Node convert<cli::Config>::encode(const cli::Config& rhs) {
		Node node;

		node["G"]         = rhs.G;
		node["opening"]   = rhs.opening;
		node["rayon"]     = rhs.rayon;
		node["softening"] = rhs.softening;
		node["pos_units"] = rhs.pos_conv;
		node["vel_units"] = rhs.vit_conv;
		node["norme"]     = rhs.norme;
		node["logfile"]   = rhs.logfile;
		node["outfile"]   = rhs.outfile;
		node["plug-ins"]  = rhs.name;
		node["nb_bin"]    = rhs.nb_bin;
		node["verbosity"] = rhs.verbosity;
		node["leaf"]      = rhs.leaf;

		return node;
	}
	bool convert<cli::Config>::decode(const Node& node, cli::Config& rhs) {
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

		return true;
	}
}

#include "cli/optionparser.hpp"

namespace cli {
	OptionParser::Option::Option(std::string id, SpecType type) {
		// this->parser = parser; // BUG déclaration
		this->id = id;
		this->internal = false;
		this->keys = new std::vector<std::string>();
		this->abbrevs = new std::vector<OptionParser::Option*>();
		this->desc = "MISSING DESCRIPTION";
		this->required = false;
		this->type = type;

		this->active_value   = "ACTIVE_VALUE_UNDEFINED";
		this->ref_option     = NULL;

		this->bool_ref_variable   = NULL;
		this->int_ref_variable    = NULL;
		this->double_ref_variable = NULL;
		this->string_ref_variable = NULL;

	}

	OptionParser::Option::~Option() {
		delete this->keys;
		delete this->abbrevs;
	}

	OptionParser::Option* OptionParser::Option::set_desc(std::string desc) {
		this->desc = desc;
		return this;
	}

	OptionParser::Option* OptionParser::Option::set_required(bool req) {
		this->required = req;
		return this;
	}

	OptionParser::Option* OptionParser::Option::add_alias(std::string key) {
		if (this->type == SWITCH) {
			this->add_abbrev(key, !*this->bool_ref_variable);
			/// this->add_abbrev(key, *this->bool_ref_variable);
		} else {
			this->keys->push_back(key);
		}
		return this;
	}

	OptionParser::Option* OptionParser::Option::add_key(std::string key) {  // idem que add_alias
		return this->add_alias(key);
	}

	// Le paramètre internal est nécessaire pour créer les options de type
	// SWITCH ; car se serait très lourd de récupérer la référence sur l'option de
	// type ABBREV crée par cette méthode add_abbrev de façon à en modifier
	// l'attribut internal après sa création par la classe OptionParser::OptionParser.
	OptionParser::Option* OptionParser::Option::add_abbrev(std::string key, std::string value, bool internal) {
		OptionParser::Option* abbrev = new OptionParser::Option(key, ABBREV);
		abbrev->active_value = value;
		abbrev->ref_option = this;
		abbrev->internal = internal;
		this->abbrevs->push_back(abbrev);
		return this;
	}

	// quelque facilités (int double) pour éviter de taper une chaine.
	// Mais attention : ne pas créer add_abbrev(std::string key, double value)
	// sinon elle est appelé à mon insu !?
	OptionParser::Option* OptionParser::Option::add_abbrev(std::string key, int value, bool internal) {
		std::stringstream buf;
		buf << value;
		return add_abbrev(key, buf.str(), internal);
	}

	OptionParser::Option* OptionParser::Option::add_abbrev(std::string key, double value, bool internal) {
		std::stringstream buf;
		buf << value;
		return add_abbrev(key, buf.str(), internal);
	}

	// Dans le cas d'un paramètre de type std::string, on prévoit le
	// cas où le type souhaité est donné par l'attribut type.
	// Si l'attribut type n'est pas STRING, on procède à une conversion.
	// Ceci permet de passer une chaine au moment de l'analyse des arguments
	// aueaue soit le type final.
	//
	// ATTENTION la variante set_value(char const*) est nécessaire sinon bug!!!
	// (i.e. aucune des méthodes set_value(...) ne serait appelée)
	// google : stackoverflow c-style-strings-to-stdstring-conversion-clarification
	OptionParser::Option* OptionParser::Option::set_value(char const* chars) {
		return set_value(std::string(chars));
	}

	OptionParser::Option* OptionParser::Option::set_value(const std::string& value) {

		// std::cout << "xxx set_value " << id << " value=" << value << std::endl;
		if (this->type==BOOL || this->type==SWITCH) {
			// TODO:  if ( (value == "1") || (value == "true") || (value == "t") ) {...}
			if (value == "1" || value == "true") {
				*this->bool_ref_variable = true;
			} else if (value == "0" || value == "false") {
				*this->bool_ref_variable = false;
			} else {
				std::cerr << "Erreur : seules les valeurs 0 ou 1 sont supportées pour "
					<< "l'option " << this->id << " (" << value << ")." << std::endl;
				std::exit(1);
			}
		} else if (this->type==INTEGER) {
			if (value.find_first_not_of("0123456789") != std::string::npos) {
				std::cerr << "Erreur : l'option " << this->id
					<< " doit être de type entier décimal." << std::endl;
				std::exit(1);
			}
			std::stringstream buf(value);
			buf >> *this->int_ref_variable;
		} else if (this->type==DOUBLE) {
			std::stringstream buf(value);
			buf >> *this->double_ref_variable;
		} else if (this->type==STRING) {
			*this->string_ref_variable = value;
		} else if (this->type==ABBREV) {
			std::cerr << "ERREUR : méthode set_value() non implémentée pour "
				<< "le type \"" << type << "\"";
			std::exit(1);
		} else {
			// DOC ou UNKNOWN
			std::cerr << "ERREUR : méthode set_value() non implémentée pour "
				<< "le type \"" << type << "\"";
			std::exit(1);
		}
		return this;
	}

	// retourne une représentation chaine du type de l'objet courant.
	std::string OptionParser::Option::type_to_s(SpecType type) {
		if (type==UNKNOWN) {
			return "UNKNOWN";
		} else if (type==BOOL) {
			return "BOOL";
		} else if (type==INTEGER) {
			return "INTEGER";
		} else if (type==DOUBLE) {
			return "DOUBLE";
		} else if (type==STRING) {
			return "STRING";
		} else if (type==ABBREV) {
			return "ABBREV";
		} else if (type==SWITCH) {
			return "SWITCH";
		} else if (type==DOC) {
			return "DOC";
		} else {
			return "ERREUR_TYPE_NON_DEFINI";
		}
	}

	std::string OptionParser::Option::type_to_s(void) {
		return type_to_s(this->type);
	}

	// retourne une représentation chaine de la valeur courante
	std::string OptionParser::Option::value_to_s(void) {
		std::stringstream buf;
		if (type==BOOL || type==SWITCH) {
			buf << *bool_ref_variable;
		} else if (type==INTEGER) {
			buf << *int_ref_variable;
		} else if (type==DOUBLE) {
			buf << *double_ref_variable;
		} else if (type==STRING) {
			buf << *string_ref_variable;
		} else if (type==ABBREV) {
			buf << *string_ref_variable;
		} else {
			buf << "NO_VALUE";
		}
		return buf.str();
	}

	// Affiche l'état détaillé de l'option
	std::string OptionParser::Option::inspect(void) {
		std::stringstream buf;
		buf << "------------------------------\n";
		if (this->type == DOC) {
			// une option DOC n'a pas de valeur associée
			buf << "id:                 " << id << std::endl;
			buf << "type:               " << type_to_s() << std::endl;
			buf << "desc:               " << desc << std::endl;
			buf << std::endl;
		} else if (this->type == ABBREV) {
			// La valeur associée à une option ABBREV est celle de sa ref_option
			buf << "id:                 " << id
				<< (this->internal ? " (internal) " : "") << std::endl;
			buf << "type:               " << type_to_s() << std::endl;
			buf << "active_value:       " << this->active_value << std::endl;
			buf << "ref_option:         " << this->ref_option->id << std::endl;
			buf << "ref_value:          " << this->ref_option->value_to_s();
			buf << std::endl;
			buf << std::endl;
		} else {
			// Le type SWITCH est traité comme le type BOOL (ou INT, ...)
			buf << "id:                 " << id
				<< (this->internal ? " (internal) " : "") << std::endl;
			buf << "alias possibles:    ";
			for (std::vector<std::string>::iterator it = keys->begin();
					it != keys->end(); it++) {
				buf << *it << ", ";
			}
			buf << std::endl;
			buf << "type:               " << type_to_s() << std::endl;
			// required n'est pas encore utilisé mais CONSERVER la ligne suivante
			// buf << "required:        " << required << std::endl;
			buf << "value:              " << value_to_s() << std::endl;
			buf << "desc:               " << desc << std::endl;
			buf << std::endl;
			if (this->abbrevs->size() >= 1) {
				for (unsigned i=0; i < this->abbrevs->size(); i++) {
					buf << this->abbrevs->at(i)->inspect();
				}
			}
		}
		return buf.str();
	}

	// Affiche l'aide pour cette option
	std::string OptionParser::Option::get_help(void) {
		if (this->internal) {
			return "";  // pas d'aide pour une option interne !
		}

		std::stringstream buf;
		// buf << "-------- Aide d'une option : --------\n";
		// On commence par le cas particulier d'une pseudo-option (e.g. DOC...)
		if (this->type == DOC) {
			buf << this->desc << std::endl;
			return buf.str();
		} else if (this->type == ABBREV) {
			buf << "    " << this->id << " : abréviation pour ";
			buf << this->ref_option->id << " " << this->active_value;
			buf << std::endl;
			return buf.str();
		} else {
			if (this->type == SWITCH) {
				// Affichage d'une aide spécial pour les types SWITCH
				//
				// on veut construire "dry-run" à partir de "--dry-run-val"
				//
				std::string base_key =  OptionParser::OptionParser::ltrim_string(this->id, "-");
				unsigned long idx  = base_key.find_last_of("-"); // xxx-var
				if (idx != std::string::npos) {
					base_key.erase(idx, std::string::npos); // suppression de "-var"
				} else {
					std::cerr << "id d'option incorrect : " << this->id << " attendu xxx-val\n";
					std::exit(1);
				}

				buf << "  " << "--[no-]" << base_key
					<< " : flag associé à  ";
				buf << this->id
					<< " (def: " << this->value_to_s() << ")" ;
				/// buf << std::endl;
			} else {
				// les types d'option standard avec valeur
				buf << "  " << this->id << " " << this->type_to_s()
					<< "  (def: " << this->value_to_s() << ")";
			}

			// Affichage des alias éventuels de cette option
			if (keys->size() >= 1) {
				buf << "    alias: ";
				for (unsigned i=0; i < this->keys->size(); i++) {
					buf << "" << this->keys->at(i) << ", ";
				}
			}
			buf << std::endl;
			buf << "    " << this->desc << std::endl;
			// Affichage des abréviations éventuelles de cette option
			if (abbrevs->size() >= 1) {
				for (unsigned i=0; i < this->abbrevs->size(); i++) {
					OptionParser::Option* abbrev = this->abbrevs->at(i);
					if (abbrev->internal) continue;
					buf << "    " << abbrev->id << " : abréviation pour ";
					buf << abbrev->ref_option->id << " " << abbrev->active_value;
					buf << std::endl;
				}
			}

			return buf.str();
		}
	}

	OptionParser::OptionParser(int argc, char *argv[]) {
		this->argc = argc;
		this->options = new std::vector<Option*>();
		this->params = std::vector<std::string>();
		this->user_params = NULL;
		this->map_options = new std::map<std::string, OptionParser::Option*>();
		this->exename = argv[0];
		this->error_handler = "exit";
		this->argv = std::vector<std::string>();
		for (int i = 1; i < this->argc; i++) {
			this->argv.push_back(std::string(argv[i]));
			// std::cout << "DETECTION DU MOT : " << argv[i] << std::endl;
		}
		this->uid = 0;

		std::stringstream buf;
		buf << "Syntaxe :\n";
		buf << "  "<< this->exename << " [options...] [--] [file1 [file2 [...]]]\n";
		buf << "  "<< this->exename << " --help  pour en savoir plus";
		this->abstract = buf.str();

	}

	OptionParser::~OptionParser() {
		delete this->options;
		delete this->map_options;
	}

	OptionParser::Option* OptionParser::add_option(std::string id, SpecType type) {
		OptionParser::Option* opt = new OptionParser::Option(id, type);
		this->options->push_back(opt);
		(*this->map_options)[id] = opt;
		return opt;
	}

	OptionParser::Option* OptionParser::add_bool_option(std::string id, bool& var) {
		OptionParser::Option* opt = add_option(id, BOOL);
		opt->bool_ref_variable = &var;
		return opt;
	}

	OptionParser::Option* OptionParser::add_int_option(std::string id, int& var) {
		OptionParser::Option* opt = add_option(id, INTEGER);
		opt->int_ref_variable = &var;
		return opt;
	}

	OptionParser::Option* OptionParser::add_double_option(std::string id, double& var) {
		OptionParser::Option* opt = add_option(id, DOUBLE);
		opt->double_ref_variable = &var;
		return opt;
	}

	OptionParser::Option* OptionParser::add_string_option(std::string id, std::string& var) {
		OptionParser::Option* opt = add_option(id, STRING);
		opt->string_ref_variable = &var;
		return opt;
	}

	OptionParser::Option* OptionParser::add_abbrev_option(std::string id, std::string ref_option_id, std::string value) { // XXX A VIRER QUAND SERA FINI
		OptionParser::Option* opt = add_option(id, ABBREV);
		opt->active_value = value;
		if (!this->key_exists(ref_option_id)) {
			std::cerr << "Erreur : aucune option d'id " << ref_option_id << " n'est définie !";
			std::exit(1);
		}
		OptionParser::Option* ref_opt = this->get(ref_option_id);
		opt->ref_option = ref_opt;
		return opt;
	}

	OptionParser::Option* OptionParser::add_switch_option(std::string id, bool& var) {
		// La création d'un switch --xxx entraine la création de trois options
		// - une option interne de type BOOL --xxx-val mémorisant la valeur
		// - l'option publique de type SWITCH (variante de ABBREV) --xxx
		//   associée à la valeur 1
		// - une option interne de type ABBREV --no-xxx associé à la valeur 0
		//
		// 1. création de la l'option BOOL pour mémoriser la valeur et la
		//    variable du switch
		std::string val_key = id + "-val";
		OptionParser::Option* val_opt = add_option(val_key, SWITCH);  // was BOOL
		val_opt->bool_ref_variable = &var;
		val_opt->internal = false; // par défaut

		// 2. création de l'abéviation principale équivalente au switch
		std::string on_key = id;
		val_opt->add_abbrev(on_key, 1, true); // true pour internal
		// val_opt->abbrevs->at(0)->internal = true; // serait équivalent mais non testé

		// 3. création de l'abréviation pour le switch complémentaire
		// le ltrim_string est nécessaire car on veut --no-dry-run et
		// non pas --no---dry-run
		std::string off_key = std::string("--no-") + OptionParser::OptionParser::ltrim_string(id, "-");
		val_opt->add_abbrev(off_key, 0, true); // true pour internal

		return val_opt;
	}

	// Insère du textet au milieur de l'aide sur les options.
	// On peut insérer un simple saut de ligne en ne passant aucun paramètre.
	OptionParser::Option* OptionParser::add_doc(std::string desc) {
		// l'id d'une doc n'a pas d'importance : on utilise donc un uid automatique
		std::stringstream buf;
		buf << "doc_" << uid++;  // on crée un id du type "doc_1"
		std::string id = buf.str();
		OptionParser::Option* opt = add_option(id);
		opt->type = DOC;
		opt->desc = desc;
		return opt;
	}

	void OptionParser::set_params_vector(std::vector<std::string>& user_params) {
		this->user_params = &user_params;
	}

	// Retourne l'objet OptionParser::Option associé à une clé.
	// Cette clé **doit** exister sinon std::exit !
	OptionParser::Option* OptionParser::get(std::string id) {
		if (!this->key_exists(id)) {
			std::cerr << "Erreur : la clé " << id << " n'existe par !\n";
			std::exit(1);
		}
		return this->map_options->find(id)->second;
	}

	bool OptionParser::key_exists(std::string id) {
		std::map<std::string, OptionParser::OptionParser::Option*>::const_iterator it = this->map_options->find(id);
		if (it != this->map_options->end()) {
			return true; // la valeur est dans it->second
		} else {
			return false;
		}
	}

	// affiche pour chaque clé sa clé de référence (i.e. l'id de l'option associée)
	std::string OptionParser::map_options_to_s(void) {
		std::stringstream buf;
		buf << "Contenu de std::map_options\n";
		std::map<std::string, OptionParser::OptionParser::Option*>::iterator it = this->map_options->begin();
		while (it != this->map_options->end()) {
			OptionParser::Option* opt = it->second;
			std::string  key = it->first;
			buf << it->first << "  " << opt->type_to_s()  << "   =>   ";
			if (opt->type == ABBREV) {
				buf << opt->ref_option->id << " active_value=" << opt->active_value;
			} else {
				buf << opt->id;
			}
			buf << std::endl;
			it++;
		}
		return buf.str();
	}

	void OptionParser::on_error(std::string handler) {
		// std::vector<string> handlers = {"exit", "raise", "warn", "ignore"}; c++0x only
		std::string ar[] = {"exit", "raise", "warn", "ignore"};
		std::vector<std::string> handlers(ar, ar+4);
		std::string handlers_help = "exit, raise, warn, ignore";
		if (std::find(handlers.begin(), handlers.end(), handler) != handlers.end()) {
			this->error_handler = handler;
		} else {
			std::cerr << "Valeur incorrecte pour on_error : " << handler << std::endl
				<< "Valeurs autorisées : " << handlers_help << std::endl;
			std::exit(1);
		}
	}

	void OptionParser::send_error(std::string txt) {
		if (this->error_handler == "ignore") {
			// rien à faire;
		} else if (this->error_handler == "warn") {
			std::cerr << txt << std::endl;
		} else if (this->error_handler == "raise") {
			std::cerr << txt << std::endl;
			/// this->print_syntaxe_abstract(std::cerr);
			std::cerr << this->abstract;
			std::cerr << std::endl;
			// throw new std::string(txt);
			std::cerr << "TODO: Valeur \"raise\" de error_handler non implémentée : " << std::endl;
			std::exit(1);
		} else { // "exit"ou tout autre valeur
			std::cerr << txt << std::endl;
			/// this->print_syntaxe_abstract(std::cerr);
			std::cerr << this->abstract;
			std::cerr << std::endl;
			std::exit(1);
		}
	}

	// Complète la structure d'options et effectue quelques vérifications
	// sur la cohérence au sein de l'ensemble des options.
	// e.g. deux clés identiques ne doivent pas déclarées, ...
	// ATTENTION : PEUT EFFECTUER UN EXIT EN CAS DE PROBLÈMES
	void OptionParser::close(void) {
		// On commence par compléter la std::map avec l'ensemble des clés déclarées
		// On en profite pour vérifier qu'il n'y a pas de répétition de clés
		for (unsigned i=0; i < this->options->size(); i++) {
			OptionParser::Option* opt = this->options->at(i);
			// on recherche parmi les clée (i.e. alias) de l'option opt
			for (unsigned i=0; i < opt->keys->size(); i++) {
				std::string key = opt->keys->at(i);
				if (this->map_options->find(key) != this->map_options->end()) {
					std::cerr << std::endl;
					std::cerr << "ERREUR : pour option#id=" << opt->id << std::endl;
					std::cerr << "  tentative de redéclaration de la clé " << key << std::endl;
					std::cerr << "  qui est déjà déclarée par l'option#id="
						<< this->map_options->find(key)->second->id << " !\n" ;
					std::exit(1);
				}
				this->map_options->insert(make_pair(key, opt));
			}
			// on recherche parmi les abbrevs de l'option opt
			for (unsigned i=0; i < opt->abbrevs->size(); i++) {
				OptionParser::Option* abbrev = opt->abbrevs->at(i);
				// La seule clé d'une abbrev est son id !
				if (this->map_options->find(abbrev->id) != this->map_options->end()) {
					std::cerr << std::endl;
					std::cerr << "ERREUR : pour option#id=" << opt->id << std::endl;
					std::cerr << "  tentative de redéclaration de la clé " << abbrev->id << std::endl;
					std::cerr << "  qui est déjà utilisée par l'option#id="
						<< this->map_options->find(abbrev->id)->second->id << " !\n" ;
					std::exit(1);
				}
				this->map_options->insert(make_pair(abbrev->id, abbrev));
			}
		}
	}

	void OptionParser::parse(void) {
		// construction de la structure d'options et vérif cohérence de la spéc.
		this->close();

		// xxx_hook() n'est utile que si cette classe est utilisée par héritage !
		this->pre_parse_hook();

		// state : position de l'argument en cours de lecture
		//   NORMAL_STATE  : état classique (recherche et extraction d'une option)
		//   POST_KEYS_STATE : il n'y a plus de clé : on accumules les arguments
		//   résiduels (e.g nom de fichiers, ...)
		//
		std::string state = "NORMAL_STATE";
		unsigned i = 0; // indice du premier paramètre le la ligne de commende
		while (i < argv.size()) {
			std::string arg = this->argv[i];
			// std::cout << "xxx i=" << i << "=> arg=" << arg << std::endl;
			if (state == "NORMAL_STATE") {
				// Traitement d'une option normale
				if (arg == "--") {
					// la suite ne contient plus de clé, même si un argument commence
					// par "-xxx"
					state = "POST_KEYS_STATE";
					i++;
					continue;
				} else if (arg == "-h"  || arg == "--help") {
					this->print_help();
					std::exit(0);
				} else if (arg == "---help") {
					std::cout << this->inspect();
					// std::exit(0); // ON PEUT VOULOIR AALYSER LES RESTE DE LA LdC
					i++;
					continue;
				} else if (arg.find("-") == 0) {
					// arg est une clé. On recherche l'option associée à cette clé
					// et on vérifie son existance.
					std::map<std::string, OptionParser::Option*>::iterator it = this->map_options->find(arg) ;
					if (it == this->map_options->end()) {
						std::stringstream buf;
						buf << "Clé inconnu : " << arg;
						this->send_error(buf.str());
						i++;
						continue;
					}
					OptionParser::Option* opt = it->second;
					// On connait l'option associé à cette clé
					// Il faut distinguer si cette clé est une clé seule (i.e. n'est
					// pas associés à une valeur) ou si c'est une option standard
					// associée à une valeur
					// die();
					if (opt->type == ABBREV) {
						// on extrait et on positionne l'option de référence associée
						// à cette option switch
						opt->ref_option->set_value(opt->active_value);
						i++;
						continue;
					} else {
						// il faut extraire la valeur associée à cette clé
						i++;
						if (i >= this->argv.size()) {
							std::cerr << "Manque la valeur pour la clé \"" << arg << "\"\n";
							std::exit(1);
						}
						std::string str_val = argv[i];
						opt->set_value(str_val);
						i++;
						continue;
					}

				} else {
					// ce paramètre n'est pas une clé : on l'ajoute à la liste
					// des "fichiers"
					this->params.push_back(arg);
					if (this->user_params != NULL) {
						this->user_params->push_back(arg);
					}
					i++;
					continue;
				}
			} else if (state == "POST_KEYS_STATE") {
				this->params.push_back(arg);
				if (this->user_params != NULL) {
					this->user_params->push_back(arg);
				}
				i++;
				continue;
			} else {
				std::cerr << "ERREUR dans classe OptionParser::OptionParser : état inconnu state="
					<< state << std::endl;
				std::exit(1);
			}
		}
		// xxx_hook() n'est utile que si cette classe est utilisée par héritage !
		this->post_parse_hook();
	}

	// Retourne une nouvelle chaine nettoyée de certains caractères sur les bords
	// Par défaut, ces caractères sont espaces ou tabulation
	std::string OptionParser::ltrim_string(std::string str, std::string trim_chars) {
		// unsigned int idx  = str.find_first_not_of(chars);
		unsigned long idx  = str.find_first_not_of(trim_chars);
		if (idx == std::string::npos) {
			// Il n'y a que des caractères à supprimer
			return "";
		} else if (idx == 0) {
			// Aucun caractère à supprimer
			return str;
		} else {
			return str.erase(0, idx);
		}
	}

	std::string OptionParser::inspect(void) {
		std::stringstream buf;
		buf << "DETAIL DES OPTIONS DÉFINIES\n";
		for (std::vector<Option*>::iterator it = options->begin();
				it != options->end(); it++) {
			buf << (*it)->inspect();
		}
		if (this->params.size() != 0) {
			buf << "Liste des " << this->params.size() << " arguments sans clé associés\n";
			buf << "params.size()="<< this->params.size() << std::endl;
			for (std::vector<std::string>::iterator it = this->params.begin();
					it != this->params.end(); it++) {
				buf << "    " << *it << std::endl;
			}
		} else {
			buf << "Liste des fichiers à traiter vide (pas d'arguments supplémentaires\n";
		}

		return buf.str();
	}

	std::ostream& OptionParser::print_dump(std::ostream& os) {
		os << this->inspect();
		return os;
	}

	/// // Affiche la syntaxe de l'appli et la description des options
	/// virtual
	/// std::ostream& print_syntaxe_abstract(std::ostream& os = std::cout) {
	///     os << "Syntaxe :\n";
	///     os << "  "<< this->exename << " [options...] [--] [file1 [file2 [...]]]\n";
	///     os << "  "<< this->exename << " --help  pour en savoir plus\n";
	///     return os;
	/// }
	// Affiche la syntaxe de l'appli et la description des options
	std::ostream& OptionParser::print_syntaxe(std::ostream& os) {
		/// os << "Syntaxe :\n";
		/// os << "  "<< this->exename << " [options...] [--] [file1 [file2 [...]]]\n";

		os << this->abstract << std::endl;

		// On n'affiche cette ligne que si l'appli ne propose pas elle-même en
		// premier une option de type DOC.
		if (this->options->size() != 0 && this->options->at(0)->type != DOC) {
			os << "Liste des options :\n";
		}

		for (std::vector<Option*>::iterator it = options->begin();
				it != options->end(); it++) {
			os << (*it)->get_help();
		}
		return os;
	}

	// affiche la valeur courante des options
	// - affiche les valeurs par défaut avant l'appel à la méthode parse()
	// - affiche dans l'ordre de la déclaration
	std::ostream& OptionParser::print_values(std::ostream& os) {
		os << "\nValeurs courantes des options :\n";
		for (std::vector<Option*>::iterator it = options->begin();
				it != options->end(); it++) {
			if ((*it)->type == DOC) {
				// on ignore le type DOC
				continue;
			}
			if ((*it)->ref_option != NULL) {
				// on ignore les switch (associées à une autre option)
				continue;
			}
			os << "    " << (*it)->id << " = " << (*it)->value_to_s() << std::endl;
		}
		if (this->params.size() == 0) {
			os << "Aucun paramètre supplémentaire :\n";
		} else {
			os << "Valeur des paramètres supplémentaires :\n";
			for (unsigned i = 0; i < this->params.size(); i++) {
				std::cout << "  Param " << i
					<< " : " << this->params[i] << std::endl;
			}
		}

		return os;
	}

	// get_help: retourne l'aide de l'application.
	// À REDÉFINIR PAR LA CLASSE FILLE
	std::ostream& OptionParser::print_help(std::ostream& os) {
		return print_syntaxe(os);
	}

	// pre_parse_hook: permet un prétraitemenet avant d'extraire les options
	// (extraction du premier paramètre à la git ou cvs, ...)
	// À REDÉFINIR PAR LA CLASSE FILLE
	void OptionParser::pre_parse_hook(void) {
	}

	// post_parse_hook: assure les vérification supplémentaire sur les options extraites
	// (validité des valeurs, cohérence pour les option interdépendantes, ...)
	// À REDÉFINIR PAR LA CLASSE FILLE
	void OptionParser::post_parse_hook() {
	}
}


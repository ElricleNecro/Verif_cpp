#ifndef VERIF_OPTION_PARSER_HPP
#define VERIF_OPTION_PARSER_HPP

#include <map>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace cli {
	class OptionParser {
		// Type de la spécification : une spéc. peut décrire une option (e.g type DOC)
		// ou du texte séparateur utilisé dans l'aide.
		public:
			enum SpecType { UNKNOWN=0, BOOL=1, INTEGER=2, DOUBLE=3, STRING=4, DOC=5,
				ABBREV=6, SWITCH=7 };

			// uid (unic id) est utilisé pour l'id des pseudo-options car il n'est pas
			// §nécessaire de les retouver à partir de leur id.
			/// static int uid; // voir initialisation à 0 en fin de fichier !
			int uid; // voir initialisation à 0 en fin de fichier !

			// nom de base de l'exécutable
			std::string exename;

			std::string abstract;

			// Que faire en cas d'option non déclarée sur ligne de commande ?
			std::string error_handler;  // possibles:  std::exit | raise | warn | ignore

			// tableau des arguments sur la ligne de commande
			std::vector<std::string> argv;

			// Mémoriser argc permet des prétaitements éventuels
			int argc;

			// liste des paramètres simples (par exemple la listes des fichiers à traiter)
			std::vector<std::string> params;

			// pointeur sur la liste des parametres (idem ci-dessus mais c'est un pointeur
			// vers une variable utilisateur)
			std::vector<std::string>* user_params;

			class Option  {
				public:
					// error: field has incomplete type 'OptionParser'
					// parser: le parser propriétaire de cette option
					// OptionParser::OptionParser parser; // BUG déclaration
					std::string id;
					std::string desc;
					bool required; // à conserver mais si pas exploité

					SpecType type;

					// l'attribut active_value n'est utilisée que pour les options ABBREV et
					// SWITCH qui doivent mémoriser la valeur à affecter.
					// Le type std::string est suffisant car le traitement se fera en fonction
					// du type de la variable de référence.
					std::string active_value;

					bool*     bool_ref_variable;
					int*       int_ref_variable;
					double* double_ref_variable;
					std::string* string_ref_variable;

					OptionParser::Option* ref_option;

					// internal : vrai pour les options dérivées créés par le programme mais
					// n'ayant pas de sens du point de vue de l'utilisateur.
					// La internal=true évite :
					// - l'affichage de la valeur de l'option;
					// - l'affichage de l'aide pour cette option.
					// Par exemple la valeur de --debug n'a pas de sens car c'est la valeur
					// de --verbose-level qui doit être affichée.
					//
					bool internal;

					// Liste des clés disponibles pour une option donnée
					// (e.g. -n, --nbiter, --max_b_iterations...)
					std::vector<std::string>* keys;

					// Liste des option de type ABBREV qui sont des abréviation de cette
					// objet
					// e.g. L'option associée à la clé --red (avec sa valeur "red") sera
					// rangée dans dans les listes abbrevs de l'option associée à --color.
					std::vector<Option*>* abbrevs;

					// Le constructeur
					// OptionParser::Option(OptionParser::OptionParser parser, std::string id) {xxx} // BUG déclaration
					Option(std::string id, SpecType type = STRING);
					~Option(void);
					Option* set_desc(std::string desc);
					Option* set_required(bool req=true);
					Option* add_alias(std::string key);
					Option* add_key(std::string key);  // idem que add_alias
					// Le paramètre internal est nécessaire pour créer les options de type
					// SWITCH ; car se serait très lourd de récupérer la référence sur l'option de
					// type ABBREV crée par cette méthode add_abbrev de façon à en modifier
					// l'attribut internal après sa création par la classe OptionParser::OptionParser.
					Option* add_abbrev(std::string key, std::string value, bool internal=false);
					// quelque facilités (int double) pour éviter de taper une chaine.
					// Mais attention : ne pas créer add_abbrev(std::string key, double value)
					// sinon elle est appelé à mon insu !?
					//
					Option* add_abbrev(std::string key, int value, bool internal=false);
					Option* add_abbrev(std::string key, double value, bool internal=false);

					// Dans le cas d'un paramètre de type std::string, on prévoit le
					// cas où le type souhaité est donné par l'attribut type.
					// Si l'attribut type n'est pas STRING, on procède à une conversion.
					// Ceci permet de passer une chaine au moment de l'analyse des arguments
					// aueaue soit le type final.
					//
					// ATTENTION la variante set_value(char const*) est nécessaire sinon bug!!!
					// (i.e. aucune des méthodes set_value(...) ne serait appelée)
					// google : stackoverflow c-style-strings-to-stdstring-conversion-clarification
					Option* set_value(char const* chars);
					Option* set_value(const std::string& value);
					// retourne une représentation chaine du type de l'objet courant.
					static std::string type_to_s(SpecType type);
					std::string type_to_s(void);
					// retourne une représentation chaine de la valeur courante
					std::string value_to_s(void);
					// Affiche l'état détaillé de l'option
					std::string inspect(void);
					// Affiche l'aide pour cette option
					std::string get_help(void);
			};

			// liste ordonnée des options
			std::vector<Option*>* options;

			// Pour accéder facilement à une option à partir d'une clé
			// Dans un premier temps : accès à l'option à partir de l'id (~ name)
			std::map<std::string, Option*>* map_options;

			OptionParser(int argc, char *argv[]);
			virtual ~OptionParser(void);

		private:
			Option* add_option(std::string id, SpecType type=STRING);

		public:
			Option* add_bool_option(std::string id, bool& var);
			Option* add_int_option(std::string id, int& var);
			Option* add_double_option(std::string id, double& var);
			Option* add_string_option(std::string id, std::string& var);
			Option* add_abbrev_option(std::string id, std::string ref_option_id, std::string value); // XXX A VIRER QUAND SERA FINI
			Option* add_switch_option(std::string id, bool& var);

			// Insère du textet au milieur de l'aide sur les options.
			// On peut insérer un simple saut de ligne en ne passant aucun paramètre.
			Option* add_doc(std::string desc = "");
			void set_params_vector(std::vector<std::string>& user_params);

			// Retourne l'objet OptionParser::Option associé à une clé.
			// Cette clé **doit** exister sinon std::exit !
			Option* get(std::string id);
			bool key_exists(std::string id);

			// affiche pour chaque clé sa clé de référence (i.e. l'id de l'option associée)
			std::string map_options_to_s(void);
			void on_error(std::string handler);
			void send_error(std::string txt);

			// Complète la structure d'options et effectue quelques vérifications
			// sur la cohérence au sein de l'ensemble des options.
			// e.g. deux clés identiques ne doivent pas déclarées, ...
			// ATTENTION : PEUT EFFECTUER UN EXIT EN CAS DE PROBLÈMES
			void close(void);
			virtual void parse(void);

			// Retourne une nouvelle chaine nettoyée de certains caractères sur les bords
			// Par défaut, ces caractères sont espaces ou tabulation
			static std::string ltrim_string(std::string str, std::string trim_chars=" \t\r\n");
			virtual std::string inspect(void);
			virtual std::ostream& print_dump(std::ostream& os = std::cout);

			/// // Affiche la syntaxe de l'appli et la description des options
			/// virtual
			/// std::ostream& print_syntaxe_abstract(std::ostream& os = std::cout) {
			///     os << "Syntaxe :\n";
			///     os << "  "<< this->exename << " [options...] [--] [file1 [file2 [...]]]\n";
			///     os << "  "<< this->exename << " --help  pour en savoir plus\n";
			///     return os;
			/// }
			// Affiche la syntaxe de l'appli et la description des options
			virtual std::ostream& print_syntaxe(std::ostream& os = std::cout);

			// affiche la valeur courante des options
			// - affiche les valeurs par défaut avant l'appel à la méthode parse()
			// - affiche dans l'ordre de la déclaration
			virtual std::ostream& print_values(std::ostream& os = std::cout);

			// get_help: retourne l'aide de l'application.
			// À REDÉFINIR PAR LA CLASSE FILLE
			virtual std::ostream& print_help(std::ostream& os = std::cout);

			// pre_parse_hook: permet un prétraitemenet avant d'extraire les options
			// (extraction du premier paramètre à la git ou cvs, ...)
			// À REDÉFINIR PAR LA CLASSE FILLE
			virtual void pre_parse_hook(void);

			// post_parse_hook: assure les vérification supplémentaire sur les options extraites
			// (validité des valeurs, cohérence pour les option interdépendantes, ...)
			// À REDÉFINIR PAR LA CLASSE FILLE
			virtual void post_parse_hook(void);

			static OptionParser* test(int argc, char *argv[]);
	};
}

#endif /* end of include guard */

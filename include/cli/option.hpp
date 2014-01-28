#ifndef VERIF_OPTION_USE_HPP
#define VERIF_OPTION_USE_HPP

#include <vector>
#include <string>
#include <iostream>

#include "cli/types.hpp"

namespace cli {
	class FromYaml {
		public:
			FromYaml(const std::string &str);
			~FromYaml(void);

			Config Get(void);
		private:
			Config cfg;
	};

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
			ArgumentParser(void)
			: exit_code(EXIT_SUCCESS)
			{
			}
			ArgumentParser(const int argc, const char** argv)
			: exit_code(EXIT_SUCCESS)
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

			cli::Config Parse(const int argc, const char** argv)
			{
				this->add_args(argc, argv);
				return this->Parse();
			}
			cli::Config Parse(void)
			{
				cli::Config parsed;
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

	typedef ArgumentParser<> DefaultParser;
}

#endif /* end of include guard */

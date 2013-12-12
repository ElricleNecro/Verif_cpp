#ifndef YAML_CONFIG_HPP
#define YAML_CONFIG_HPP

#include <yaml-cpp/yaml.h>

namespace cfg {
	class Config {
		public:
			Config(const std::string& fname)
			{
				this->config = YAML::LoadFile(fname.c_str());
			}
		private:
			YAML::Node config;
	};
}

#endif /* end of include guard */

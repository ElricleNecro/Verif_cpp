#ifndef VERIF_PLUGINS_PLUGINS_H
#define VERIF_PLUGINS_PLUGINS_H

#include <dlfcn.h>

#include <string>
#include <iostream>
#include <stdexcept>

#include "define.h"
#include "plugins/exceptions.hpp"

namespace plugins {
	typedef Version GetVersion(void);
	typedef int GetVersionMajor(void);
	typedef int GetVersionMinor(void);

	class Plugins {
		public:
			Plugins(void);
			Plugins(const Plugins& old);
			Plugins(const std::string &plugins_name);
			Plugins& operator=(const Plugins& old);
			void SetFile(const std::string& fn);
			bool VerifyAPI(void);
			virtual ~Plugins(void);

			//... RegisterFunc(const char *func_name);
			template<typename TSignature>
			TSignature* GetFunction(const std::string &functionName)
			{
				dlerror(); // clear error value

				void *functionAddress = dlsym(
						this->id,
						functionName.c_str()
				);

				const char *error = dlerror(); // check for error
				if(error != NULL) {
					throw std::runtime_error(std::string("Could not find exported function: ") + error);
				}

				return reinterpret_cast<TSignature*>(functionAddress);
			}

		public:
			Version version;

		private:
			void *id;
			std::string plug_name, plug_version;

			void load(void);
	};
} // namespace plugins

#endif /* end of include guard */

//vim:spelllang=

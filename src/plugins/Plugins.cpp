#include "plugins/Plugins.hpp"

namespace plugins {
	//--------------------------------------------------------------------------------
	// Plugins class
	//--------------------------------------------------------------------------------
	Plugins::Plugins(void) : id(NULL), plug_name()
	{
	}

	Plugins::Plugins(const Plugins& old) : id(NULL), plug_name(old.plug_name)
	{
		this->load();
	}

	Plugins::Plugins(const std::string &fname) : id(NULL), plug_name(fname)
	{
		this->load();
	}

	Plugins& Plugins::operator=(const Plugins& old)
	{
		this->plug_name = old.plug_name;
		if( id != NULL )
			dlclose(this->id);
		this->load();
		return *this;
	}

	Plugins::~Plugins(void)
	{
		//std::cerr << this->id << std::endl;
		if(this->id != NULL)
			dlclose(this->id);
	}

	void Plugins::load(void)
	{
		std::cerr << "Loading " << plug_name << "!" << std::endl;
		this->id = dlopen(this->plug_name.c_str(), RTLD_LAZY);
		if( this->id == NULL )
		{
			throw PlugInNotFound(dlerror());
		}
	}

	void Plugins::SetFile(const std::string& fn)
	{
		this->plug_name = fn;
		this->load();
	}

	bool Plugins::VerifyAPI(void)
	{
		try {
			GetVersion *api_ver = this->GetFunction<GetVersion>("GetAPIVersion");
			this->version = api_ver();
		} catch( const std::runtime_error &e) {
			std::cerr << e.what() << std::endl;

			return false;
			//throw PlugInWrongAPI("GetAPIVersion function not found!");
		}

		return true;
	}

} // namespace plugins

#include "io/gadget.hpp"

namespace io {
	namespace gadget {
		Read::Read(const char *_fname, const int _ftype, const int _num) : part(), ftype(_ftype), NbPart(0), numfile(_num), fname(_fname), pot(false), acc(false), entropy(false), ts(false)
		{
		}

		Read::Read(std::string _fname, const int _ftype, const int _num) : part(), ftype(_ftype), NbPart(0), numfile(_num), fname(_fname), pot(false), acc(false), entropy(false), ts(false)
		{
		}

		types::Particules Read::GetPart(void)
		{
			return this->part;
		}

		int Read::GetNbPart(void) const
		{
			return this->NbPart;
		}

		int Read::GetFileType(void) const
		{
			return this->ftype;
		}

		void Read::SetFileType(const int type)
		{
			this->ftype = type;
		}

		int Read::GetNumFile(void) const
		{
			return this->numfile;
		}

		void Read::SetNumFile(const int num)
		{
			this->numfile = num;
		}

		std::string Read::GetFileName(void) const
		{
			return this->fname;
		}

		iogadget::_particule_data Read::operator[](const int index) const
		{
			return this->part[index];
		}

		iogadget::_particule_data& Read::operator[](const int index)
		{
			return this->part[index];
		}

		void Read::ReadPotential(void)
		{
			this->pot = true;
		}

		void Read::ReadTimeStep(void)
		{
			this->ts = true;
		}

		void Read::ReadRateEntropy(void)
		{
			this->entropy = true;
		}

		void Read::ReadAcceleration(void)
		{
			this->acc = true;
		}

		void Read::NotReadPotential(void)
		{
			this->pot = false;
		}

		void Read::NotReadTimeStep(void)
		{
			this->ts = false;
		}

		void Read::NotReadRateEntropy(void)
		{
			this->entropy = false;
		}

		void Read::NotReadAcceleration(void)
		{
			this->acc = false;
		}

		void Read::Read_File(void)
		{
			int NbPart = 0;
			iogadget::Particule tmp = NULL;

			if( this->ftype == 1 )
				this->part = Gadget_Read_format1(this->fname.c_str(), this, this->numfile, this->pot, this->acc, this->entropy, this->ts);
			else if( this->ftype == 2 )
				this->part = Gadget_Read_format2(this->fname.c_str(), this, this->numfile, this->pot, this->acc, this->entropy, this->ts);
			else
				throw Unsupported("File type unsupported!");

			if( this->part == NULL )
				throw std::runtime_error("Memory error!");

			for(int i=0; i<6; i++)
				NbPart += this->npart[i];

		}

		std::ostream& operator<<(std::ostream &obj, io::gadget::Read const& print)
		{
			obj << "Gadget file `" << print.fname << "` (" << print.num_files << "):" << std::endl;
			obj << "Simulation basics parameters:" << std::endl;

			obj << "\t-Particules number: [" << print.npart[0];
			for(int i=1; i<6; i++)
				obj << ", " << print.npart[i];
			obj << "]" << std::endl;

			obj << "\t-Particules masses: [" << print.npart[0];
			for(int i=1; i<6; i++)
				obj  << ", " << print.npart[i];
			obj << "]" << std::endl;

			obj << "\t-Time: " << print.time << ", redshift: " << print.redshift << std::endl;
			obj << "\t-BoxSize: " << print.BoxSize << std::endl;

			obj << "Physical Gadget Parameter:" << std::endl;
			obj << "\t-SFR:                  "  << print.flag_sfr << std::endl;
			obj << "\t-Feed back:            "  << print.flag_feedback << std::endl;
			obj << "\t-Cooling:              "  << print.flag_cooling << std::endl;
			obj << "\t-Metallicity:          "  << print.flag_metals << std::endl;
			obj << "\t-Stellar age:          "  << print.flag_stellarage << std::endl;
			obj << "\t-Entropy instead of u: "  << print.flag_entropy_instead_u << std::endl;

			obj << "Cosmological parameters:"   << std::endl;
			obj << "\t-Omega 0:          "      << print.Omega0 << std::endl;
			obj << "\t-Omega Lambda:     "      << print.OmegaLambda << std::endl;
			obj << "\t-Hubble parameter: "      << print.HubbleParam << std::endl;

			return obj;
		}

		Read::operator io::types::Particules(void)
		{
			return this->part;
		}

		Read::operator iogadget::Particule(void)
		{
			return this->part.GetParticule();
		}
	}
}


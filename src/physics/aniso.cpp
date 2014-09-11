#include "physics/aniso.hpp"

namespace Stats {
	Anisotropy::Anisotropy(
			io::types::Particules part,
			const int NbPart,
			const double r_min,
			const double r_max,
			const int bin,
			const double r_norm
			) : Histogram(part, NbPart, r_min, r_max, bin, r_norm)
	{
	}

	Anisotropy::~Anisotropy(void)
	{
	}

	const char* Anisotropy::GetName(void)
	{
		return "Anisotropy";
	}

	double Anisotropy::GetAnisotropy(void)
	{
		return this->Coeff;
	}

	void Anisotropy::Calcul(void)
	{
	}

	void Anisotropy::OldCalcul(void)
	{
		double *vr    = NULL,
		*vt    = NULL,
		*vp    = NULL,
		*vrn   = NULL,
		*vtn   = NULL,
		*vpn   = NULL,
		cvr    = 0.0,
		cvt    = 0.0,
		cvp    = 0.0,
		cvrm   = 0.0,
		cvpm   = 0.0,
		cvtm   = 0.0,
		r      = 0.0;
		int    *nb    = NULL,
		       ind    = 0;

		vr    = new double[this->NbBin];
		vt    = new double[this->NbBin];
		vp    = new double[this->NbBin];
		vrn   = new double[this->NbBin];
		vtn   = new double[this->NbBin];
		vpn   = new double[this->NbBin];

		nb    = new int[this->NbBin];

		for(int i = 0; i < this->NbBin; i++)
		{
			vr[i]  = 0.;
			vt[i]  = 0.;
			vp[i]  = 0.;
			vrn[i] = 0.;
			vtn[i] = 0.;
			vpn[i] = 0.;

			nb[i]  = 0;
		}

		//Calcul de la moyenne :
		for (int i = 0; i < this->NbPart; i++)
		{
			double svr, vtheta, vphi, theta, phi;

			if(this->part[i].Pos[2]>0.0)
				theta = std::asin(sqrt(this->part[i].Pos[0]*this->part[i].Pos[0] + this->part[i].Pos[1]*this->part[i].Pos[1])/Utils::rayon(&this->part[i]));
			else
				theta = std::acos(-1.0) - std::asin(sqrt(this->part[i].Pos[0]*this->part[i].Pos[0] + this->part[i].Pos[1]*this->part[i].Pos[1])/Utils::rayon(&this->part[i]));
			phi   = std::atan2(this->part[i].Pos[0], this->part[i].Pos[1]);

			svr = this->part[i].Vit[0] * std::cos(theta)*std::cos(phi)
				+ this->part[i].Vit[1] * std::cos(theta)*std::sin(phi)
				- this->part[i].Vit[2] * std::sin(theta);
			/* \f$ \frac{d\phi}{dt} = \frac{1}{y} \left( x \frac{x \frac{dx}{dt} + y \frac{dy}{dt}}{x^2 + y^2} - \frac{dx}{dt} \right) \f$ */
			vphi = -this->part[i].Vit[0] * std::sin(phi) + this->part[i].Vit[1] * std::cos(phi);
			/* \f$ \frac{d\theta}{dt} = \frac{1}{\sqrt{x^2 + y^2}} \left( \frac{dr}{dt}\frac{z}{r} - \frac{dz}{dt} \right) \f$ */
			vtheta = this->part[i].Vit[0] * std::cos(phi)*std::sin(theta)
				+ this->part[i].Vit[1] * std::sin(phi)*std::sin(theta)
				+ this->part[i].Vit[2] * std::cos(theta);

			cvr += svr;
			cvp += vphi;
			cvt += vtheta;

			r = Utils::rayon(&part[i]);
			for(int j = 0; j<this->NbBin; j++)
				if( r > this->r[j] && r <= this->r[j+1] )
				{
					vr[j] += svr;
					vp[j] += vphi;
					vt[j] += vtheta;
					nb[j] ++;
					break;
				}
		}
		for (int i = 0; i < NbBin; i++) {
			vr[i] /= ( (nb[i]!=0)?nb[i]:1 );//this->NbPart;
			vt[i] /= ( (nb[i]!=0)?nb[i]:1 );//this->NbPart;
			vp[i] /= ( (nb[i]!=0)?nb[i]:1 );//this->NbPart;
		}
		cvr /= this->NbPart;
		cvt /= this->NbPart;
		cvp /= this->NbPart;

		ind = 0;
		//Calcul de l'écart-type :
		for (int i = 0; i < this->NbPart; i++)
		{
			double svr, vtheta, vphi, theta, phi;

			if(this->part[i].Pos[2]>0.0)
				theta = std::asin(sqrt(this->part[i].Pos[0]*this->part[i].Pos[0] + this->part[i].Pos[1]*this->part[i].Pos[1])/Utils::rayon(&this->part[i]));
			else
				theta = std::acos(-1.0) - std::asin(sqrt(this->part[i].Pos[0]*this->part[i].Pos[0] + this->part[i].Pos[1]*this->part[i].Pos[1])/Utils::rayon(&this->part[i]));
			phi   = std::atan2(this->part[i].Pos[0], this->part[i].Pos[1]);

			svr = this->part[i].Vit[0] * std::cos(theta)*std::cos(phi)
				+ this->part[i].Vit[1] * std::cos(theta)*std::sin(phi)
				- this->part[i].Vit[2] * std::sin(theta);
			vphi = -this->part[i].Vit[0] * std::sin(phi) + this->part[i].Vit[1] * std::cos(phi);
			vtheta = this->part[i].Vit[0] * std::cos(phi)*std::sin(theta)
				+ this->part[i].Vit[1] * std::sin(phi)*std::sin(theta)
				+ this->part[i].Vit[2] * std::cos(theta);

			cvrm += pow(svr - cvr, 2.0);
			cvpm += pow(vphi - cvp, 2.0);
			cvtm += pow(vtheta - cvt, 2.0);

			r = Utils::rayon(&part[i]);
			for(int j = 0; j<this->NbBin; j++)
				if( r > this->r[j] && r <= this->r[j+1] )
				{
					vrn[j] += pow(svr - vr[j], 2.0);
					vpn[j] += pow(vphi - vp[j], 2.0);
					vtn[j] += pow(vtheta - vt[j], 2.0);
					break;
				}
		}
		for (int i = 0; i < NbBin; i++) {
			vrn[i] /= ( (nb[i]-1>0)?(nb[i]-1):1 );
			vtn[i] /= ( (nb[i]-1>0)?(nb[i]-1):1 );
			vpn[i] /= ( (nb[i]-1>0)?(nb[i]-1):1 );
		}
		cvrm /= (this->NbPart-1.0);
		cvtm /= (this->NbPart-1.0);
		cvpm /= (this->NbPart-1.0);


		//Calcul de l'anisotropie 1 - 2*sig_r^2 / (sig_t^2 + sig_p^2) :
		for (int i = 0; i < NbBin; i++) {
			if( vrn[i] != 0.0 )
				this->bins[i] = 1.0 - ((vtn[i]) + (vpn[i])) / (2.0 * (vrn[i]));
			else
				this->bins[i] = 0.0;
		}

		Coeff = 1.0 - ( (cvtm) + (cvpm) ) / (2.0 * (cvrm));

		delete[] nb;
		delete[] vr;
		delete[] vt;
		delete[] vp;
		delete[] vrn;
		delete[] vtn;
		delete[] vpn;
	}

} /* Stats */

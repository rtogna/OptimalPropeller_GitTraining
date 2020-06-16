//============================================================================
// Name        : S-Rotor
// Author      : Ettore Saetta
// Version     : 3.0
// Copyright   : 
// Description : Evaluate the optimal a and a' distributions along the propeller blade
//				Hub Radius added
//============================================================================

// PROVA DI MODIFICA CODICE

#include <iostream>
#include <fstream>
#include <cmath>
#include "gnuplot.h"
#include <stdio.h>
#include <termios.h>

using namespace std;

// Global Variables
int Stations;
double long Vinf, n, D, h, R, J, dStations;
double long r[50], chi[50];

// Function Prototypes
double long a_distribution (double long, double long);
void gnuPrint (double long[], double long[], double long[], double long[], double long[]);
void Pause();
void Print_external_file(double long[], double long[]);

//
int main()
{
	int i, i_hub, iter;
	double long Ct, rhub, rs_hub, Omega;
	double long dCp[50]={0};

	double long w_old, w_0, w[50]={0};
	double long a_new[50]={0}, a_old[50]={0}, a_0[50]={0}, ap_old[50]={0}, dCt_new[50]={0}, dCt_old[50]={0}, dCt_0[50]={0};
	double long Ct_new, Ct_old, Ct_0, eps, err_old, err_0, err_new, DeltaP[50], T, Ct_Renard, Cp, eta;
	double long F[50]={0}, N;	char Prandtl, ext_file;	bool corr, err;

	double long w_new;

	cout<<"%%%%      Optimal Rotor 3.0      %%%%"<<endl;
	cout<<"Evaluate optimal dCt/drs and dCp/drs."<<endl;
	cout<<"Output file in SU2 configuration file version."<<endl;

	Pause();

	do{
		cout << "Number of radial stations: ";
		cin >> Stations;
		cout << endl;
		if (Stations > 50) {
			cout << "ERROR: Maximum number of stations exceeded. Use a value < 50 " << endl << endl;
			err = true;
		}
		else {
			dStations = Stations;
			err = false;
		}
	} while(err != false);

	cout << "Ct: ";
	cin >> Ct;
	cout << endl;

	cout << "R: ";
	cin >> R;
	cout << endl;

	cout << "r_hub: ";
	cin >> rhub;
	cout << endl;

	cout << "J: ";
	cin >> J;
	cout << endl;


	cout << "Vinf: ";
	cin >> Vinf;
	cout << endl;

	cout << "Using Prandtl correction? (y/n): ";
	cin >> Prandtl;
	cout << endl;

	if (Prandtl == 'y'){
		cout << "Number of blades N: ";
		cin >> N;
		cout << endl;

		corr = true;
	}
	else {corr = false;}

	cout << "Generate external txt file? (y/n): ";
	cin >> ext_file;
	cout << endl;

	rs_hub = rhub/R;

	for(i=1;i<Stations+1;i++)
	{
		r[i-1]=i/dStations;
		if (r[i-1] <= rs_hub) {i_hub = i-1;}
	}


	D = 2*R;
	n = Vinf/(D*J); //Giri/s
	Omega = n*2*M_PI;

	if (corr == true){
		for(i=0;i<Stations;i++)
		{
			F[i] = (2/M_PI)*acos(exp(-0.5*N*(1-r[i])*sqrt(1+pow(Omega*R/Vinf,2))));
		}
	}
	else{
		for(i=0;i<Stations;i++)
			F[i] = 1.0;
	}

	for(i=0;i<Stations;i++)
	{
		chi[i]=Omega*r[i]*R/Vinf;
	}

	eps = 5E-20;
	h = (1.0/Stations);

	for(i=0;i<Stations;i++)
	{
		w[i] = (2/Vinf*Vinf)*((-1/Vinf)+sqrt(1+((pow(D,4)*(Ct)*n*n)/(Vinf*Vinf*M_PI*r[i]))));
	}
	w_0 = 0.0;
	for(i=0;i<Stations;i++)
	{
		w_0 += w[i];
	}
	w_0 = w_0/(Vinf*Stations);

	for(i=0;i<Stations;i++)
	{
		a_0[i] = a_distribution(w_0*F[i],chi[i]);
	}

	for(i=0;i<Stations;i++)
	{
		dCt_0[i]= M_PI*J*J*r[i]*(1+a_0[i])*a_0[i];
	}

	Ct_0 = 0;
	for(i=i_hub;i<Stations;i++)
	{
		Ct_0 += h*dCt_0[i];
	}

	err_0 = Ct_0-Ct; cout<<endl<<"ERROR CONVERGENCE HISTORY:"<<endl<<err_0<<endl;

	w_old = w_0 + 0.1;
	for(i=0;i<Stations;i++)
	{
		a_old[i] = a_distribution(w_old*F[i],chi[i]);
	}

	for(i=0;i<Stations;i++)
	{
		dCt_old[i]= M_PI*J*J*r[i]*(1+a_old[i])*a_old[i];
	}

	Ct_old = 0;
	for(i=i_hub;i<Stations;i++)
	{
		Ct_old += h*dCt_old[i];
	}

	err_old = Ct_old - Ct;	cout<<err_old<<endl;

	///////////////////////////////////////// ITERAZIONI /////////////////////////////////////////
	iter = 2;
	err_new = err_old;
	while(fabs(err_new) >= eps && err_0!=err_old)
	{
		iter += 1;
		w_new = (w_old*err_0 - w_0*err_old)/(err_0 - err_old);
		for(i=0;i<Stations;i++)
		{
			a_new[i] = a_distribution(w_new*F[i],chi[i]);
		}

		for(i=0;i<Stations;i++)
		{
			dCt_new[i]= M_PI*J*J*r[i]*(1+a_new[i])*a_new[i];
		}

		Ct_new = 0;
		for(i=i_hub;i<Stations;i++)
		{
			Ct_new += h*dCt_new[i];
		}

		err_new = Ct_new - Ct;

		cout<<err_new<<endl;

		err_0 = err_old;
		err_old = err_new;

		w_0 = w_old;
		w_old = w_new;

	}

	for(i=0;i<Stations;i++)
	{
		a_old[i] = a_distribution(w_new*F[i],chi[i]);
		ap_old[i] = (w_new*F[i])*((1+w_new*F[i])/(chi[i]*chi[i]+pow(1+w_new*F[i],2)));
	}

	for(i=0;i<Stations;i++)
	{
		dCt_old[i]= M_PI*J*J*r[i]*(1+a_old[i])*a_old[i];
	}

	for(i=0;i<Stations;i++)
	{
		dCp[i] = (R*4*M_PI/(pow(n,3)*pow(D,5)))*(pow(Vinf,3)*pow(1+a_old[i],2)*a_old[i]*r[i]*R+pow(Omega,2)*Vinf*(1+a_old[i])*pow(ap_old[i],2)*pow(r[i]*R,3));
		//		dCp[i] = ((4*M_PI*R*R/(pow(n,3)*pow(D,5)))*pow(Vinf,3)*pow(1+a_old[i],2)*a_old[i]*r[i]) + ((4*M_PI*pow(R,4)/(pow(n,3)*pow(D,5)))*Omega*Omega*Vinf*(1+a_old[i])*pow(ap_old[i],2)*pow(r[i],3));
	}

	Cp = 0;
	for(i=i_hub;i<Stations;i++)
	{
		Cp += h*dCp[i];
	}

	Ct_old = 0;
	for(i=i_hub;i<Stations;i++)
	{
		Ct_old += h*dCt_old[i];
	}

	for(i=0;i<Stations;i++){
		DeltaP[i] = (dCt_old[i])*(2*Vinf*Vinf)/(J*J*M_PI*r[i]);
		//	DeltaP[i] = 2*a_old[i]*Vinf*Vinf*(1+a_old[i]);
		//	DeltaP[i] = dCt_old[i]*(4*Vinf*Vinf*R*R)/(J*J);	// dT/drs
	}

	T = 0.0;
	for(i=i_hub;i<Stations;i++)
	{
		T += 2*M_PI*r[i]*R*R*h*DeltaP[i];
	}
	Ct_Renard = (T)/(n*n*pow(D,4));
	eta = J*(Ct_old/Cp);

	cout<<endl<<"%%%%%%%%%%%%%%%%%%%%%%%%% CHECK OUTPUT VALUES %%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
	cout<<endl<<"		dCt integral: "<<Ct_old<<endl;
	cout<<endl<<"		dCt evaluated passing through Delta P: "<<Ct_Renard<<endl;
	cout<<endl<<"		dCp integral: "<<Cp<<endl;
	cout<<endl<<"		Thrust over Density (T/rho): "<<T<<" [N*m^3/kg]"<<endl;
	cout<<endl<<"		Efficiency eta: "<<eta<<endl;
	cout<<endl<<"		w0/Vinf: "<<w_new<<endl;
	cout<<endl<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;


	///////////////////////////////////////// GNUPLOT /////////////////////////////////////////

	gnuPrint(a_old, ap_old, dCt_old, dCp, F);

	GnuplotPipe gp;
	gp.sendLine("set multiplot layout 3,1");
	gp.sendLine("set grid");
	gp.sendLine("set key top left");
	gp.sendLine("set xlabel 'r/R'");
	gp.sendLine("plot 'GnuOut.dat' u 2:5 title 'dCt/drs' w l , 'GnuOut.dat' u 2:6 title 'dCp/drs' w l");
	gp.sendLine("set key top right");
	gp.sendLine("set xlabel 'r/R'");
	gp.sendLine("plot 'GnuOut.dat' u 2:3 title 'a' w l , 'GnuOut.dat' u 2:4 title 'a^1' w l");
	gp.sendLine("set xlabel 'r/R'");
	gp.sendLine("set key top right");
	gp.sendLine("plot 'GnuOut.dat' u 2:7 title 'F' w l");
	gp.sendLine("unset multiplot");
	///////////////////////////////////// SCRITTURA FILE /////////////////////////////////////
	if (ext_file == 'y'){
		ofstream SaveFile("Input_ActDisk_SU2");

		SaveFile << "ACTDISK_TYPE = VARIABLES_JUMP" << endl
				<< "ACTDISK_JUMP = DIFFERENCE" << endl
				<< "ACTDISK_FILE = YES" << endl
				<< "ACTDISK_FILE_NAME = InputActDisk" << endl
				<< "MARKER_ACTDISK = ( , )" << endl;
		SaveFile.close();
	}
	else{
		ofstream SaveFile("Input_ActDisk_SU2");

		SaveFile << "ACTDISK_TYPE = VARIABLES_JUMP" << endl
				<< "ACTDISK_JUMP = DIFFERENCE" << endl
				<< "MARKER_ACTDISK = ( , )" << endl
				<< "ACTDISK_CENTER = ( , , )" << endl
				<< "ACTDISK_AXIS = ( , , )" << endl;

		SaveFile << "ACTDISK_ADV_RATIO = " << J << endl
				<< "ACTDISK_RADIUS = " << R << endl
				<< "ACTDISK_NEL_DISTR = " << Stations << endl;

		SaveFile		<< "ACTDISK_RAD = (";

		for(i=0;i<49;i++)
		{
			SaveFile << r[i]<<", ";
			//		if((i%5 == 0.0)&&(i!=0.0)) SaveFile << endl;
		}
		SaveFile << r[49] << ")"<<endl;

		SaveFile		<< "ACTDISK_DCT = (";
		for(i=0;i<49;i++)
		{
			SaveFile << dCt_old[i]<<", ";
			//		if((i%5 == 0.0)&&(i!=0.0)) SaveFile << endl;
		}
		SaveFile << dCt_old[49] << ")"<<endl;

		SaveFile		<< "ACTDISK_DCP = (";
		for(i=0;i<49;i++)
		{
			SaveFile << dCp[i]<<", ";
			//		if((i%5 == 0.0)&&(i!=0.0)) SaveFile << endl;
		}
		SaveFile << dCp[49] << ")"<<endl;

		SaveFile		<< "ACTDISK_DCR = (";
		for(i=0;i<49;i++)
		{
			SaveFile << 0 <<", ";
			//		if((i%5 == 0.0)&&(i!=0.0)) SaveFile << endl;
		}
		SaveFile << 0 << ")"<<endl;

		SaveFile.close();
	}
	cout<<"SU2 file generated..."<<endl;

	if (ext_file == 'y') Print_external_file(dCt_old, dCp);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
double long a_distribution (double long w0, double long Chi)
{
	double long a;

	a = ((w0)*(Chi*Chi))/(Chi*Chi+pow((1+(w0)),2));
	return a;
}

void gnuPrint (double long a[], double long ap[], double long dCt[], double long dCp[], double long F[])
{
	ofstream SaveFile("GnuOut.dat");

	for(int i=0;i<Stations;i++)
	{
		SaveFile << chi[i] << "	" << r[i] << "	" << a[i] << "	" << ap[i] << "	" << dCt[i] << "	" << dCp[i] << "	" << F[i] << endl;
	}
	SaveFile.close();
}

void Print_external_file(double long CTrs[],double long CPrs[])
{
	ofstream SaveFile("InputActDisk");
	SaveFile << "NAME= " << endl
			<< "CENTER= " << endl
			<< "RADIUS= " << R << endl
			<< "ADV_RATIO= " << J << endl
			<< "NROW= " << Stations << endl
			<< endl;

	for(int i=0;i<Stations;i++)
	{
		SaveFile << r[i] << "	" << CTrs[i] << "	" << CPrs[i] << "	" << 0.0 << endl;
	}
	SaveFile.close();
}

void Pause()
{
	cout<<endl<<endl<<"Press ENTER to continue..."<<endl;
	while(getchar() != '\n');
}


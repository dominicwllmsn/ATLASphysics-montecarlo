// James Cummins & Dominic Williamson 26/11/18

///////////////////////////////////////////////////////////////////////////////////////////////
// Throughout this program, you can use the variable "choice" as the name of the decay chain //
// you're interested in, e.g. running the python script "MultiRun.py" with "Zee" as an input //
// changes any mention of choice to the word "Zee" in this file.                             //
///////////////////////////////////////////////////////////////////////////////////////////////

// Header guard to ensure file is imported properly.
#ifndef MyAnalysis
#define MyAnalysis

// Include the file that lets the program know about the data.
#include "Headers/mini.h"
#include "TLorentzVector.h"
#include "math.h"

// This function is where you "book" your histograms.
// N.B. Be sure to declare any histograms in MyAnalysis.h, or it won't run correctly!  
void mini::Book(std::string choice){
		// 1-dimensional histograms (TH1F) are defined like: h1 = new TH1F("My_Histogram_Name", "Title", number of bins, x_min, x_max);
		
		// Example: booking of a histogram (to plot number of leptons per event)
  	h_lep_n = new TH1F("Lepton_no", "Number of Leptons", 10, -0.5, 9.5);
  	
  	// Example: using "choice" to name the histogram and its title.
  	// TString::Format is a function that takes a string of your choosing and substitutes it for %s in another string.
  	// For example, in this code, if choice == "Zee", then the name of the histogram would be "Zee_pt" and the title "Zee: p_t distribution". 
  	// N.B. You should try and use this format when you get on to analysing multiple datasets!
  	h_lep_type = new TH1F("type", TString::Format("%s: Lepton type",choice.c_str()) , 10, 10.5, 20.5);
   	//h_lep_cone = new TH1F("ptcone", TString::Format("%s: Transverse Momentum Cone",choice.c_str()) , 200, -500, 8000);
	
	h_lep_pt = new TH1F("lep_pt", TString::Format("%s: Missing Transverse Energy",choice.c_str()) , 100, 0, 100000);
	} // End of function Book

// This function is where you select events and fill your histograms. It happens once per event.
void mini::FillHist(std::string choice){

	 // Weight calculation --- PLEASE DO NOT CHANGE OR REMOVE!
	 ////////////////////////////////////////////////////////////////////////////////////
         std::unordered_map<std::string, double> ScaleFactor_Lumi = {
		{"Zee", 4.01173e-08},
		{"Zmumu", 4.41736e-08},
		{"Ztautau", 4.273775e-08},
		{"DYeeM08to15", 0.02162},
		{"DYeeM15to40", 0.0212436},
		{"DYmumuM08to15", 0.019266},
		{"DYmumuM15to40", 0.024265},
		{"WenuB",0.27983},
		{"WenuNoB", 0.907435},
		{"WenuNoBJets", 0.350585},
		{"WmunuB", 0.289016},
		{"WmunuNoB", 1.04169},
		{"WmunuNoBJets", 0.328063},
		{"WtaunuB", 0.18807},
		{"WtaunuNoB",0.920297 },
		{"WtaunuNoBJets", 0.343289},
		{"WW", 0.0216398},
		{"WZ", 0.00723487},
		{"ZZ", 0.00663601},
		{"ttLep", 0.0382083},
		{"ttHad", 0.0118436},
		{"ZPrime500", 0.03945},
		{"ggH4lep", 3.1687e-05},
		{"VBFH4lep", 2.84628e-06}
	 };
	 double Scale_factor, Event_weight, weight;

	 if(choice=="Muons" || choice=="Electrons")
		 {
			 weight = 1;
		 }
	 else
		 {
		 	 // Calculate weights:
		   Scale_factor = ScaleFactor_Lumi[choice] * scaleFactor_ELE * scaleFactor_MUON * scaleFactor_TRIGGER;
		 	 Event_weight = scaleFactor_PILEUP * scaleFactor_ZVERTEX * mcWeight;
		 	 weight = Scale_factor * Event_weight;
		 }
     ////////////////////////////////////////////////////////////////////////////////////
     // End of weight calculation

     // Whenever you fill a histogram, you should write histogram->Fill(quantity, weight), as shown in examples below:

	 // Example: Fill histogram with the number of leptons in each event
	 h_lep_n->Fill(lep_n,weight);
	 
	 float m_T;
	 float delta_phi;
         unsigned int lepton_type = 13;
         float ptcone30_limit = 0.;
	 float etcone20_limit = 0.;
	 float mt_limit = 0.;
         float met_et_limit = 30000.;
         float pt_limit = 30000.;
         
         if (lepton_type == 11) 
         {
         	 ptcone30_limit = 2500.;
		 etcone20_limit = 3500.;
		 mt_limit = 50000.;
         }
         
         if (lepton_type == 13)
         {
         	 ptcone30_limit = 2500.;
		 etcone20_limit = 2000.;
		 mt_limit = 40000.;
         }
         
         
         // Loop over each lepton
         for (size_t lep_i=0; lep_i<lep_n; lep_i++)  
         	{	
           		h_lep_type->Fill(lep_type[lep_i],weight);
           		
           		if (lep_n == 1 && lep_type[lep_i] == lepton_type && lep_ptcone30[lep_i] < ptcone30_limit && lep_etcone20[lep_i] < etcone20_limit && met_et > met_et_limit && lep_pt[lep_i] > pt_limit)
           		{
		   		if ((met_phi > 0 && lep_phi[lep_i] > 0) || (met_phi < 0 && lep_phi[lep_i] < 0))
		   		{
		   			delta_phi = abs(met_phi - lep_phi[lep_i]);
		   		}
		   		if ((met_phi > 0 && lep_phi[lep_i] < 0) || (met_phi < 0 && lep_phi[lep_i] > 0))
		   		{	
		   			delta_phi = abs(met_phi - lep_phi[lep_i]);
		   			if (delta_phi > M_PI)
		   			{
		   				delta_phi = abs(abs(met_phi - lep_phi[lep_i]) - 2*M_PI);
		   			}
		   			
		   		}
		   		
		   		m_T = sqrt(2.*met_et*lep_pt[lep_i]*(1. - cos(delta_phi)));
		   		if (m_T < mt_limit) 
			   		{
			   		h_lep_pt->Fill(met_et,weight);
			   		}
           		}
        
         	}
        
}  // End of function FillHist

// This function is where you can control the style elements of your histograms and then write them to a file.
void mini::Style(std::string choice){

	// Example: Change some properties!
	h_lep_n->SetLineColor(6); // Awesome pink line
	//h_lep_cone->GetXaxis()->SetTitle("p_t (MeV)"); // X-axis title
	//h_lep_cone->GetYaxis()->SetTitle("Number of entries"); // Y-axis title
	//h_lep_cone->SetAxisColor(2,"x"); // Set colour of X-axis
	// etc... (See online ROOT documentation!)
	
	// Write the histograms to the file:
	h_lep_n->Write();
	h_lep_type->Write();
	//h_lep_cone->Write();

	h_lep_pt->GetXaxis()->SetTitle("Transverse Energy (MeV)"); // X-axis title
	h_lep_pt->GetYaxis()->SetTitle("Number of Entries"); // Y-axis title
	h_lep_pt->SetAxisColor(2,"x"); // Set colour of X-axis
	// etc... (See online ROOT documentation!)
	
	// Write the histograms to the file:
	h_lep_pt->Write();

} // End of function Style

#endif // End of header guard

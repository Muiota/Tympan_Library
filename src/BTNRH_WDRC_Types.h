
#ifndef _BTNRH_WDRC_TYPES_H
#define _BTNRH_WDRC_TYPES_H

//include "utility/textAndStringUtils.h"

//#include <SdFat_Gre.h>   //for reading and writing settings to SD card
//#include "AccessConfigDataOnSD.h"

#define DSL_MXCH 8  
	
namespace BTNRH_WDRC {

	// Here are the settings for the adaptive feedback cancelation
	class CHA_AFC {
		public: 
			int default_to_active; //enable AFC at startup?  1=active. 0=disabled.
			int afl;	//length (samples) of adaptive filter for modeling feedback path.
			float mu;	//mu, scale factor for how fast the adaptive filter adapts (bigger is faster)
			float rho;	//rho, smoothing factor for estimating audio envelope (bigger is a longer average)
			float eps;	//eps, when est the audio envelope, this is the min allowed level (avoids divide-by-zero)
			
	
			void printAllValues(void) { printAllValues(&Serial); }
			void printAllValues(Stream *s) {
				s->println("CHA_AFC:");
				s->print("    : enable = "); s->println(default_to_active);
				s->print("    : filter length (afl) = "); s->println(afl);
				s->print("    : Adaptation speed (mu) = "); s->println(mu,6);
				s->print("    : Smooothing factor (rho) "); s->println(rho,6);
				s->print("    : Min Tolerance (eps) = "); s->println(eps,6);
			};
		
			
	};
          
	class CHA_DSL  {
	
		public:
			//CHA_DSL(void) {};  //no constructor means that I can use brace initialization
			static const int DSL_MAX_CHAN = DSL_MXCH;    // maximum number of channels
			float attack;               // attack time (ms)
			float release;              // release time (ms)
			float maxdB;                // maximum signal (dB SPL)
			int ear;                    // 0=left, 1=right
			int nchannel;               // number of channels
			float cross_freq[DSL_MXCH]; // cross frequencies (Hz)
			float exp_cr[DSL_MXCH];		// compression ratio for low-SPL region (ie, the expander)
			float exp_end_knee[DSL_MXCH];	// expansion-end kneepoint
			float tkgain[DSL_MXCH];     // compression-start gain
			float cr[DSL_MXCH];         // compression ratio
			float tk[DSL_MXCH];         // compression-start kneepoint
			float bolt[DSL_MXCH];       // broadband output limiting threshold
		
			
			void printAllValues(void) { printAllValues(&Serial); }
			void printAllValues(Stream *s) {
				int last_chan = nchannel;
				//last_chan = DSL_MXCH;
				
				s->println("CHA_DSL:");
				s->print("    : attack (ms) = "); s->println(attack);
				s->print("    : release (ms) = "); s->println(release);
				s->print("    : maxdB (dB SPL) = "); s->println(maxdB);
				s->print("    : ear (0 = left, 1 = right) "); s->println(ear);
				s->print("    : nchannel = "); s->println(nchannel);
				s->print("    : cross_freq (Hz) = ");
				for (int i=0; i< last_chan;i++) { s->print(cross_freq[i]); s->print(", ");}; s->println();
				s->print("    : exp_cr = ");
				for (int i=0; i<last_chan;i++) { s->print(exp_cr[i]); s->print(", ");}; s->println();
				s->print("    : exp_end_knee (dB SPL) = ");
				for (int i=0; i<last_chan;i++) { s->print(exp_end_knee[i]); s->print(", ");}; s->println();
				s->print("    : tkgain (dB) = ");
				for (int i=0; i<last_chan;i++) { s->print(tkgain[i]); s->print(", ");}; s->println();
				s->print("    : cr = ");
				for (int i=0; i<last_chan;i++) { s->print(cr[i]); s->print(", ");}; s->println();
				s->print("    : tk (dB SPL) = ");
				for (int i=0; i<last_chan;i++) { s->print(tk[i]); s->print(", ");}; s->println();
				s->print("    : bolt (dB SPL) = ");
				for (int i=0; i<last_chan;i++) { s->print(bolt[i]); s->print(", ");}; s->println();
			};
			
	
			
	};
	
	typedef struct {
		float alfa;                 // attack constant (not time)
		float beta;                 // release constant (not time
		float fs;                   // sampling rate (Hz)
		float maxdB;                // maximum signal (dB SPL)
		float tkgain;               // compression-start gain
		float tk;                   // compression-start kneepoint
		float cr;                   // compression ratio
		float bolt;                 // broadband output limiting threshold
		//float gcalfa;				// ??  Added May 1, 2018 as part of adaptie feedback cancelation
		//float gcbeta;				// ??  Added May 1, 2018 as part of adaptie feedback cancelation
		float mu;				// 1e-3, adaptive feedback cancelation  (AFC) filter-estimation step size
		float rho;				// 0.984, AFC filter-estimation forgetting factor
		float pwr;				// estimated (smoothed) signal power for modifying mu for AFC
		float fbm;				// some sort of quality metric for AFC (some sort of power residual...smaller is better?)
		
	} CHA_DVAR_t;
	
	typedef struct {
		//int cs;  //chunk size (number of samples in data block)
		//int nc;  //number of frequency bands (channels)
		//int nw;	//number of samples in data window
		int rsz;    //32, ring buffer size (samples)?  Used for AFC
		int rhd;    //last starting point in the ring buffer (head)
		int rtl;    //last ending point in the ring buffer (tail)
		int afl;	//100, adaptive filter length for AFC
		int fbl;	//100, length of simulated feedback (ie, the given feedback impulse response)
		int nqm;    //number of quality metrics:  nqm = (fbl < afl) ? fbl : afl;
		
	} CHA_IVAR_t;
	
/* 	typedef struct {
		float attack;               // attack time (ms), unused in this class
		float release;              // release time (ms), unused in this class
		float fs;                   // sampling rate (Hz), set through other means in this class
		float maxdB;                // maximum signal (dB SPL)...I think this is the SPL corresponding to signal with rms of 1.0
		float tkgain;               // compression-start gain
		float tk;                   // compression-start kneepoint
		float cr;                   // compression ratio
		float bolt;                 // broadband output limiting threshold
	} CHA_WDRC; */
	
	class CHA_WDRC {
		public: 
			float attack;               // attack time (ms), unused in this class
			float release;              // release time (ms), unused in this class
			float fs;                   // sampling rate (Hz), set through other means in this class
			float maxdB;                // maximum signal (dB SPL)...I think this is the SPL corresponding to signal with rms of 1.0
			float exp_cr;				// compression ratio for low-SPL region (ie, the expander)
			float exp_end_knee;			// expansion-end kneepoint
			float tkgain;               // compression-start gain
			float tk;                   // compression-start kneepoint
			float cr;                   // compression ratio
			float bolt;                 // broadband output limiting threshold
			
			
			void printAllValues(void) { printAllValues(&Serial); }
			void printAllValues(Stream *s) {
				s->println("CHA_WDRC:");
				s->print("    : attack (ms) = "); s->println(attack);
				s->print("    : release (ms) = "); s->println(release);
				s->print("    : fs (Hz) = "); s->println(fs);
				s->print("    : maxdB (dB SPL) = "); s->println(maxdB);;
				s->print("    : exp_cr = "); s->println(exp_cr);
				s->print("    : exp_end_knee (dB SPL) = "); s->println(exp_end_knee);
				s->print("    : tkgain (dB) = "); s->println(tkgain);
				s->print("    : tk (db SPL) = "); s->println(tk);
				s->print("    : cr = "); s->println(cr);
				s->print("    : bolt (dB SPL) = "); s->println(bolt);
			};
		
	};
	
}

#endif
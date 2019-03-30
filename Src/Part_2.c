#include "Part_2.h"


/**
* @brief 	Update the ping-pong buffer in accordance with the ADSR enveloppe and the phase
*					of the last buffer (that was just played by the microcontroller).
*

*/
void update_tab(uint32_t *tab, setting* chosen_setting)
{
	int A= instruments[instrument_id]->A;
	int D= instruments[instrument_id]->D;
	int S= instruments[instrument_id]->S;
	int R= instruments[instrument_id]->R;
	float delta_phi =  signal_freq / (float)SAMPLE_FREQ * (float)LUT_SIZE;    // phase increment

// generate buffer of output
	for (int i = 0; i < DMA_SIZE; ++i)
	{
		
		//amplitude passe de 0 à 4 en "attack" intervalle
		if(iterateur_adsr==0){
			amplitude_modulator=0.0f;
			iterateur_adsr++;
		}
			
		//amplitude passe de 0 à 4 en "attack" intervalle
		if (iterateur_adsr>0&&iterateur_adsr<=A)
		{	
			amplitude_modulator+=4.0f/(float)A;
			iterateur_adsr++;
			
		}
		//amplitude passe de 4 à 2 en "decay" intervalles
		else if( iterateur_adsr>=(A+1)&&iterateur_adsr<=(A+D))
		{	
			amplitude_modulator-=2.0f/(float)D;
			iterateur_adsr++;
		}
		//amplitude constante a 2 pendant "sustain" intervalles
	
		else if( iterateur_adsr>=(A+D+1)&&iterateur_adsr<=(A+D+S))
		{	
			amplitude_modulator=2.0f;
			if(setting1.Mode == MODE1){
				iterateur_adsr=(A+D+1);
			}
			iterateur_adsr++;
		}
		//amplitude passe de 2 à 0 en "release" intervalles
		else if( iterateur_adsr>=(A+D+S+1)&&iterateur_adsr<=(A+D+S+R))
		{	
			amplitude_modulator-=2.0f/(float)R;
			iterateur_adsr++;
		}
		
		
		int phase_i = (int)phase;        // get integer part of our phase
	
		//compute output from different LUT
			*(tab+i)= (uint32_t)(amplitude_modulator*(LUT_TABLE[instrument_id][phase_i]));  
			phase += delta_phi;              // increment phase
			if (phase >= (float)LUT_SIZE)    // handle wraparound
					phase -= (float)LUT_SIZE;
	}
	
}


void play_note (float chosen_note, int chosen_instrument)
{
	instrument_id = chosen_instrument;
	instrument* addr_instru = instruments[instrument_id];
	
	//magic formula to compute frequency from input as described -> note : le numéro de la note : 0 = Do3, 49=Do5;
 // it is just a power function, because frequencies are distributed logarithmically
	if(chosen_note >= 0){
		signal_freq=(float)440.0f*powf(2.0f,(float)(chosen_note-9.0f)/12.0f);
		nb_points_adsr=addr_instru->A+addr_instru->D+addr_instru->S+addr_instru->R+1;//nombre d'échantillons total
	}
	else signal_freq=0.0f;

}


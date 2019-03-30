#include "Part_1.h"

/**
*  Displays the current mean and note using the LCD library

*/
void display_distance(void)
{
	LCD_FillRect(0, 0, 240,60, BLACK);
	LCD_SetCursor(0,0);
	
	if(mean[1]>0)
	{
		LCD_Printf("%.2f\n",mean[0]);
	}
	if(note[0] >= 0)
	{
		LCD_Printf("%i\n",note[0]);
	}
}

/**
* @brief 	Computes the mean of the last 5 captures read by the sensor
*					and the according note. The note changes if the last 3 means have been identical.
*					This requirement has been validated in practise. It essential for the stability
*					of the distance we output and consequently the note we are playing

*/

void refresh_distance(void){	
		float nb_good=0.0;
		if(captures[1]>captures[0])
		{	
			distances[it]=captures[1]-captures[0];
		}
		else
		{
			//we should almost never get here
			//The timer3 has a period of 65535 ms
			//if during that time no capture is done, the value in capture is -1
			//This means if we are here, it took more than 65536 to receive the echo
			distances[it] = (65536 - captures[0]) + captures[1];
		}
		//not a magic number. Maths implying speed of sound involve 58.
		//this is all you need to know, programmer.
		distances[it]/=58.0f;
				
		it++;
		if(it>=5)
		{
			mean[1]=0.0;
			for(int i=0;i<it;i++)
			{
				if(distances[i]<400)
				{
					mean[1]+=distances[i];
					nb_good++;
				}
			}
			
			if(mean[1]!=0)
			{	
				mean[1]/=nb_good;//actual current mean of last distances
			}
			
			nb_good=0.0f;
			it=0;
			diffCarre = (mean[0] - mean[1])*(mean[0] - mean[1]);//ecart avec la derniere moyenne pour vérifier la stabilité	
		
      // Pour determiner si le mean change assez pour trigger un changement de valeur a l'ecran
			// 2.5 a été trouvée expérimentalement (it works pretty good)
			if(diffCarre > 2.5f)
			{
				mean[0] = mean[1];//mean[0] is the actual current mean of last distances
				refresh_note(&setting1);
			}
			else 
			{
				note[0] = note[1];
				return;
			}
			
			if(note[0]!=note[1])
			{
					note[1] = note[0];
				//note[0] is the actual current note of last distances
				// the -1 indicates that we are changing notes
					note[0] = -1;
			}
		}
}

void refresh_note(setting *chosen_setting){
	const float increment_chromatic=5.0f;
	const float increment_diatonic = 8.0f;
	const float increment_pentatonic= 8.0f;
	
	if(chosen_setting->Mode == MODE2 || chosen_setting->Mode == MODE0)
	{
		if(chosen_setting->Gamme == CHROMATIC) // Computing des notes dans les modes 0 et 2 chromatique
		{
			if(mean[0]>0&&mean[0]< (increment_chromatic)) note[0]=0;
			else
			{
				for(int i=1;i<25;i++)
				{
					if(mean[0]>=increment_chromatic*(i) && mean[0]< increment_chromatic*(i+1)) 
					{
						note[0]=i;
						break;
					}
					if(i==24)
					{
						note[0]=-1;
						return;	
					}
				}
			}
		}
			
		if(chosen_setting->Gamme == DIATONIC) // Computing des notes dans les modes 0 et 2
		{
			float note_temp = 0;
			for(int i = 0; i < 15 ; i++)
			{
				if(mean[0]>=increment_diatonic*(i) && mean[0]< increment_diatonic*(i+1)) 
				{
						note[0] = note_temp;
						return;
				}
				if(((i%7) < 2) || (((i%7) > 2) && ((i%7) < 6))){
						note_temp+=2;
				}
				else if(((i%7) == 2) || ((i%7) == 6)){
						note_temp+=1;
				}
				if(i==14)
				{
						note[0]=-1;
						return;	
				}
			}
		}
			
		if(chosen_setting->Gamme == PENTATONIC) // Computing des notes dans les modes 0 et 2
		{
			float note_temp = 0;
			for(int i = 0; i < 12 ; i++)
			{
				if(mean[0]>=increment_pentatonic*(i) && mean[0]< increment_pentatonic*(i+1)) 
				{
					note[0] = note_temp;
					return;
				}
				if((i%5 < 2) || (i%5 == 3)){
					note_temp+=2;
				}
				else if((i%5 == 2) || (i%5 == 4)){
					note_temp+=3;
				}
				if(i==11)
				{
					note[0]=-1;
					return;	
				}
			}
		}
	}
	
	else // Computing de la note dans le mode 1
	{
		if(mean[0] <= 3.6f){
			note[0]= 0;
		} 
		else if(mean[0] > 3.6f && mean[0] <= 128.5f)
		{
			note[0] = 24.0f*((mean[0] - 3.5f)/125.0f);
		}
		else note[0]=24;
		note[1] = note[0];
	}
}


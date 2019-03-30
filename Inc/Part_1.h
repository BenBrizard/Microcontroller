#ifndef PART1_H
#define PART1_H
#include "Part_3.h"

extern unsigned int captures[2]; //  Temps écoulé entre 2 echo du capteur ultrasonique. Note:uint32_t == unsigned int
extern float signal_freq;
int it = 0;
float diffCarre = 0;
float distances[20]={0}; // Tableau contenant les distances calculées
float mean[2] = {0}; 	// Tableau contenant les moyennes(utilisé pour stabiliser les différentes
											//	mesures prises): 
											//	[0]->Moyenne précédente
											//	[1]->Moyenne en cours de calcul
float note[2] = {0};

void refresh_distance(void); // sert a compute la distance 
void refresh_note(setting* chosen_setting); // sert a compute la note dependamment du mode/gamme
void display_distance(void); // sert a afficher la distance et la note (le nom de la fonction serait a changer
														 // assez misleading)

#endif

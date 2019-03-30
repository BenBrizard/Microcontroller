#ifndef PART3_H

// La definition des parametres est faite selon la valeure 
// de keypressed. Le premier bouton donne 1, le 2e donne 2, le 3e donne 4
// le 4e donne 8
#include "stm32f4xx_hal.h"
#define PART3_H
#define VIOLON 2
#define DIAPASON 3
#define FLUTE 1
#define PIANO 0

#define MODE0 0
#define MODE1 1
#define MODE2 2
#define MODEZELDA 4
#define MODEMARIO 5
#define MODEJP 6

#define CHROMATIC 0
#define PENTATONIC 1
#define DIATONIC 2

/*typedef struct Picture
{ 
   uint16_t hauteur; 
   uint16_t largeur; 
   uint8_t data[]; 
} picture;*/ 

/* USER STRUCTURES - BEGIN*/
typedef struct Settings {
    int Instrument,Mode,Gamme;
}setting;

typedef struct NotesPrecis {
    char Note[3];

}notePrecis;

typedef struct Positions {
    uint16_t x;
		uint16_t y;
}positions;
/* USER STRUCTURES - END*/

/* USER VARIABLES - BEGIN*/

const char tab_BEN[] = "C4 C4#D4 D4#E4 F4 F4#G4 G4#A4 A4#B4 C5 C5#D5 D5#E5 F5 F5#G5 G5#A5 A5#B5 C6 ";

const struct Positions position1={15,100};
const struct Positions position02={15,120};
setting setting1;
/* USER VARIABLES - END*/

/* USER FUNCTIONS - BEGIN*/
void showModeMenu(void);
void showInstrumentMenu(void);
void showGammeMenu(void);
void showFenetrePrincipal(struct Settings* );
void showNoteMode0(const struct Positions* position);
void showNoteMode1(const struct Positions* position);
void showNoteMode2(const struct Positions* position);
void DerroulementMenu(void);
void LCD_DrawPicture(const uint8_t* picture_chosen, uint16_t largeur, uint16_t hauteur, int16_t l_offset, int16_t h_offset,uint8_t resizing);
/* USER FUNCTIONS - END*/

#endif

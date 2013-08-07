/*
	File for the music definitions to use with the Lemon Player

	created with mod2gbl 0.6

	Whoops, in fact you will need to create this file with the part of the song data
	created with mod2gbl.
	Just follow this exemple

	on 26 sept 99

	lemon@urbanet.ch

*/

UBYTE patern ;
UBYTE nbr_patern;
UBYTE song_nbr ;

UWORD *data_song_ptr;

extern UWORD data_song_fullos0[];
extern UWORD data_song_fullos1[];
extern UWORD data_song_fullos2[];
extern UWORD data_song_fullos3[];
extern UWORD data_song_fullos4[];
extern UWORD data_song_fullos5[];
extern UWORD data_song_fullos6[];
extern UWORD data_song_fullos7[];
extern UWORD data_song_fullos8[];
extern UWORD data_song_fullos9[];
extern UWORD data_song_fullos10[];
extern UWORD data_song_fullos11[];
extern UWORD data_song_fullos12[];
extern UWORD data_song_fullos13[];
extern UWORD data_song_fullos14[];
extern UWORD data_song_fullos15[];
extern UWORD data_song_fullos16[];
extern UWORD data_song_fullos17[];

void patern_definition() NONBANKED
{
         if(patern == 0){ data_song_ptr = data_song_fullos0 ; }
         if(patern == 1){ data_song_ptr = data_song_fullos0 ; }
         if(patern == 2){ data_song_ptr = data_song_fullos1 ; }
         if(patern == 3){ data_song_ptr = data_song_fullos2 ; }
         if(patern == 4){ data_song_ptr = data_song_fullos3 ; }
         if(patern == 5){ data_song_ptr = data_song_fullos4 ; }
         if(patern == 6){ data_song_ptr = data_song_fullos5 ; }
         if(patern == 7){ data_song_ptr = data_song_fullos6 ; }
         if(patern == 8){ data_song_ptr = data_song_fullos7 ; }
         if(patern == 9){ data_song_ptr = data_song_fullos8 ; }
         if(patern == 10){ data_song_ptr = data_song_fullos9 ; }
         if(patern == 11){ data_song_ptr = data_song_fullos10 ; }
         if(patern == 12){ data_song_ptr = data_song_fullos11 ; }
         if(patern == 13){ data_song_ptr = data_song_fullos12 ; }
         if(patern == 14){ data_song_ptr = data_song_fullos13 ; }
         if(patern == 15){ data_song_ptr = data_song_fullos14 ; }
         if(patern == 16){ data_song_ptr = data_song_fullos15 ; }
         if(patern == 17){ data_song_ptr = data_song_fullos16 ; }
         if(patern == 18){ data_song_ptr = data_song_fullos17 ; }
}

/*
	
	GB Lemon tracker	LP1.297a

	it use the mod2gbl music pack
	
	you can change inside the "void sound ()" part the number of music you want to play
	(don't forget to place the same value in the song_def.c and the exemple.bat) 
	
	modified on 26 aug 2000
		found a bug in volume update, that trashed the frequencies in voice 1 and 2 on real GB
	
	
	modified on 30 july 2000
		8 inst by voices
		pan fx on each voices	

	code by Lemon on 26 sept 99 

	if you use this source (or part) in your code give me some credits... ^_^
	lemon@urbanet.ch

*/


#include <gb/gb.h>

#include "../music/songs_def.c"		// here you just include the file who will say to the player what he need to play



unsigned int step;
int timer;

int musicwait;


int voice;
unsigned char freqLOW;
unsigned char freqHI;
unsigned int freqHI_v1;
unsigned int freqHI_v2;
unsigned int gb_freq;
UWORD data_song;

unsigned char fx_value;
unsigned char fx_volume;
unsigned int fx_pan0;
unsigned int fx_pan1;
unsigned int fx_pan2;
unsigned int fx_pan3;

unsigned int snd_swp0 ;
unsigned int snd_lngh0 ;          // to play different instrument on one track !!
unsigned int snd_env0 ;
unsigned int snd_cons0 ;

unsigned int snd_lngh1 ;          // to play different instrument on one track !!
unsigned int snd_env1 ;
unsigned int snd_cons1 ;

unsigned int snd_lngh3 ;          // to play different instrument on one track !!
unsigned int snd_env3 ;
unsigned int snd_poly3 ;
unsigned int snd_cons3 ;
void stopmusic();
void resetmusic();

/************************************************************************/
// here is your instruments wave for channel 3
// 1 full line = 1 instrument
// you can not have more than 8 lines ( = 8 instruments)
// to create 1 new instrument , just use the GameBee Synthetiser, and save it for RGBDS file
// then transform the line to be like those... et voila


extern unsigned int wave[];

/************************************************************************/
extern UWORD frequencies[];
const UBYTE initL[] = {0x4C,0x45,0x4D,0x4F,0x4E};
/************************************************************************/

void stopmusic() NONBANKED
{
  step=0;
  patern=0;
    /* initialise sound output */
  NR52_REG = 0x8FU;
  NR51_REG = 0x00U;
  NR50_REG = 0x77U;
  fx_pan0 = 0x11U ;
  fx_pan1 = 0x22U ;
  fx_pan2 = 0x44U ;
  fx_pan3 = 0x88U ;
}

/************************************************************************/

void resetmusic() NONBANKED
{
    /* initialise sound output */

  NR52_REG = 0x00U;

  step = 0;
  patern = 0;
  fx_pan0 = 0x11U ;
  fx_pan1 = 0x22U ;
  fx_pan2 = 0x44U ;
  fx_pan3 = 0x88U ;

  NR52_REG = 0x8FU;
  NR51_REG = 0x00U;
  NR50_REG = 0x77U;

}

/************************************************************************/

void effect_test() NONBANKED
{
//volume effect
	if ( ((data_song >> 4 ) & 0x0003) == 1){
		if (voice == 0){
        		fx_value = ( (data_song & 0x000F ) << 4 ) ;
        		fx_volume = fx_value;
        		if (gb_freq == 0x48 ){ 
        			NR12_REG = (snd_env0 & 0x000F) | fx_value ;
        			NR14_REG = freqHI_v1 | 0x80U ;
        		}
		}
		if(voice == 1){
			fx_value = ( (data_song & 0x000F ) << 4 ) ;
			fx_volume = fx_value;
			if (gb_freq == 0x48 ){ 
				NR22_REG = (snd_env1 & 0x000F) | fx_value ;
				NR24_REG = freqHI_v2 | 0x80U ;
			}
		}
		if(voice == 2){
			fx_value = (data_song & 0x000F );
			if (fx_value == 0) { fx_value = 0x00U ; }
			if (fx_value >= 1 && fx_value <= 5 ) { fx_value = 0x60U ; }
			if (fx_value >= 6 && fx_value <= 10 ) { fx_value = 0x40U ; }
			if (fx_value >= 11 && fx_value <= 15 ) { fx_value = 0x20U ; }
			fx_volume = fx_value;
			if (gb_freq == 0x48 ){ 
				NR32_REG = fx_volume ;
			}
		}
		if(voice == 3){
			fx_value = ( (data_song & 0x000F ) << 4 ) ;
			fx_volume = fx_value;
			if (gb_freq == 0x48 ){ 
				NR42_REG = (snd_env3 & 0x000F) | fx_value ;
				NR44_REG = 0x80U ;
			}
		}
		

	} 
//pan effect
	if ( ((data_song >> 4 ) & 0x0003) == 2){
		fx_value = (data_song & 0x000F );
		if (voice == 0){
			if (fx_value == 0x0) { fx_pan0 = 0x01U ; }
			if (fx_value == 0x8) { fx_pan0 = 0x11U ; }
			if (fx_value == 0xF) { fx_pan0 = 0x10U ; }
			if (gb_freq == 0x48 ) { NR51_REG = fx_pan0 | fx_pan1 | fx_pan2 | fx_pan3 ; }
			fx_volume = 0xF0U ;
		}
		if (voice == 1){
			if (fx_value == 0x0) { fx_pan1 = 0x02U ; }
			if (fx_value == 0x8) { fx_pan1 = 0x22U ; }
			if (fx_value == 0xF) { fx_pan1 = 0x20U ; }
			if (gb_freq == 0x48 ) { NR51_REG = fx_pan0 | fx_pan1 | fx_pan2 | fx_pan3 ; }
			fx_volume = 0xF0U ;
		}
		if (voice == 2){
			if (fx_value == 0x0) { fx_pan2 = 0x04U ; }
			if (fx_value == 0x8) { fx_pan2 = 0x44U ; }
			if (fx_value == 0xF) { fx_pan2 = 0x40U ; }
			if (gb_freq == 0x48 ) { NR51_REG = fx_pan0 | fx_pan1 | fx_pan2 | fx_pan3 ; }
			fx_volume = 0x20U ;
		}
		if (voice == 3){
			if (fx_value == 0x0) { fx_pan3 = 0x08U ; }
			if (fx_value == 0x8) { fx_pan3 = 0x88U ; }
			if (fx_value == 0xF) { fx_pan3 = 0x80U ; }
			if (gb_freq == 0x48 ) { NR51_REG = fx_pan0 | fx_pan1 | fx_pan2 | fx_pan3 ; }
			fx_volume = 0xF0U ;
		}

	}

	if (((data_song >> 4 ) & 0x0003) == 0){
		if(voice == 0){fx_volume = 0xF0U ;}
		if(voice == 1){fx_volume = 0xF0U ;}
		if(voice == 2){fx_volume = 0x20U ;}
		if(voice == 3){fx_volume = 0xF0U ;}
	}                  
}

/************************************************************************/
void instru_test() NONBANKED
{

	if ( voice == 0 ) {
		if ( ((data_song >> 6 ) & 0x0007) == 0){     // instrument 1 with sustain middle
		
			snd_swp0 = 0x00U;
			snd_lngh0 = 0x07U;
			snd_env0 = fx_volume | 0x02U;  
			snd_cons0 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007) == 1){     // instrument 2 with sustain middle
		
			snd_swp0 = 0x00U;
			snd_lngh0 = 0x47U;
			snd_env0 = fx_volume | 0x02U;
			snd_cons0 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007) == 2){     // instrument 3 with sustain middle
		
			snd_swp0 = 0x00U;
			snd_lngh0 = 0x87U;
			snd_env0 = fx_volume | 0x02U;
			snd_cons0 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007) == 3){     // same as 1 with sustain infinite
		
			snd_swp0 = 0x00U;
			snd_lngh0 = 0x07U;
			snd_env0 = fx_volume | 0x00U;
			snd_cons0 = 0x80U;
		} 
 		if ( ((data_song >> 6 ) & 0x0007) == 4){     // same as 2 with sustain infinite
		
			snd_swp0 = 0x00U;
			snd_lngh0 = 0x47U;
			snd_env0 = fx_volume | 0x00U;  
			snd_cons0 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007) == 5){     // same as 3 with sustain infinite
		
			snd_swp0 = 0x00U;
			snd_lngh0 = 0x87U;
			snd_env0 = fx_volume | 0x00U;
			snd_cons0 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007) == 6){     // create your instrument 7 here
		
			snd_swp0 = 0x00U;
			snd_lngh0 = 0x07U;
			snd_env0 = fx_volume | 0x07U;
			snd_cons0 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007) == 7){     // create your instrument 8 here
		
			snd_swp0 = 0x00U;
			snd_lngh0 = 0x07U;
			snd_env0 = fx_volume | 0x07U;
			snd_cons0 = 0x80U;
		} 
		                          
	}
	if ( voice == 1 ) {
		if ( ((data_song >> 6 ) & 0x0007) == 0){     // instrument 1 with sustain middle
			
			snd_lngh1 = 0x00U;
			snd_env1 = fx_volume | 0x02U;
			snd_cons1 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007) == 1){     // instrument 2 with sustain middle
			
			snd_lngh1 = 0x40U;
			snd_env1 = fx_volume | 0x02U;
			snd_cons1 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007) == 2){     // instrument 3 with sustain middle
			
			snd_lngh1 = 0x80U;
			snd_env1 = fx_volume | 0x02U;
			snd_cons1 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007) == 3){     // same as 1 with sustain infinite
			
			snd_lngh1 = 0x00U;
			snd_env1 = fx_volume | 0x00U;
			snd_cons1 = 0x80U;
		} 
		if ( ((data_song >> 6 ) & 0x0007) == 4){     // same as 2 with sustain infinite
			
			snd_lngh1 = 0x40U;
			snd_env1 = fx_volume | 0x00U;
			snd_cons1 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007) == 5){     // same as 3 with sustain infinite
			
			snd_lngh1 = 0x80U;
			snd_env1 = fx_volume | 0x00U;
			snd_cons1 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007) == 6){     // create your instrument 7 here
			
			snd_lngh1 = 0x80U;
			snd_env1 = fx_volume | 0x07U;
			snd_cons1 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007) == 7){     // create your instrument 8 here
			
			snd_lngh1 = 0x80U;
			snd_env1 = fx_volume | 0x07U;
			snd_cons1 = 0x80U;
		} 

	}
	if ( voice == 2 ) {
		int instru_number = ((data_song >> 6 ) & 0x0007);
		int loop ;
		for (loop = 0 ; loop < 16 ; loop++)
			{
				(*(UBYTE *)(0xFF30+loop)) = wave[loop+(instru_number*16)] ;
			}
	}
	if ( voice == 3 ) {
		if ( ((data_song >> 6 ) & 0x0007)== 0){
			
								// bass drum
			snd_lngh3 = 0x01U;
			snd_env3 = fx_volume | 0x01U;
			snd_poly3 = 0x55U;
			snd_cons3 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007)== 1){
		
								// hi hat
			snd_lngh3 = 0x01U;
			snd_env3 = fx_volume | 0x01U;
			snd_poly3 = 0x01U;
			snd_cons3 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007)== 2){
			
								// snare
			snd_lngh3 = 0x01U;
			snd_env3 = fx_volume | 0x01U;
			snd_poly3 = 0x52U;
			snd_cons3 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007)== 3){
			
								// cymbal
			snd_lngh3 = 0x01U;
			snd_env3 = fx_volume | 0x03U;
			snd_poly3 = 0x02U;
			snd_cons3 = 0x80U;
		} 
		if ( ((data_song >> 6 ) & 0x0007)== 4){
			
								// Laser tom high
			snd_lngh3 = 0x01U;
			snd_env3 = fx_volume | 0x01U;
			snd_poly3 = 0x58U;
			snd_cons3 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007)== 5){
		
								// Laser tom mid
			snd_lngh3 = 0x01U;
			snd_env3 = fx_volume | 0x01U;
			snd_poly3 = 0x59U;
			snd_cons3 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007)== 6){
			
								// Laser tom low
			snd_lngh3 = 0x01U;
			snd_env3 = fx_volume | 0x01U;
			snd_poly3 = 0x5AU;
			snd_cons3 = 0x80U;
		} else
		if ( ((data_song >> 6 ) & 0x0007)== 7){
			
								// strange reflect
			snd_lngh3 = 0x01U;
			snd_env3 = fx_volume | 0x03U;
			snd_poly3 = 0x4FU;
			snd_cons3 = 0x80U;
		} 

	}
}

/************************************************************************/

void voice0() NONBANKED
{

voice = 0;

data_song = data_song_ptr[(step<<2)+voice] ;

gb_freq = data_song >> 8 ;
gb_freq = gb_freq >> 1 ;

effect_test();

if (gb_freq != 0x48 ){

    freqLOW = frequencies[gb_freq] & 0x00FF ;
    freqHI = (frequencies[gb_freq] & 0x0700) >> 8 ;
    
    freqHI_v1 = freqHI ;

    instru_test();
   
    NR51_REG = 0xEEU;
    
    NR10_REG = snd_swp0 ;
    NR11_REG = snd_lngh0 ;
    NR12_REG = snd_env0 ;

    NR13_REG = freqLOW;                       // 8bits of data
    NR14_REG = snd_cons0 | freqHI;            // 3bits of data
       
    NR51_REG = fx_pan0 | fx_pan1 | fx_pan2 | fx_pan3 ;
         
   }
 
}

/************************************************************************/
 void voice1() NONBANKED
{

voice = 1 ;

data_song = data_song_ptr[(step<<2)+voice] ;

gb_freq = data_song >> 8 ;
gb_freq = gb_freq >> 1 ;

effect_test();

if (gb_freq != 0x48 ){

    freqLOW = frequencies[gb_freq] & 0x00FF ;
    freqHI = (frequencies[gb_freq] & 0x0700) >> 8 ;
    
    freqHI_v2 = freqHI ;
	 
    instru_test();              


    NR51_REG = 0xDDU;

    NR21_REG = snd_lngh1 ;      //  0x01
    NR22_REG = snd_env1 ;
    NR23_REG = freqLOW ;
    NR24_REG = snd_cons1 | freqHI ;
            
    NR51_REG = fx_pan0 | fx_pan1 | fx_pan2 | fx_pan3 ;

    }

}

/************************************************************************/
 void voice2() NONBANKED
{

voice = 2 ;

data_song = data_song_ptr[(step<<2)+voice] ;

gb_freq = data_song >> 8 ;
gb_freq = gb_freq >> 1 ;

effect_test();

if (gb_freq != 0x48 ){

    freqLOW = frequencies[gb_freq] & 0x00FF ;
    freqHI = (frequencies[gb_freq] & 0x0700) >> 8 ;

    NR51_REG = 0xBBU;

    NR30_REG = 0x00U;        //    0x80 = 1.0.0.0-0.0.0.0 output ON
    NR34_REG = 0x00U;
    
    instru_test();

    NR31_REG = 0xFFU;
    NR32_REG = fx_volume;
    
    NR30_REG = 0x80U;
    
    NR33_REG = freqLOW ;
    NR34_REG = 0x80U | freqHI;

    NR51_REG = fx_pan0 | fx_pan1 | fx_pan2 | fx_pan3 ;
     
   }
   
}
/************************************************************************/
 void voice3() NONBANKED
{


voice = 3 ;

data_song = data_song_ptr[(step<<2)+voice] ;

gb_freq = data_song >> 8 ;
gb_freq = gb_freq >> 1 ;

effect_test();

if (gb_freq != 0x48 ){      
    
    instru_test();

    NR51_REG = 0x77U;
    
    NR41_REG = snd_lngh3;
    NR42_REG = snd_env3;
    NR43_REG = snd_poly3;
    NR44_REG = snd_cons3;
              
    NR51_REG = fx_pan0 | fx_pan1 | fx_pan2 | fx_pan3 ;

   }

}

/************************************************************************/


void music() NONBANKED
{

if ( song_nbr == 0 ) {
  ENABLE_RAM_MBC1;
  SWITCH_ROM_MBC1 (4);	// you can choose here the bank where you place your song data in the exemple.bat file
  nbr_patern = 18;		// here you need to say the length of your song
}


  voice0();
  voice1();
  voice2();
  voice3();
  
  ++step;
    if (step == 64){
        step = 0;
        ++patern;
        if (patern == nbr_patern ){
            patern = 0;
            }
     }

  patern_definition();
  
  DISABLE_RAM_MBC1;
}

/************************************************************************/


/*
I'm sure there is plenty of better way to do this but don't blame me I'm not god ;o)
*/

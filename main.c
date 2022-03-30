#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL_mixer.h>

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

#include "vecteur.h"
#include "physique.h"
#include "jeu.h"
#include "affichage.h"
#include "musique.h"


Partie partie; // états de la partie, définie dans jeu.h
Corps boules[6]; // propriétés des objets: physiques.h


void BouclePrincipale(void);

int main(int argc, char *argv[])
{
	
	srand(time(NULL));

	InitialisationAffichage();
	
	InitialisationSon();
	
	InitialisationPartie(&partie);
	
	TextureLangages();

//------------Boucle principale adaptée à emcc ou gcc-------------------

	#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(BouclePrincipale, -1, 1);
    #endif
    #ifndef __EMSCRIPTEN__
		while (partie.programme_en_cours)
			BouclePrincipale();
	#endif
	
//------------Sortie de la boucle, on nettoie tout----------------------
	
	DestructionSon();
	
	DestructionAffichage();
	
	#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
	#endif
	
	return EXIT_SUCCESS;
}

//------------Quand le jeu tourne---------------------------------------

void BouclePrincipale(void)
{
//------------Menu de démarrage-----------------------------------------

	if (partie.langage < 0)
	{
		AfficherIllustration();
		AfficherLangage(partie.langage);
		if (ActionsLangage(&partie) == 1)
		{
			ChargerTextes(partie.langage);
			JouerBruitage(7);
		}
	}
//------------Jeu battu, impossible, mais bon...------------------------

	else if (partie.nombre_boules > 6)
	{
		AfficherIllustration();
		AfficherIncroyable(&partie);
		ActionsFin(&partie);
	}
	else
	{
//------------Balles neuves---------------------------------------------

		if (partie.reset == 1)
		{
			PartieReset(&partie);			
			BoulesReset(partie.nombre_boules, boules);
		}
//------------"Moteur Physique" en action-------------------------------			

		if (partie.pause != 1 && partie.resultat != 1 && partie.resultat != 0 && partie.reset != 1)
		{
			partie.temps++;
				
			ForcesEtMouvements(partie.nombre_boules, boules);
			
			ActionsEnJeu(&partie, boules);			
								
			if (partie.temps == TEMPS_NIVEAU)
				partie.resultat = ComparerTiragePositions(partie.tirage, boules, partie.nombre_boules);
		}			
//------------Toujours affiché------------------------------------------

		AfficherVent(boules[0]);				
		AfficherConsignes(&partie);				
		AfficherTemps(partie.temps);
				
		AfficherChallenge(&partie);
				
		AfficherCorps(boules, &partie);
					
		if (partie.resultat == 1)
			AfficherBravo();
		if (partie.resultat == 0)
			AfficherIncorrect();
		if (partie.pause == 1)	
			AfficherPause();
	}
	AfficherLeRendu();

//------------Affichage quand le moteur physique est skippé-------------									
	if (partie.pause == 1 || partie.resultat == 1 || partie.resultat == 0)		
		ActionsALArret(&partie);
}






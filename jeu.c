#include <stdlib.h>
#include <SDL_mixer.h>

#include "vecteur.h"
#include "physique.h"
#include "musique.h"
#include "controle.h"
#include "jeu.h"

void InitialisationPartie(Partie *p)
{
	p->langage = -1;
	p->programme_en_cours = 1;
	p->pause = 0;
	p->reset = 1;
	p->compteur_reset = 0;
	p->resultat = -1;
	p->nombre_boules = 2;
}

int Choixlangage(Direction b, Partie *p)
{
	switch(b)
	{
		case BAS:
			if (p->langage > -4)
				p->langage--;
			return 0;			
			break;
		case HAUT:
			if (p->langage < -1)
				p->langage++;
			return 0;
			break;
		case ENTREE:
		case PAUSE:
			p->langage *= -1;
			return 1;
			break;
		default:
			return 0;
			break;
	}
}

void PartieReset(Partie *p)
{
	p->pause = 0;
	p->reset = 0;
	p->temps = 0;
	p->resultat = -1;
	p->compteur_reset++;
	
	for (int i = 0; i < 6; i++)
			p->tirage[i] = 99;
	int tiragetemp[p->nombre_boules];
	for (int i = 0; i < p->nombre_boules; i++)
		tiragetemp[i] = i;
	for (int i = 0; i < p->nombre_boules; i++)
		p->tirage[i] = tiragetemp[i];
	MelangerTableau(p);
	JouerMusique(p->nombre_boules - 1);
}

void BoulesReset(int nb_boules, Corps *boules)
{
	// Definition des corps et de leurs propriétés constantes		 
	for (int i = 0; i < nb_boules; i++)
		boules[i] = NouveauCorps(LARGEUR_FENETRE/2 - 80 * (nb_boules - 1) / 2 + 80 * i , 50, 10 + i * 10);
		
	// Vitesse de départ aléatoire
	for (int i = 0; i < nb_boules; i++)
	{
		boules[i].vitesse = VecteurRandom();
		boules[i].vitesse.y = 0;
	}
}

void MelangerTableau(Partie *p) // https://benpfaff.org/writings/clc/shuffle.html
{
	int i;
	for (i = 0; i < p->nombre_boules - 1; i++) 
	{
		int j = i + rand() / (RAND_MAX / (p->nombre_boules - i) + 1);
		int t = p->tirage[j];
		p->tirage[j] = p->tirage[i];
		p->tirage[i] = t;
	}
}

void ForcesEtMouvements(int nb_boules, Corps *boules)
{
	for (int cycles = 0; cycles < 5; cycles++)
	{			
		// Collision avec mur
		for (int i = 0; i < nb_boules; i++)
			TestCollisionMur(&boules[i]);
				
		// Collision avec sol
		for (int i = 0; i < nb_boules; i++)
			if (TestCollisionSol(&boules[i]) == 1)
			{
				RebondSol(&boules[i]);
				if (boules[i].contact_sol == 0)
					JouerBruitage(i);
				boules[i].contact_sol++;
			}
			else
			{
				boules[i].rebond = VecteurNul();
				boules[i].contact_sol = 0;
			}
								
		// Collision entre corps	
		for (int i = 0; i < nb_boules; i++)
			boules[i].repulsion = VecteurNul();				
			
		for (int i = 0; i < nb_boules - 1; i++)
			for (int j = i + 1; j < nb_boules; j++)
				if (TestCollisionCorps(&boules[i], &boules[j]) == 1)
				{
					RepulsionCorps(&boules[i], &boules[j]);
					JouerCollision(i);
					JouerCollision(j);
				}
					
		for (int i = 0; i < nb_boules; i++)
			if (boules[i].contact_sol == 0)
				TraineeAir(&boules[i]);
								
		for (int i = 0; i < nb_boules; i++)
			if (boules[i].contact_sol >= 2)
				FrictionSol(&boules[i]);

		// Forces -> acc -> vit -> position
		for (int i = 0; i < nb_boules; i++)
			AppliquerForces(&boules[i]);
	}
}

int ComparerTiragePositions(int *tableau, Corps *alignes, int nb)
{
	JouerMusique(0);
	for (int i = 0; i < nb - 1; i++)
		if (alignes[tableau[i]].position.x > alignes[tableau[i + 1]].position.x)
		{
			JouerBruitage(6);
			return 0;
		}
	JouerBruitage(7);
	return 1;	
}

int ActionsLangage(Partie *partie)
{
	Direction bouton = EntreeJoueur();
	
	if (bouton == FERMERFENETRE)
	{
		partie->programme_en_cours = 0;
		return 0;
	}	
	if (Choixlangage(bouton, partie) == 1)
		return 1;
	return 0;	
}

void ActionsEnJeu(Partie *partie, Corps *boules)
{
	Direction bouton = EntreeJoueur();
	
	switch(bouton)
	{
		case FERMERFENETRE:
			partie->programme_en_cours = 0;
			break;
		case ENTREE:
			partie->resultat = ComparerTiragePositions(partie->tirage, boules, partie->nombre_boules);
			break;
		case PAUSE:
			partie->pause = 1;
			PauseMusique();
			break;
		case SANS:
			for (int i = 0; i < partie->nombre_boules; i++)
				boules[i].vent = VecteurNul();
			break;
		case DROITE:
			for (int i = 0; i < partie->nombre_boules; i++)
				VentSurface(&boules[i], DROITE);
			break;
		case GAUCHE:
			for (int i = 0; i < partie->nombre_boules; i++)
				VentSurface(&boules[i], GAUCHE);
			break;
		default:
			break;
	}
}

void ActionsALArret(Partie *partie)
{
	Direction bouton = EntreeJoueur();
	
	switch(bouton)
	{
		case FERMERFENETRE:
			partie->pause = 0;
			partie->programme_en_cours = 0;
			break;			
		case ENTREE:
			partie->pause = 0;
			partie->reset = 1;
			if (partie->resultat == 1)
				partie->nombre_boules++;
			break;
		case PAUSE:
			if (partie->pause == 1)
				PauseMusique();
			partie->pause = 0;
			partie->resultat = -1;
			if (partie->temps >= TEMPS_NIVEAU)
				partie->reset = 1;
			break;							
		default:
			break;
	}
}

void ActionsFin(Partie *partie)
{
	Direction bouton = EntreeJoueur();
	if (bouton == FERMERFENETRE)
		partie->programme_en_cours = 0;
}

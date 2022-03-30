#ifndef __JEU_H__
#define __JEU_H__

typedef struct Partie
{
	int langage;
	int programme_en_cours;
	int pause;
	int reset;
	int compteur_reset;
	int temps;
	int resultat;
	int nombre_boules;
	int tirage[6];
} Partie;

//------------Initialisation et rechargement----------------------------
void InitialisationPartie(Partie *p);
int Choixlangage(Direction b, Partie *p);
void PartieReset(Partie *p);
void BoulesReset(int nb_boules, Corps *boules);
void MelangerTableau(Partie *p);

//------------Appelle les fonctions mathématiques et physiques----------
void ForcesEtMouvements(int nb_boules, Corps *boules);
int ComparerTiragePositions(int *tableau, Corps *alignes, int nb);

//------------Transmet les actions du joueur----------------------------
int ActionsLangage(Partie *partie);
void ActionsEnJeu(Partie *partie, Corps *boules);
void ActionsALArret(Partie *partie);
void ActionsFin(Partie *partie);

#endif

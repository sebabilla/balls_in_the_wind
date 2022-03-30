#ifndef __PHYSIQUE_H__
#define __PHYSIQUE_H__

#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 600
#define TEMPS_NIVEAU 1500 // nombre de frame par niveau

// GRANDEURS PHYSIQUES
#define GRAVITE 0.01
#define KSOL 0.5
#define KCORPS 0.01
#define SOUFFLE 0.001
#define RAIR 0.001
#define RSOL 0.002
#define LIMVITESSE 4

typedef enum Direction
{
	HAUT, BAS, GAUCHE, DROITE, ENTREE, PAUSE, FERMERFENETRE, SANS, PASDACTION
} Direction;

typedef struct Corps
{
	Vecteur position;
	float masse;
	float rayon;
	Vecteur poids;
	int contact_sol;
	Vecteur friction;
	Vecteur vent;
	Vecteur trainee;
	Vecteur repulsion;
	Vecteur rebond;
	Vecteur resultante;
	Vecteur vitesse;
	Vecteur acceleration;
} Corps;

Corps NouveauCorps(float x, float y, float masse);

//------------Collisions------------------------------------------------
int TestCollisionMur(Corps *corps); // Attention change vecteur vitesse, à séparer en deux
int TestCollisionSol(Corps *corps);
int TestCollisionCorps(Corps *corps1, Corps *corps2);

//------------Forces----------------------------------------------------
void VentSurface(Corps *corps, Direction d);
void RebondSol(Corps *corps);
void RepulsionCorps(Corps *corps1, Corps *corps2);
void TraineeAir(Corps *corps);
void FrictionSol(Corps *corps);

//------------Bilan des forces -> acc -> vit -> position----------------
void AppliquerForces(Corps *corps);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vecteur.h"
#include "physique.h"

Corps NouveauCorps(float x, float y, float masse)
{
	Vecteur gravite = {.x = 0, .y = GRAVITE};
	
	Corps corps = {.position.x = x, .position.y = y, 
			.masse = masse};
	corps.rayon = 5 * sqrt(corps.masse); // ou cbrtf pour avoir r3
	corps.poids = VecteurMultiplierScalaire(gravite, corps.masse);
	// Forces variables
	corps.vent = VecteurNul();	
	corps.trainee = VecteurNul();	
	corps.friction = VecteurNul();
	corps.repulsion = VecteurNul();
	corps.rebond = VecteurNul();
	// compteurs : friction seulement si contact prolongé
	corps.contact_sol = 0;
	// initialisation acceleration, vitesse
	corps.acceleration = VecteurNul();
	corps.vitesse = VecteurNul();
	
	return corps;	
}

int TestCollisionMur(Corps *corps)
{
	if (corps->position.x < corps->rayon)
		{
			corps->position.x = corps->rayon;
			corps->vitesse.x = - corps->vitesse.x;
			return 1;
		}
	if (corps->position.x > LARGEUR_FENETRE - corps->rayon)
		{
			corps->position.x = LARGEUR_FENETRE - corps->rayon;
			corps->vitesse.x = - corps->vitesse.x;
			return 1;		
		}
	return 0;
}

int TestCollisionSol(Corps *corps)
{
	if (corps->position.y > HAUTEUR_FENETRE - corps->rayon)
		return 1;
	return 0;
}

int TestCollisionCorps(Corps *corps1, Corps *corps2)
{
	float distance = fabsf(VecteurNorme(VecteursSoustraire(corps1->position, corps2->position)));
	if (distance <= corps1->rayon + corps2->rayon)
		return 1;
	return 0;
}

void RebondSol(Corps *corps)
{
	Vecteur v_sol = {.x = corps->position.x + (corps->vitesse.x) / 2, .y = HAUTEUR_FENETRE + corps->rayon};
	Vecteur difference = VecteursSoustraire(corps->position, v_sol);
	float distance = VecteurNorme(difference);
	float penetration = 2*corps->rayon - distance;
	float ressort = KSOL * (penetration);
	difference = VecteurNormaliser(difference);
		
	if (penetration >= 0)
	{
		corps->rebond = VecteurMultiplierScalaire(difference, ressort);
		corps->rebond = VecteursSoustraire(corps->rebond, VecteurMultiplierScalaire(corps->vitesse, 0.01 * corps->masse));
	}
	else
	{
		corps->rebond = VecteurMultiplierScalaire(difference, -1*ressort);
		corps->rebond = VecteursSoustraire(corps->rebond, VecteurMultiplierScalaire(corps->vitesse, 0.01 * corps->masse));
	}
}

void RepulsionCorps(Corps *corps1, Corps *corps2)
{
	Vecteur difference = VecteursSoustraire(corps1->position, corps2->position);
	float distance = VecteurNorme(difference);
	float penetration = corps1->rayon + corps2->rayon - distance;
	float ressort = KCORPS * (penetration);
	difference = VecteurNormaliser(difference);
	
	if (penetration >= 0)
	{
	corps1->repulsion = VecteursAjouter(corps1->repulsion, VecteurMultiplierScalaire(difference, ressort));
	corps2->repulsion = VecteursAjouter(corps2->repulsion, VecteurMultiplierScalaire(difference, -1*ressort));
	}
	else
	{
	corps1->repulsion = VecteursAjouter(corps1->repulsion, VecteurMultiplierScalaire(difference, -1*ressort));
	corps2->repulsion = VecteursAjouter(corps2->repulsion, VecteurMultiplierScalaire(difference, ressort));
	}
}

void VentSurface(Corps *corps, Direction d)
{
	int dir;
	if (d == GAUCHE)
		dir = -1;
	else
		dir = 1;
	
	corps->vent.x = dir * SOUFFLE * corps->rayon;
}

void TraineeAir(Corps *corps)
{
	corps->trainee = VecteurNormaliser(corps->vitesse);
	corps->trainee = VecteurMultiplierScalaire(corps->trainee, -1 * RAIR); 
	float norme_vitesse = VecteurNorme(corps->vitesse);
	corps->trainee = VecteurMultiplierScalaire(corps->trainee, norme_vitesse * norme_vitesse);
	corps->friction = VecteurNul();
}

void FrictionSol(Corps *corps)
{
	corps->friction = VecteurNormaliser(corps->vitesse);
	corps->friction = VecteurMultiplierScalaire(corps->friction, -1 * RSOL); 
	corps->trainee = VecteurNul();
}

void AppliquerForces(Corps *corps)
{
	// Resultante des forces
	corps->resultante = VecteursAjouter(corps->poids, corps->vent);
	corps->resultante = VecteursAjouter(corps->resultante, corps->trainee);
	corps->resultante = VecteursAjouter(corps->resultante, corps->friction);
	corps->resultante = VecteursAjouter(corps->resultante, corps->repulsion);
	corps->resultante = VecteursAjouter(corps->resultante, corps->rebond);
			
	// Acceleration et vitesse
	corps->acceleration = VecteurMultiplierScalaire(corps->resultante, 1/corps->masse);
	corps->vitesse = VecteursAjouter(corps->vitesse, corps->acceleration);
	// Eviter qu'un corps allant trop vite en traverse un autre
	// Solution bidon masi bon...,
	if (VecteurNorme(corps->vitesse) > LIMVITESSE)
		corps->vitesse = VecteurAttribuerNorme(corps->vitesse, LIMVITESSE);
	corps->position = VecteursAjouter(corps->position, corps->vitesse);				
}

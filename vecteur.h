#ifndef __VECTEUR_H__
#define __VECTEUR_H__

typedef struct Vecteur
{
	double x;
	double y;
} Vecteur;

//------------Opérations mathématiques de base--------------------------
Vecteur VecteurNul(void);
Vecteur VecteursAjouter(Vecteur v1, Vecteur v2);
Vecteur VecteursSoustraire(Vecteur v1, Vecteur v2);
Vecteur VecteurMultiplierScalaire(Vecteur v, float s);
float VecteurNorme(Vecteur v);
Vecteur VecteurNormaliser(Vecteur v);
Vecteur VecteurAttribuerNorme(Vecteur v, float n);
Vecteur VecteurRandom(void);
Vecteur VecteurRandomScalaire(float s);

#endif


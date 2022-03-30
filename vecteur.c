#include <stdio.h>
#include <stdlib.h>
#include <math.h> // ne pas oublier -lm à la compilation
#include "vecteur.h"

Vecteur VecteurNul(void)
{
	Vecteur v = {.x = 0, .y = 0};
	return v;
}

Vecteur VecteursAjouter(Vecteur v1, Vecteur v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	
	return v1;
}

Vecteur VecteursSoustraire(Vecteur v1, Vecteur v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	
	return v1;
}

Vecteur VecteurMultiplierScalaire(Vecteur v, float s)
{
	v.x *= s;
	v.y *= s;
	
	return v;
}

float VecteurNorme(Vecteur v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

Vecteur VecteurNormaliser(Vecteur v)
{
	float inv_k = 1.0 / VecteurNorme(v);
	if (inv_k == 0)
		inv_k = 0.000001;
	
	return VecteurMultiplierScalaire(v, inv_k);
}

Vecteur VecteurAttribuerNorme(Vecteur v, float n)
{
	v = VecteurNormaliser(v);
	
	return VecteurMultiplierScalaire(v, n);
}

Vecteur VecteurRandom(void)
{
	Vecteur v;
	float angle = rand() / (RAND_MAX + 1.) * 2 * M_PI;
	v.x = cos(angle);
	v.y = sin(angle);
	
	return v;
}

Vecteur VecteurRandomScalaire(float s)
{
	Vecteur v = VecteurRandom();
	
	return VecteurMultiplierScalaire(v, s);	
}

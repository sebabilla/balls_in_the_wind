#ifndef __AFFICHAGE_H__
#define __AFFICHAGE_H__

#define TITRE "Balls in the wind"
#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 600
#define FPS 16
#define LIGNES_TEXTE 14
#define NOMBRE_LANGAGES 4

#define ROUGE 0xFF0000FF
#define VERT 0xFF00FF00
#define BLEU 0xFFFF901E
#define JAUNE 0xFF00FFFF
#define ROSE 0xFFC693FE
#define MARRON 0xFF024EAE
#define TURQUOISE 0xFFC3E617
#define BLANC 0xFFEDEDED

typedef struct Textes
{
		char bravo[10];
		char incorrect[15];
		char pause[10];
		char fleches[60];
		char entree[60];
		char espace[60];
		char but[10];
		char credits[10];
		char auteur[100];
		char sources[100];
		char musique[100];
		char bruitages[100];
} Textes;
	
//------------Gestion de la SDL-----------------------------------------
void SDL_ExitWithError(const char *message);
void limit_fps(void);
void InitialisationAffichage(void);
void ChargerImage(char *l, int i);
void AfficherLeRendu(void);
void DestructionAffichage(void);

//------------Gestion des objets en mouvement---------------------------
void InitialiserParticules(void);
void AfficherVent(Corps c);
void AfficherChallenge(Partie *partie);
void AfficherCorps(Corps *boules, Partie *partie);

//------------Gestion du texte------------------------------------------
void ChargerTextes(int l);
int VerificationLangage(const char *s);
SDL_Texture *TextureTexte(char *texte, int i);
void TextesTraduits(char *l);
void TextesNiveaux(void);
void EcrireTexteProvisoire(char *texte, int X, int Y, int W, int H);
void AfficherConsignes(Partie *partie);
void AfficherTemps(int im);
void AfficherBravo(void);
void AfficherIncorrect(void);
void AfficherPause(void);
void TextureLangages(void);
void AfficherLangage(int l);
void AfficherIllustration (void);
void AfficherIncroyable(Partie *partie);
 

#endif

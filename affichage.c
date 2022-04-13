#include <SDL.h>
#include <stdio.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

#include "vecteur.h"
#include "physique.h"
#include "jeu.h"
#include "affichage.h"

SDL_Window *window;
SDL_Renderer *renderer;
int frame_limit;
TTF_Font *font;
SDL_Texture *texture_illustration[9];
SDL_Texture *textures_niveaux[5];
SDL_Texture *textures_langages[NOMBRE_LANGAGES * 2];
int longueur_langage[NOMBRE_LANGAGES * 2];
SDL_Texture *texture_textes[LIGNES_TEXTE];
int longueur_texte[LIGNES_TEXTE];
Uint32 couleurs[6];

Vecteur particules[LARGEUR_FENETRE / 20][HAUTEUR_FENETRE / 20];
float sinusvent;

void SDL_ExitWithError(const char *message)
{
	SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
	SDL_Quit();
	exit(EXIT_FAILURE);
}

void limit_fps(void)
{
	unsigned t = SDL_GetTicks();
	
	if (frame_limit < t)
	{
		frame_limit = SDL_GetTicks() + FPS;
		return;
	}
	else if (frame_limit > t + FPS)
		SDL_Delay(FPS);
	else
		SDL_Delay(frame_limit - t);
	frame_limit = SDL_GetTicks() + FPS;
}

void InitialisationAffichage(void)
{
	window = NULL;
	renderer = NULL;

	frame_limit = 0;
	InitialiserParticules();
		
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
		SDL_ExitWithError("Initiation SDL");
		
	if (SDL_CreateWindowAndRenderer(LARGEUR_FENETRE, HAUTEUR_FENETRE, 0, &window, &renderer))
		SDL_ExitWithError("Impossible de creer la fenetre et le rendu");
	
	SDL_SetWindowTitle(window, TITRE);

	if (TTF_Init() < 0)
		SDL_ExitWithError("Initiation TTF");
		
	font = TTF_OpenFont("res/OtomanopeeOne-Regular.ttf", 100);
	if (!font)
		SDL_ExitWithError("Chargement de la police");
		
	ChargerImage("res/rouge.bmp", 0);
	ChargerImage("res/vert.bmp", 1);
	ChargerImage("res/bleu.bmp", 2);
	ChargerImage("res/jaune.bmp", 3);
	ChargerImage("res/rose.bmp", 4);
	ChargerImage("res/marron.bmp", 5);
	ChargerImage("res/turquoise.bmp", 6);
	ChargerImage("res/blanc.bmp", 7);
	ChargerImage("res/ballons_vent.bmp", 8);
		
	couleurs[0] = ROUGE;
	couleurs[1] = VERT;
	couleurs[2]	= BLEU;
	couleurs[3]	= JAUNE;
	couleurs[4]	= ROSE;
	couleurs[5]	= MARRON;		
}

void ChargerImage(char *l, int i)
{
	SDL_Surface *image = NULL;
	texture_illustration[i] = NULL;	
	if ((image = SDL_LoadBMP(l)) == NULL)
		SDL_ExitWithError("Impossible de charger l'image");
	if ((texture_illustration[i] = SDL_CreateTextureFromSurface(renderer, image)) == NULL)
		SDL_ExitWithError("Impossible de charger l'image");
	SDL_FreeSurface(image);
}

void AfficherLeRendu(void)
{
	SDL_RenderPresent(renderer);
			
	#ifndef __EMSCRIPTEN__
		limit_fps();
	#endif
	
	SDL_Rect fond_fenetre = {.x = 0, .y = 0, .w = LARGEUR_FENETRE, .h = HAUTEUR_FENETRE};
	SDL_RenderClear(renderer);					
	
	if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0)
		SDL_ExitWithError("Impossible de changer la couleur du rendu");
		
	if (SDL_RenderFillRect(renderer, &fond_fenetre) != 0)
		SDL_ExitWithError("Impossible de dessiner un rectangle");
}

void DestructionAffichage(void)
{
	for (int i = 0; i < 9; i++)
		SDL_DestroyTexture(texture_illustration[i]);
	for (int i = 0; i < LIGNES_TEXTE; i++)
		SDL_DestroyTexture(texture_textes[i]);
	for (int i = 0; i < 5; i++)
		SDL_DestroyTexture(textures_niveaux[i]);
	for (int i = 0; i < NOMBRE_LANGAGES * 2; i++)
		SDL_DestroyTexture(textures_langages[i]);
		
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void InitialiserParticules(void)
{
	for (int i = 0; i < LARGEUR_FENETRE / 40; i++)
		for (int j = 0; j < HAUTEUR_FENETRE / 40; j++)
		{
			particules[i][j].x = i * 40 + rand() % 35;
			particules[i][j].y = 10 + j * 40 + rand() % 35;
		}
	sinusvent = 0;
}

void AfficherVent(Corps c)
{
	for (int i = 0; i < LARGEUR_FENETRE / 40; i++)
		for (int j = 0; j < HAUTEUR_FENETRE / 40; j++)
		{
			particules[i][j].x += c.vent.x * c.rayon * 10; 
			if (particules[i][j].x < 0)
				particules[i][j].x += LARGEUR_FENETRE;
			if (particules[i][j].x > LARGEUR_FENETRE)
				particules[i][j].x -= LARGEUR_FENETRE;
		}
	
	int houle = sin(sinusvent) * 10;
	if (c.vent.x != 0)
		sinusvent += 0.05 ;
		
	SDL_Rect rect_illu = {.w = 2, .h = 2};	
	for (int i = 0; i < LARGEUR_FENETRE / 40; i++)
		for (int j = 0; j < HAUTEUR_FENETRE / 40; j++)
		{				
			rect_illu.x = particules[i][j].x; 
			rect_illu.y = particules[i][j].y + houle;
			if (SDL_RenderCopy(renderer, texture_illustration[6], NULL, &rect_illu) != 0)
				SDL_ExitWithError("Impossible d'afficher la texture");
		}	
}

void AfficherChallenge(Partie *partie)
{
	SDL_Rect rect_illu = {.w = 40, .h = 40};	
						
	for (int i = 0; i < partie->nombre_boules; i++)
	{
		rect_illu.x = 60 + 40 * i; 
		rect_illu.y = 15;
		if (SDL_RenderCopy(renderer, texture_illustration[partie->tirage[i]], NULL, &rect_illu) != 0)
			SDL_ExitWithError("Impossible d'afficher la texture");
	}
}

void AfficherCorps(Corps *boules, Partie *partie)
{
	SDL_Rect rect_illu;
	
	for (int i = 0; i < partie->nombre_boules; i++)
	{		
		rect_illu.x = boules[i].position.x - boules[i].rayon; 
		rect_illu.y = boules[i].position.y - boules[i].rayon;
		rect_illu.w = boules[i].rayon * 2;
		rect_illu.h = boules[i].rayon * 2;
		if (SDL_RenderCopy(renderer, texture_illustration[i], NULL, &rect_illu) != 0)
			SDL_ExitWithError("Impossible d'afficher la texture");
	}
}

void ChargerTextes(int l)
{
	char *lien;
	if (l == 1)
		lien = "res/EN.txt";
	if (l == 2)
		lien = "res/ES.txt";
	if (l == 3)
		lien = "res/FR.txt";
	if (l == 4)
		lien = "res/JA.txt";
	
	TextesTraduits(lien);
	
	TextesNiveaux();
	
}

int VerificationLangage(const char *s)
{
	FILE *fichier;
	if ((fichier = fopen(s, "r")))
	{
		fclose(fichier);
		return 1;
	}
	else
		return 0;
}

SDL_Texture *TextureTexte(char *texte, int i)
{
	SDL_Color couleur;
	if (i == 0) // blanc
	{
		couleur.r = 250; couleur.g = 250; couleur.b = 250; couleur.a = 0;
	}
	if (i == 1) // jaune
	{
		couleur.r = 250; couleur.g = 250; couleur.b = 0; couleur.a = 0;
	}
										
	SDL_Surface *surface_message = TTF_RenderUTF8_Solid(font, texte, couleur);
	SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surface_message);
	SDL_FreeSurface(surface_message);
	
	return message;
}

void TextesTraduits(char *l)
{
	if (VerificationLangage(l) == 1)
	{		
		FILE *lecture = fopen(l, "r");
		char tampon[150];
		
		for (int i = 0; i < LIGNES_TEXTE; i++)
		{
			fgets(tampon, sizeof(tampon), lecture);
			tampon[strcspn(tampon, "\n")] = 0;
			longueur_texte[i] = strlen(tampon);
			texture_textes[i] = TextureTexte(tampon, 0);
		}		
		fclose(lecture);
	}
	else
	{
		printf("Fichier linguistique non trouvé");
		exit(EXIT_FAILURE);
	}
}

void TextesNiveaux(void)
{
	char niveau[4];
	for (int i = 0; i < 5; i++)
	{
		sprintf(niveau, "%d/6", i + 2);
		textures_niveaux[i] = TextureTexte(niveau, 0);
	}
}

void EcrireTexteProvisoire(char *texte, int X, int Y, int W, int H)
{	
	SDL_Texture *message = TextureTexte(texte, 0);	
	SDL_Rect MessageRect = {.x = X, .y = Y, .w = W, .h = H};
	SDL_RenderCopy(renderer, message, NULL, &MessageRect);
	SDL_DestroyTexture(message);
}

void AfficherConsignes(Partie *partie)
{
	if (roundedBoxRGBA(renderer, 5, 5, LARGEUR_FENETRE - 5, 65, 10, 50, 50, 50, 255) != 0) 
		SDL_ExitWithError("Impossible de dessiner la légende");
	
	SDL_Rect niveau_rect = {.x = 15, .y = 35, .w = 3 * 8, .h = 20};
	SDL_RenderCopy(renderer, textures_niveaux[partie->nombre_boules - 2], NULL, &niveau_rect);	
	
	
	if (partie->langage != 4)
	{
		for (int i = 0; i < 3; i++)
		{
			SDL_Rect message_rect = {.x = 400, .y = 5 + i * 20, .w = longueur_texte[3 + i] * 8, .h = 20};
			SDL_RenderCopy(renderer, texture_textes[3 + i], NULL, &message_rect);
		}
		SDL_Rect message_rect = {.x = 10, .y = 15, .w = longueur_texte[6] * 8, .h = 20};
		SDL_RenderCopy(renderer, texture_textes[6], NULL, &message_rect);
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			SDL_Rect message_rect = {.x = 450, .y = 5 + i * 20, .w = longueur_texte[3 + i] * 6 - 15 * i, .h = 20};
			SDL_RenderCopy(renderer, texture_textes[3 + i], NULL, &message_rect);
		}
		SDL_Rect message_rect = {.x = 10, .y = 15, .w = longueur_texte[6] * 6, .h = 20};
		SDL_RenderCopy(renderer, texture_textes[6], NULL, &message_rect);
	}
}

void AfficherTemps(int im)
{
	char temps_restant[4];
	sprintf(temps_restant, "%02d", (TEMPS_NIVEAU - im) / 60);
	EcrireTexteProvisoire(temps_restant, LARGEUR_FENETRE - 50, 10, 40, 50);
}

void AfficherBravo(void)
{
	SDL_Rect message_rect = {.x = LARGEUR_FENETRE / 2 - 75, .y = HAUTEUR_FENETRE / 2 - 75, .w = 150, .h = 100};
	SDL_RenderCopy(renderer, texture_textes[0], NULL, &message_rect);
}

void AfficherIncorrect(void)
{
	SDL_Rect message_rect = {.x = LARGEUR_FENETRE / 2 - 75, .y = HAUTEUR_FENETRE / 2 - 75, .w = 150, .h = 100};
	SDL_RenderCopy(renderer, texture_textes[1], NULL, &message_rect);
}

void AfficherPause(void)
{
	SDL_Rect message_rect = {.x = LARGEUR_FENETRE / 2 - 50, .y = HAUTEUR_FENETRE / 2 - 50, .w = 150, .h = 100};
	SDL_RenderCopy(renderer, texture_textes[2], NULL, &message_rect);
	
	for (int i = 0; i < 5; i++)
	{
		SDL_Rect message_rect = {.x = 5, .y = HAUTEUR_FENETRE - 110 + i * 20, .w = longueur_texte[7 + i] * 8, .h = 20};
		SDL_RenderCopy(renderer, texture_textes[7 + i], NULL, &message_rect);
	}
}

void TextureLangages(void)
{
	char *lien = "res/LANG.txt";

	if (VerificationLangage(lien) == 1)
	{		
		FILE *lecture = fopen(lien, "r");
		char tampon[15];
		
		for (int i = 0; i < NOMBRE_LANGAGES; i ++)
		{
			fgets(tampon, sizeof(tampon), lecture);
			tampon[strcspn(tampon, "\n")] = 0;
			longueur_langage[i * 2] = strlen(tampon);
			longueur_langage[i * 2 + 1] = longueur_texte[i * 2];
			textures_langages[i * 2] = TextureTexte(tampon, 0);
			textures_langages[i * 2 + 1] = TextureTexte(tampon, 1);
		}		
		fclose(lecture);
	}
	else
	{
		printf("Fichier linguistique non trouvé");
		exit(EXIT_FAILURE);
	}
}


void AfficherLangage(int l)
{
	for (int i = 0; i < NOMBRE_LANGAGES; i ++)
	{
		SDL_Rect message_rect = {.x = LARGEUR_FENETRE / 2 - longueur_langage[i * 2] * 8, .y = HAUTEUR_FENETRE / 2 + 50 + i * 30, .w = longueur_langage[i * 2] * 16, .h = 40};
		if (-l - 1 != i)
			SDL_RenderCopy(renderer, textures_langages[i * 2], NULL, &message_rect);
		else
			SDL_RenderCopy(renderer, textures_langages[i * 2 + 1], NULL, &message_rect);
	}
}

void AfficherIllustration (void)
{
	SDL_Rect rect_illu = {.w = 200, .h = 280};
	rect_illu.x = LARGEUR_FENETRE / 2 - 100; 
	rect_illu.y = HAUTEUR_FENETRE / 2 - 300;
	if (SDL_RenderCopy(renderer, texture_illustration[8], NULL, &rect_illu) != 0)
			SDL_ExitWithError("Impossible d'afficher la texture");
			
	hlineColor(renderer, LARGEUR_FENETRE / 2 - 200, LARGEUR_FENETRE / 2 + 200, HAUTEUR_FENETRE / 2 + 20, BLANC);
}

void AfficherIncroyable(Partie *partie)
{
	SDL_Rect message_rect1 = {.x = LARGEUR_FENETRE / 2 - longueur_texte[12] * 8, .y = HAUTEUR_FENETRE / 2 + 25, .w = longueur_texte[12] * 16, .h = 40};
	SDL_Rect message_rect2 = {.x = LARGEUR_FENETRE / 2 - longueur_texte[13] * 8, .y = HAUTEUR_FENETRE / 2 + 75, .w = longueur_texte[13] * 16, .h = 40};
	SDL_RenderCopy(renderer, texture_textes[12], NULL, &message_rect1);
	SDL_RenderCopy(renderer, texture_textes[13], NULL, &message_rect2);
	
	char essais[5];
	sprintf(essais, "%04d", partie->compteur_reset);
	EcrireTexteProvisoire(essais, LARGEUR_FENETRE / 2 - 25, HAUTEUR_FENETRE / 2 + 125, 50, 40);
}





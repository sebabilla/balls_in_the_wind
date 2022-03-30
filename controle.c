#include <SDL.h>
#include <stdio.h>

#include "vecteur.h"
#include "physique.h"
#include "controle.h"

Direction EntreeJoueur(void)
{
	SDL_Event event;		
		if (SDL_PollEvent(&event)) 
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_DOWN:
							return BAS;
							break;
						case SDLK_UP:
							return HAUT;
							break;
						case SDLK_LEFT:
							return GAUCHE;
							break;
						case SDLK_RIGHT:
							return DROITE;
							break;
						case SDLK_RETURN:
							return ENTREE;
							break;
						case SDLK_SPACE:
							return PAUSE;
							break;
						case SDLK_ESCAPE:
							return FERMERFENETRE;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym)
					{
						case SDLK_DOWN:
							return SANS;
							break;
						case SDLK_UP:
							return SANS;
							break;
						case SDLK_LEFT:
							return SANS;
							break;
						case SDLK_RIGHT:
							return SANS;
							break;						
						default:
							break;
					}
					break;
				case SDL_QUIT: 
					return FERMERFENETRE;
					break;			
				default:
					break;
			}
		}
		return PASDACTION;
}

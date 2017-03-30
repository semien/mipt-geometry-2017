#include <SDL.h>
#include <cstdio>
#include<vector>
#include"attributes.h"
#include"primitives.h"
#include"detail.h"

const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 300;

void draw(std::vector<Colour>& colors) {
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL не смог запуститься! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		window = SDL_CreateWindow("RayTracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Окно не может быть создано! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			while (true)
			{
				SDL_Event e;
				if (SDL_PollEvent(&e))
					if (e.type == SDL_QUIT)
						break;
				SDL_RenderClear(renderer);
				for (int i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; i++) {
					SDL_SetRenderDrawColor(renderer, colors[i].red, colors[i].green, colors[i].blue, 0);
					SDL_RenderDrawPoint(renderer, i/SCREEN_WIDTH, i%SCREEN_WIDTH);
				}
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
				SDL_RenderPresent(renderer);
			}

			SDL_DestroyRenderer(renderer);
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	std::vector<Object*> objects;
	Triangle* tr = new Triangle(Point(3, 0, 0), Point(3, 3, 0), Point(3, 0, 4), Point(-1, 0, 0),
		Attributes(Colour{ 255,0,0 }, Colour{ 0,255,0 }));
	Sphere* sph = new Sphere(Point(2, 0, 0), 1, Attributes(Colour{ 0,0,255 }, Colour{ 0,0,0 }));
	Quadrangle* qu = new Quadrangle(Point(5, -5, -5), Point(5, -5, 5), Point(5, 2, 2), Point(5, 5, -2), Point(-1, 0, 0),
		Attributes(Colour{ 0,255,0 }, Colour{ 255,0,0 }));
	objects.push_back(sph);
	objects.push_back(tr);
	objects.push_back(qu);
	

	Point spec(0, 0, 0);
	Screen scr(Point(1,-2,-2),Point(1,-2,2),Point(1,2,2),Point(1,2,-2), SCREEN_WIDTH, SCREEN_HEIGHT);
	Scene sc(objects, spec);
	std::vector<Colour> pixels;
	sc.setPixels(scr, pixels);
	draw(pixels);
	return 0;
}

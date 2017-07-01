#include<SDL.h>
#include<cstdio>
#include<vector>
#include<iostream>
#include<time.h>
#include<string>
#include<fstream>
#include<map>
#include"attributes.h"
#include"primitives.h"
#include"detail.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
Point A; 
Point B;
Point C;
Point D;
Point spec;

Point up = Point(0.5, 0, 0);
Point right = Point(0, 0.5, 0);
Point w = Point(0, 0, 0.5);

std::vector<Colour> pixels;
std::vector<Object*> objects;
std::vector<Light*> lighters;
std::map<std::string, Attributes> materials;
Scene sc;

void RTformat(std::string filename) {
	std::ifstream fin(filename);
	std::string current;
	std::string name;
	double a, b, c;
	double refl;
	double radius;
	Point p1, p2, p3, p4;
	while (!fin.eof()) {
		fin >> current;
		if (current == "#") {
			std::getline(fin, current);
		}
		if (current == "viewport") {
			while (current != "endviewport") {
				fin >> current;
				if (current == "origin") {
					fin >> a >> b >> c;
					spec = Point(a, b, c);
				}
				if (current == "topleft") {
					fin >> a >> b >> c;
					B = Point(a, b, c);
				}
				if (current == "bottomleft") {
					fin >> a >> b >> c;
					A = Point(a, b, c);
				}
				if (current == "topright") {
					fin >> a >> b >> c;
					C = Point(a, b, c);
				}
				if (current == "#") {
					std::getline(fin, current);
				}
			}
			D = C + B - A;
		}
		if (current == "materials") {
			while (current != "endmaterials")
			{
				fin >> current;
				if (current == "entry") {
					while (current != "endentry") {
						fin >> current;
						if (current == "#") {
							std::getline(fin, current);
						}
						if (current == "name") {
							fin >> name;
						}
						if (current == "color") {
							fin >> a >> b >> c;
						}
						if (current == "alpha") {
							fin >> current; //pfffffff
						}
						if (current == "reflect") {
							fin >> refl;
						}
						if (current == "refract") {
							fin >> current; //pfffffff
						}
					}
					materials.insert(std::pair<std::string,Attributes>(name, Attributes(Colour{ a,b,c }, Colour{ 0,0,0 },refl)));
				}
			}
		}
		if (current == "lights") {
			while (current != "endlights") {
				fin >> current;
				if (current == "#") {
					std::getline(fin, current);
				}
				if (current == "reference") {
					while (current != "endreference") {
						fin >> current;
					}
				}
				if (current == "point") {
					while (current != "endpoint") {
						fin >> current;
						if (current == "#") {
							std::getline(fin, current);
						}
						if (current == "coords") {
							fin >> a >> b >> c;
							p1 = Point{ a,b,c };
						}
						if (current == "power") {
							fin >> a;
						}
					}
					Light* ligh = new Light{ p1,a };
					lighters.push_back(ligh);
				}

			}
		}
		if (current == "geometry") {
			while (current != "endgeometry") {
				fin >> current;
				if (current == "#") {
					std::getline(fin, current);
				}
				if (current == "sphere") {
					while (current != "endsphere")
					{
						fin >> current;
						if (current == "#") {
							std::getline(fin, current);
						}
						if (current == "coords") {
							fin >> a >> b >> c;
							p1 = Point{ a,b,c };
						}
						if (current == "radius") {
							fin >> radius;
						}
						if (current == "material") {
							fin >> name;
						}
					}
					Sphere* sph = new Sphere(p1, radius, materials[name]);
					objects.push_back(sph);
				}
				if (current == "triangle") {
					int y = 0;
					while (current != "endtriangle")
					{
						fin >> current;
						if (current == "#") {
							std::getline(fin, current);
						}
						if (current == "vertex") {
							++y;
							fin >> a >> b >> c;
							if (y == 1) {
								p1 = Point{ a,b,c };
							}
							if (y == 2) {
								p2 = Point{ a,b,c };
							}
							if (y == 3) {
								p3 = Point{ a,b,c };
							}
						}
						if (current == "material") {
							fin >> name;
						}
					}
					Triangle* tr = new Triangle(p1,p2,p3, (p2-p1)^(p3-p2),materials[name]);
					objects.push_back(tr);
				}
				if (current == "quadrangle") {
					int y = 0;
					while (current != "endquadrangle")
					{
						fin >> current;
						if (current == "#") {
							std::getline(fin, current);
						}
						if (current == "vertex") {
							++y;
							fin >> a >> b >> c;
							if (y == 1) {
								p1 = Point{ a,b,c };
							}
							if (y == 2) {
								p2 = Point{ a,b,c };
							}
							if (y == 3) {
								p3 = Point{ a,b,c };
							}
							if (y == 4) {
								p4 = Point{ a,b,c };
							}
						}
						if (current == "material") {
							fin >> name;
						}
					}
					Quadrangle* qu = new Quadrangle(p1, p2, p3, p4, (p2-p1) ^ (p3 - p2), materials[name]);
					objects.push_back(qu);
				}
			}
		}
	}
}

void setPartOfPixels(int i) {
	for (int z = i; z < SCREEN_HEIGHT; z+=4) {
		for (int y = 0; y < SCREEN_WIDTH; ++y) {
			sc.setPixel(z, y, pixels);
		}
	}
}

void setPixels() {
	double tim = clock();
	std::thread th1(setPartOfPixels, 0);
	std::thread th2(setPartOfPixels, 1);
	std::thread th3(setPartOfPixels, 2);
	std::thread th4(setPartOfPixels, 3);
	th1.join();
	th2.join();
	th3.join();
	th4.join();
	std::cout << clock() - tim << std::endl;
}

void move_UP(std::vector<Colour>& colors, Scene & sc) {
	colors.clear();
	colors.resize(SCREEN_HEIGHT*SCREEN_WIDTH);
	A = A + up;
	B = B + up;
	C = C + up;
	D = D + up;
	spec = spec + up;
	sc.newSpec(spec);
	Screen scr(A, B, C, D, SCREEN_WIDTH, SCREEN_HEIGHT);
	sc.newScreen(scr);
	setPixels();
}

void move_DOWN(std::vector<Colour>& colors, Scene & sc) {
	colors.clear();
	colors.resize(SCREEN_HEIGHT*SCREEN_WIDTH);
	A = A - up;
	B = B - up;
	C = C - up;
	D = D - up;
	spec = spec - up;
	sc.newSpec(spec);
	Screen scr(A, B, C, D, SCREEN_WIDTH, SCREEN_HEIGHT);
	sc.newScreen(scr);
	setPixels();
}

void move_RIGHT(std::vector<Colour>& colors, Scene & sc) {
	colors.clear();
	colors.resize(SCREEN_HEIGHT*SCREEN_WIDTH);
	A = A + right;
	B = B + right;
	C = C + right;
	D = D + right;
	spec = spec + right;
	sc.newSpec(spec);
	Screen scr(A, B, C, D, SCREEN_WIDTH, SCREEN_HEIGHT);
	sc.newScreen(scr);
	setPixels();
}

void move_LEFT(std::vector<Colour>& colors, Scene & sc) {
	colors.clear();
	colors.resize(SCREEN_HEIGHT*SCREEN_WIDTH);
	A = A - right;
	B = B - right;
	C = C - right;
	D = D - right;
	spec = spec - right;
	sc.newSpec(spec);
	Screen scr(A, B, C, D, SCREEN_WIDTH, SCREEN_HEIGHT);
	sc.newScreen(scr);
	setPixels();
}

void move_W(std::vector<Colour>& colors, Scene & sc) {
	colors.clear();
	colors.resize(SCREEN_HEIGHT*SCREEN_WIDTH);
	A = A + w;
	B = B + w;
	C = C + w;
	D = D + w;
	spec = spec + w;
	spec = Point(spec.x, spec.y, spec.z + 0.5);
	sc.newSpec(spec);
	Screen scr(A, B, C, D, SCREEN_WIDTH, SCREEN_HEIGHT);
	sc.newScreen(scr);
	setPixels();
}

void move_S(std::vector<Colour>& colors, Scene & sc) {
	colors.clear();
	colors.resize(SCREEN_HEIGHT*SCREEN_WIDTH);
	A = A - w;
	B = B - w;
	C = C - w;
	D = D - w;
	spec = spec - w;
	spec = Point(spec.x, spec.y, spec.z - 0.5);
	sc.newSpec(spec);
	Screen scr(A, B, C, D, SCREEN_WIDTH, SCREEN_HEIGHT);
	sc.newScreen(scr);
	setPixels();
}
double c = 6;

void move_D(std::vector<Colour>& colors, Scene & sc) {
	colors.clear();
	colors.resize(SCREEN_HEIGHT*SCREEN_WIDTH);
	Point delta1 = right*c * 2;
	Point delta2 = up * 2 * c;
	A = A + delta1;
	B = B + delta1;
	C = C - delta2;
	D = D - delta2;
	Point y = up;
	up = right;
	right = y*(-1);
	Screen scr(A, B, C, D, SCREEN_WIDTH, SCREEN_HEIGHT);
	sc.newScreen(scr);
	setPixels();
}

void move_A(std::vector<Colour>& colors, Scene & sc) {
	colors.clear();
	colors.resize(SCREEN_HEIGHT*SCREEN_WIDTH);
	Point delta1 = right*c * 2;
	Point delta2 = up * 2 * c;
	A = A - delta2;
	B = B - delta2;
	C = C - delta1;
	D = D - delta1;
	Point y = up;
	up = right*(-1);
	right = y;
	Screen scr(A, B, C, D, SCREEN_WIDTH, SCREEN_HEIGHT);
	sc.newScreen(scr);
	setPixels();
}




void draw(std::vector<Colour>& colors, Scene& sc) {
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
			const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
			while (true)
			{
				SDL_Event e;
				if (SDL_PollEvent(&e))
				{
					if (e.type == SDL_QUIT)
						break;
					if (keyboardState[SDL_SCANCODE_UP])
						move_UP(colors, sc);
					if (keyboardState[SDL_SCANCODE_DOWN])
						move_DOWN(colors, sc);
					if (keyboardState[SDL_SCANCODE_LEFT])
						move_LEFT(colors, sc);
					if (keyboardState[SDL_SCANCODE_RIGHT])
						move_RIGHT(colors, sc);
					if (keyboardState[SDL_SCANCODE_W])
						move_W(colors, sc);
					if (keyboardState[SDL_SCANCODE_S])
						move_S(colors, sc);
					if (keyboardState[SDL_SCANCODE_D])
						move_D(colors, sc);
					if (keyboardState[SDL_SCANCODE_A])
						move_A(colors, sc);
				}
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
	RTformat("scene.txt");
	for (int i = 2; i < 11; i+=2) {
		for (int j = -2; j < 7; j+=2) {
			Quadrangle* qu = new Quadrangle(Point(i-2.5, j-2.5, -3), Point(i - 2.5, j - 0.5, -3), Point(i - 0.5, j - 0.5, -3), Point(i - 0.5, j - 2.5, -3), Point(0, 0, 1),
				Attributes(Colour{ (double)255*((i+j)/2%2),(double)255 * ((i + j)/2 % 2),(double)255 * ((i + j)/2 % 2) }, Colour{ 0,0,0 }, 50));
			objects.push_back(qu);
		}
	}

	Screen scr(A, B, C, D, SCREEN_WIDTH, SCREEN_HEIGHT);
	sc = Scene(objects, lighters, spec, scr);
	pixels.resize(SCREEN_HEIGHT*SCREEN_WIDTH);
	setPixels();
	draw(pixels, sc);
	for (auto i : objects) {
		delete(i);
	}
	for (auto i : lighters) {
		delete(i);
	}
	return 0;
}

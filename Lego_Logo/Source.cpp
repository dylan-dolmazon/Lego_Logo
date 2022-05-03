#include<SDL.h> 
#include<SDL_ttf.h> 
#include<iostream>
#include "config_sdl.h"
#include "Fonction.h"
#include <math.h>
#include<fstream>

using namespace std;

int main(int argn, char* argv[]) {



	//ouverture de la SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Echec à l’ouverture";
		return 1;
	}

	//on ouvre la police de caractères
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 75);

	Fenetre_demmarage(font);

	//on crée la fenêtre
	SDL_Window* win = SDL_CreateWindow("Lego Logo",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		LARGEUR,
		HAUTEUR,
		SDL_WINDOW_SHOWN
	);

	if (win == NULL)
		cout << "erreur ouverture fenetre";
	SDL_Renderer* rendu = SDL_CreateRenderer(
		win,
		-1,
		SDL_RENDERER_ACCELERATED);

	int const nb_RectD = 6; // nombre de rectangle 
	SDL_Rect RectDroite[nb_RectD]; // tableau  du nombre de rectangle de la fenetre
	 // tableau de deux rectangle de depart et arrivé

	int nb_Obstacle = 0;
	Obstacle tabObstacle[100];
	int nb_ptn = 0; // nombre actuelle de points tracé
	int const ptnMax = 10; // nombre maximum de point dans la fenetre
	SDL_Rect Ptn[ptnMax];// tableau de points pour la ligne 
	tracerFenetre(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);//trace fenetre debut
	Clear_right(rendu);//supprime tout du menu de droite
	Draw_Right(rendu, font, RectDroite);//dessinne la partie de droite 

	Ptn[0].x = RctDA[1].x + RctDA[1].w;
	Ptn[0].y = RctDA[1].y;
	Ptn[0].w = 5;
	Ptn[0].h = 5;
	nb_ptn++;

	int nb_equation = 0; // nombre equation du tracer
	Equation tabEquation[100]; // stock equation des tracer

	SDL_Rect rectMenu2[nb_rectMenu2]; // tableau rectangle du menu 2


	/*************BOUCLE D'evenements**************/
	bool continuer = true;
	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT: //clic sur la croix de fermeture
			continuer = false;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				//si on clique sur le bouton Obstacle
				if (event.button.x > RectDroite[0].x && event.button.x<RectDroite[0].x + RectDroite[0].w && event.button.y>RectDroite[0].y && event.button.y < RectDroite[0].y + RectDroite[0].h) {

					Etape1(rendu, font, RectDroite, tabObstacle, nb_Obstacle, RctDA, Ptn, nb_ptn);

				}
				//on rafraichit
				SDL_RenderPresent(rendu);
				//si on clique sur le bouton dessin
				if (event.button.x > RectDroite[1].x && event.button.x<RectDroite[1].x + RectDroite[1].w && event.button.y>RectDroite[1].y && event.button.y < RectDroite[1].y + RectDroite[1].h) {

					Tracer(rendu, Ptn, font, rectMenu2, nb_ptn, RectDroite, tabObstacle, nb_Obstacle, nb_equation, tabEquation); // appel le deuxieme menu 
				}
				SDL_RenderPresent(rendu);
				//si onj clique sur le bouton Calcul
				if (event.button.x > RectDroite[3].x && event.button.x<RectDroite[3].x + RectDroite[3].w && event.button.y>RectDroite[3].y && event.button.y < RectDroite[3].y + RectDroite[3].h) {

					Calcul(rendu, font, RectDroite, tabObstacle, nb_Obstacle, Ptn, nb_ptn);//envoie dans la fonction dédié pour avoir accès à la règle et au rapporteur
					tracerFenetre(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
				}
				SDL_RenderPresent(rendu);
				if (event.button.x > RectDroite[4].x && event.button.x<RectDroite[4].x + RectDroite[4].w && event.button.y>RectDroite[4].y && event.button.y < RectDroite[4].y + RectDroite[4].h) {
					Instruction(rendu, font, RectDroite);
				}

				if (event.button.x > RectDroite[5].x && event.button.x<RectDroite[5].x + RectDroite[5].w && event.button.y>RectDroite[5].y && event.button.y < RectDroite[5].y + RectDroite[5].h) {
					tracerFenetre(rendu, font, tabObstacle, nb_Obstacle);
					deplacement_Train(rendu, Ptn, font, nb_ptn, nb_equation, tabEquation, tabObstacle, nb_Obstacle);
				}
				SDL_RenderPresent(rendu);//on rafraichit
			}

		}

	}
	SDL_DestroyRenderer(rendu);
	SDL_DestroyWindow(win);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	return 0;
}

// ------------- //
/* FENETRE DEMMARAGE */
// ------------- //

void Fenetre_demmarage(TTF_Font* font) {


	SDL_Window* dem = SDL_CreateWindow("Lego Logo",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		LARGEUR,
		HAUTEUR,
		SDL_WINDOW_SHOWN
	);

	if (dem == NULL)
		cout << "erreur ouverture fenetre";

	SDL_Renderer* view = SDL_CreateRenderer(
		dem,
		-1,
		SDL_RENDERER_ACCELERATED);

	//image de ciel
	SDL_Texture* ciel;
	ciel = loadImage(view, "bateau.jpg");
	SDL_Rect position;
	position.w = LARGEUR;
	position.h = HAUTEUR;
	position.y = 0;
	position.x = 0;
	SDL_RenderCopy(view, ciel, NULL, &position);
	SDL_RenderPresent(view);

	//texte Lego Logo
	SDL_Rect position_welc;
	position_welc.x = LARGEUR / 5 - largeur_texte_welc / 2;
	position_welc.y = HAUTEUR / 4 - hauteur_texte_welc / 2;

	SDL_Texture* texture_welc = loadText(view, "Lego Logo", marron, font);
	SDL_QueryTexture(texture_welc, NULL, NULL, &position_welc.w, &position_welc.h);
	position_welc.w = largeur_texte_welc;
	position_welc.h = hauteur_texte_welc;
	SDL_RenderCopy(view, texture_welc, NULL, &position_welc);
	SDL_DestroyTexture(texture_welc);

	//tracer un rectangle tuto
	SDL_Rect tuto;
	tuto.w = 150;
	tuto.h = 50;
	tuto.x = LARGEUR / 6 - tuto.w / 2;
	tuto.y = HAUTEUR / 2;
	SDL_SetRenderDrawColor(view, 135, 206, 235, 255);
	SDL_RenderFillRect(view, &tuto);
	SDL_SetRenderDrawColor(view, 255, 255, 255, 0);
	SDL_RenderDrawRect(view, &tuto);

	//Texte du rectangle tuto
	SDL_Rect positionTexte_tuto;
	positionTexte_tuto.x = tuto.x + 20;
	positionTexte_tuto.y = tuto.y + 10;
	SDL_Texture* texture_tuto = loadText(view, "Regles", marron, font);
	SDL_QueryTexture(texture_tuto, NULL, NULL, &positionTexte_tuto.w, &positionTexte_tuto.h);
	positionTexte_tuto.w = largeur_texte;
	positionTexte_tuto.h = hauteur_texte;
	SDL_RenderCopy(view, texture_tuto, NULL, &positionTexte_tuto);
	SDL_DestroyTexture(texture_tuto);

	//tracer un rectangle commencer
	SDL_Rect commencer;
	commencer.w = 150;
	commencer.h = 50;
	commencer.x = LARGEUR / 6 - commencer.w / 2;
	commencer.y = HAUTEUR / 2 + 80;
	SDL_SetRenderDrawColor(view, 135, 206, 235, 255);
	SDL_RenderFillRect(view, &commencer);
	SDL_SetRenderDrawColor(view, 255, 255, 255, 255);
	SDL_RenderDrawRect(view, &commencer);

	//Texte du rectangle commencer
	SDL_Rect positionTexte_comm;
	positionTexte_comm.x = commencer.x + 20;
	positionTexte_comm.y = commencer.y + 10;
	SDL_Texture* texture_com = loadText(view, "Commencer", marron, font);
	SDL_QueryTexture(texture_com, NULL, NULL, &positionTexte_comm.w, &positionTexte_comm.h);
	positionTexte_comm.w = largeur_texte;
	positionTexte_comm.h = hauteur_texte;
	SDL_RenderCopy(view, texture_com, NULL, &positionTexte_comm);
	SDL_DestroyTexture(texture_com);

	//tracer un rectangle Quitter
	SDL_Rect quit;
	quit.w = 150;
	quit.h = 50;
	quit.x = LARGEUR / 6 - quit.w / 2;
	quit.y = HAUTEUR / 2 + 160;
	SDL_SetRenderDrawColor(view, 135, 206, 235, 255);
	SDL_RenderFillRect(view, &quit);
	SDL_SetRenderDrawColor(view, 255, 255, 255, 255);
	SDL_RenderDrawRect(view, &quit);

	//Texte du rectangle quitter
	SDL_Rect positionTexte_quit;
	positionTexte_quit.x = quit.x + 20;
	positionTexte_quit.y = quit.y + 10;
	SDL_Texture* texture_quit = loadText(view, "Quitter", marron, font);
	SDL_QueryTexture(texture_quit, NULL, NULL, &positionTexte_quit.w, &positionTexte_quit.h);
	positionTexte_quit.w = largeur_texte;
	positionTexte_quit.h = hauteur_texte;
	SDL_RenderCopy(view, texture_quit, NULL, &positionTexte_quit);
	SDL_DestroyTexture(texture_quit);

	SDL_RenderPresent(view);

	int i = 0;
	bool continuer = true;
	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				//si on clique sur le bouton tuto
				if (event.button.x > tuto.x && event.button.x<tuto.x + tuto.w && event.button.y>tuto.y && event.button.y < tuto.y + tuto.h) {

					Fenetre_regle(font);

				}
				//si on clique sur le bouton commencer
				if (event.button.x > commencer.x && event.button.x<commencer.x + commencer.w && event.button.y>commencer.y && event.button.y < commencer.y + commencer.h) {

					continuer = false;
					//destruction du renderer à la fin
					SDL_DestroyRenderer(view);
					//destruction à la fin
					SDL_DestroyWindow(dem);
					break;
				}

				//si onj clique sur le bouton Quitter
				if (event.button.x > quit.x && event.button.x<quit.x + quit.w && event.button.y>quit.y && event.button.y < quit.y + quit.h) {

					//destruction du renderer à la fin
					SDL_DestroyRenderer(view);
					//destruction à la fin
					SDL_DestroyWindow(dem);


				}

				SDL_RenderPresent(view);

			}
		}
	}



}

void Fenetre_regle(TTF_Font* font) {

	SDL_Window* reg = SDL_CreateWindow("Lego Logo regles",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		LARGEUR - 100,
		HAUTEUR,
		SDL_WINDOW_SHOWN
	);

	if (reg == NULL)
		cout << "erreur ouverture fenetre";


	SDL_Renderer* rend = SDL_CreateRenderer(
		reg,
		-1,
		SDL_RENDERER_ACCELERATED);

	int i = 0;
	int j = 0;

	//image règle
	SDL_Texture* regle;
	regle = loadImage(rend, "notice.png");
	SDL_Rect position;
	position.w = LARGEUR - 100;
	position.h = HAUTEUR;
	position.y = 0;
	position.x = 0;
	SDL_RenderCopy(rend, regle, NULL, &position);
	SDL_RenderPresent(rend);

	bool continuer = true;
	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				//si on clique sur le bouton tuto
				if (event.button.x > 0 && event.button.x < LARGEUR && event.button.y>0 && event.button.y < HAUTEUR + 200) {

					//destruction du renderer à la fin
					SDL_DestroyRenderer(rend);
					//destruction à la fin
					SDL_DestroyWindow(reg);
					continuer = false;
					break;
				}
			}
		}
	}
}

// ------------- //
/* MENU DE DEPART */
// ------------- //

void Clear_right(SDL_Renderer* rendu) {

	//tracer un rectangle qui va mettre toutes la partie de droite en gris
	SDL_Rect black_rect;
	black_rect.x = xMaxPartiGauche;
	black_rect.y = 0;
	black_rect.w = LARGEUR - xMaxPartiGauche;
	black_rect.h = HAUTEUR;
	SDL_SetRenderDrawColor(rendu, 100, 100, 100, 255);
	SDL_RenderFillRect(rendu, &black_rect);

}

void tracerFenetre(SDL_Renderer* rendu,TTF_Font* font, Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int& nb_ptn) {

	//tracer un rectangle pour la couleur de fond
	SDL_Rect fond;	
	fond.x = 0;  
	fond.y = 0; 
	fond.w = xMaxPartiGauche;		
	fond.h = HAUTEUR;		
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderFillRect(rendu, &fond); 

	SDL_SetRenderDrawColor(rendu, 100, 100, 100, 255); 

	//tracer le cadre noir pour délimiter la zonde du déssin
	SDL_RenderDrawLine(rendu, 5, 0, 5, HAUTEUR - 7); //x1,y1,x2,y2 verticale a gauche
	SDL_RenderDrawLine(rendu, xMaxPartiGauche - 5, 0, xMaxPartiGauche - 5, HAUTEUR - 7); //x1,y1,x2,y2 verticale a droite
	SDL_RenderDrawLine(rendu, 5, 0, xMaxPartiGauche - 5, 0); //x1,y1,x2,y2 horizontale en haut
	SDL_RenderDrawLine(rendu, 5, HAUTEUR - 7, xMaxPartiGauche - 5, HAUTEUR - 7); //x1,y1,x2,y2 horizontale en bas


	//texture arrivé
	//image de gare
	SDL_Texture* gare;
	gare = loadImage(rendu, "gare.jpg");
	SDL_Rect positiongare;
	positiongare.w = 120;
	positiongare.h = 90;
	positiongare.x = LARGEUR / 2 + 30;
	positiongare.y = HAUTEUR / 2 - 230;
	SDL_Point centre;
	centre.x = positiongare.x;
	centre.y = positiongare.y;
	SDL_RenderCopyEx(rendu, gare, NULL, &positiongare, NULL, &centre, SDL_FLIP_NONE);
	//stock les coordonnées du rectangle vert
	RctDA[0].x = positiongare.x;
	RctDA[0].y = positiongare.y;
	RctDA[0].w = positiongare.w;
	RctDA[0].h = positiongare.h;

	//texture depart
	//image de depart

	SDL_Texture* depart;
	depart = loadImage(rendu, "depart.jpg");
	SDL_Rect positiondepart;
	positiondepart.w = 100;
	positiondepart.h = 70;
	positiondepart.x = LARGEUR / 2 - 350;
	positiondepart.y = HAUTEUR / 2 + 160;
	centre.x = positiondepart.x;
	centre.y = positiondepart.y;
	SDL_RenderCopyEx(rendu, depart, NULL, &positiondepart, NULL, &centre, SDL_FLIP_NONE);
	//stock les coordonnées du rectangle vert
	RctDA[1].x = positiondepart.x;
	RctDA[1].y = positiondepart.y;
	RctDA[1].w = positiondepart.w;
	RctDA[1].h = positiondepart.h;

	//image de train
	SDL_Texture* train;
	train = loadImage(rendu, "train.jpg");
	SDL_Rect positiontrain;
	positiontrain.w = radius;
	positiontrain.h = 30;
	positiontrain.y = RctDA[1].y;
	positiontrain.x = RctDA[1].x + RctDA[1].w - 20;

	//angle du train de base
	int angle = 30;

	//milieu du train
	SDL_Point millieu;
	millieu.x = (positiontrain.x + (positiontrain.w / 2)) / angle;
	millieu.y = (positiontrain.y + (positiontrain.h / 2)) / angle;

	SDL_RenderCopyEx(rendu, train, NULL, &positiontrain, angle, &millieu, SDL_FLIP_NONE);

	//pour retracer les obstacles et les points à chaque refresh
	if (nb_Obstacle > 0) {
		for (int i = 0; i <= nb_Obstacle; i++) {

			if (tabObstacle[i].type == cercle) {
				SDL_SetRenderDrawColor(rendu, 64, 64, 64, 255);
				Cercle(rendu, tabObstacle[i].x, tabObstacle[i].y, tabObstacle[i].r);
			}
			else {

				Rectangle(rendu, tabObstacle[i].x, tabObstacle[i].y, tabObstacle[i].w, tabObstacle[i].h);
			}
		}
	}
	if (nb_ptn > 1) {
		for (int i = 1; i < nb_ptn; i++) {
			SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau noir
			SDL_RenderFillRect(rendu, &Ptn[i]); //on trace un rectangle plein
			SDL_RenderDrawLine(rendu, Ptn[i - 1].x, Ptn[i - 1].y, Ptn[i].x, Ptn[i].y);
		}
	}
}

void TracerRectangleMenu(SDL_Renderer* rendu, char* Rect1, char* Rect2, char* Rect3, char* Rect4, SDL_Rect* rectMenu2, TTF_Font* font) {

	int i;

	for (i = 0; i < nb_rectMenu2; i++) {

		char nomRect[100];

		if (i == 0) {
			strcpy_s(nomRect, Rect1);
		}
		if (i == 1) {
			strcpy_s(nomRect, Rect2);
		}
		if (i == 2) {
			strcpy_s(nomRect, Rect3);
		}
		if (i == 3) {
			strcpy_s(nomRect, Rect4);
		}if (i == 4) {
			strcpy_s(nomRect, "Retour");
		}

		rectMenu2[i].x = LARGEUR / 2 + 200;
		rectMenu2[i].y = 50 + i * 80;
		rectMenu2[i].w = 150;
		rectMenu2[i].h = 50;
		SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
		SDL_RenderFillRect(rendu, &rectMenu2[i]);
		SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
		SDL_RenderDrawRect(rendu, &rectMenu2[i]); //on trace un rectangle vide

		//Texte des rectangles
		SDL_Rect positionTexte;
		positionTexte.x = rectMenu2[i].x + 20;
		positionTexte.y = rectMenu2[i].y + 10;
		SDL_Texture* texture_retour = loadText(rendu, nomRect, grise, font);
		SDL_QueryTexture(texture_retour, NULL, NULL, &positionTexte.w, &positionTexte.h);
		positionTexte.w = largeur_texte;
		positionTexte.h = hauteur_texte;
		SDL_RenderCopy(rendu, texture_retour, NULL, &positionTexte);
		SDL_DestroyTexture(texture_retour);


	}
	SDL_RenderPresent(rendu);
}

void Draw_Right(SDL_Renderer* rendu, TTF_Font* font, SDL_Rect RectDroite[]) {

	//tracer une ligne noir pour délimiter la zone de déssin
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //on définit la couleur noir (r,g,b,a)
	SDL_RenderDrawLine(rendu, xMaxPartiGauche, 0, xMaxPartiGauche, HAUTEUR); //x1,y1,x2,y2 

	//rectangle obstacle
	SDL_Rect rect1;
	rect1.x = LARGEUR / 2 + 200;
	rect1.y = 25;
	rect1.w = 150;
	rect1.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rect1);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &rect1);

	//on stock les coordonnées du rectangle
	RectDroite[0].x = rect1.x;
	RectDroite[0].y = rect1.y;
	RectDroite[0].w = rect1.w;
	RectDroite[0].h = rect1.h;

	//texte du rectangle 
	SDL_Rect positionTexte_rect;
	positionTexte_rect.x = rect1.x + 20;
	positionTexte_rect.y = rect1.y + 10;
	SDL_Texture* texture = loadText(rendu, "Obstacle", grise, font);
	SDL_QueryTexture(texture, NULL, NULL, &positionTexte_rect.w, &positionTexte_rect.h);
	positionTexte_rect.w = largeur_texte;
	positionTexte_rect.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture, NULL, &positionTexte_rect);
	SDL_DestroyTexture(texture);

	//rectangle dessin
	SDL_Rect rect2;
	rect2.x = LARGEUR / 2 + 200;
	rect2.y = 100;
	rect2.w = 150;
	rect2.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rect2);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &rect2);

	//coordonne du rectangle
	RectDroite[1].x = rect2.x;
	RectDroite[1].y = rect2.y;
	RectDroite[1].w = rect2.w;
	RectDroite[1].h = rect2.h;

	//texte du rectangle 
	SDL_Rect positionTexte_rect2;
	positionTexte_rect2.x = rect2.x + 20;
	positionTexte_rect2.y = rect2.y + 10;
	SDL_Texture* texture2 = loadText(rendu, "Dessin", grise, font);
	SDL_QueryTexture(texture2, NULL, NULL, &positionTexte_rect2.w, &positionTexte_rect2.h);
	positionTexte_rect2.w = largeur_texte;
	positionTexte_rect2.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture2, NULL, &positionTexte_rect2);
	SDL_DestroyTexture(texture2);

	//rectangle Calcul
	SDL_Rect rect4;
	rect4.x = LARGEUR / 2 + 200;
	rect4.y = 175;
	rect4.w = 150;
	rect4.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rect4); //on trace un rectangle plein
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255); //pinceau blanc
	SDL_RenderDrawRect(rendu, &rect4); //on trace un rectangle vide

	//coordonnee 
	RectDroite[3].x = rect4.x;
	RectDroite[3].y = rect4.y;
	RectDroite[3].w = rect4.w;
	RectDroite[3].h = rect4.h;

	//texte du rectangle 
	SDL_Rect positionTexte_rect4;
	positionTexte_rect4.x = rect4.x + 20;
	positionTexte_rect4.y = rect4.y + 10;
	SDL_Texture* texture4 = loadText(rendu, "Calcul", grise, font);
	SDL_QueryTexture(texture4, NULL, NULL, &positionTexte_rect4.w, &positionTexte_rect4.h);
	positionTexte_rect4.w = largeur_texte;
	positionTexte_rect4.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture4, NULL, &positionTexte_rect4);
	SDL_DestroyTexture(texture4);

	//rectangle instruction
	SDL_Rect rect5;
	rect5.x = LARGEUR / 2 + 200;
	rect5.y = 250;
	rect5.w = 150;
	rect5.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rect5);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &rect5);

	//coordonnée
	RectDroite[4].x = rect5.x;
	RectDroite[4].y = rect5.y;
	RectDroite[4].w = rect5.w;
	RectDroite[4].h = rect5.h;

	//texte du rectangle 
	SDL_Rect positionTexte_rect5;
	positionTexte_rect5.x = rect5.x + 20;
	positionTexte_rect5.y = rect5.y + 10;
	SDL_Texture* texture5 = loadText(rendu, "Instruction", grise, font);
	SDL_QueryTexture(texture5, NULL, NULL, &positionTexte_rect5.w, &positionTexte_rect5.h);
	positionTexte_rect5.w = largeur_texte;
	positionTexte_rect5.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture5, NULL, &positionTexte_rect5);
	SDL_DestroyTexture(texture5);

	//rectangle rendu
	SDL_Rect rect6;
	rect6.x = LARGEUR / 2 + 200;
	rect6.y = HAUTEUR / 2 + 190;
	rect6.w = 150;
	rect6.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rect6);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &rect6);

	//coordonnée
	RectDroite[5].x = rect6.x;
	RectDroite[5].y = rect6.y;
	RectDroite[5].w = rect6.w;
	RectDroite[5].h = rect6.h;

	//texte du rectangle 
	SDL_Rect positionTexte_rect6;
	positionTexte_rect6.x = rect6.x + 20;
	positionTexte_rect6.y = rect6.y + 10;
	SDL_Texture* texture6 = loadText(rendu, "Rendu", grise, font);
	SDL_QueryTexture(texture6, NULL, NULL, &positionTexte_rect6.w, &positionTexte_rect6.h);
	positionTexte_rect6.w = largeur_texte;
	positionTexte_rect6.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture6, NULL, &positionTexte_rect6);
	SDL_DestroyTexture(texture6);

	SDL_RenderPresent(rendu);
}

void ActualiserFenetreTracer(SDL_Renderer* rendu, TTF_Font* font, SDL_Rect* rectMenu, SDL_Rect* Ptn, int nb_ptn, Obstacle tabObstacle[100], int& nb_Obstacle) {

	char Verifier[100]; // nom du 1 rectangle menu de droite
	strcpy_s(Verifier, "Verifier");
	char modifier[100];// nom du p2 rectangle menu de droite
	strcpy_s(modifier, "Modifier");
	char tracer[100];// nom du 3 rectangle menu de droite
	strcpy_s(tracer, "tracer");
	char effacer[100];// nom du 4 rectangle menu de droite
	strcpy_s(effacer, "Effacer");

	SDL_RenderClear(rendu);
	tracerFenetre(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
	Clear_right(rendu);
	TracerRectangleMenu(rendu, tracer, modifier, effacer, Verifier, rectMenu, font); // Trace le deuxieme menu a droite
}

void ActualiserFenetreObstacle(SDL_Renderer* rendu, TTF_Font* font, Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int nb_ptn) {

	SDL_RenderClear(rendu);
	tracerFenetre(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
	Clear_right(rendu);
	traceMenuObstacle(rendu, font);

}

void ActualiserFenetreCalcule(SDL_Renderer* rendu, TTF_Font* font, Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int nb_ptn) {

	SDL_RenderClear(rendu);
	tracerFenetre(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
	Clear_right(rendu);
	trace_rect_calcul(rendu, font);

}

void tracerFenetre(SDL_Renderer* rendu, TTF_Font* font, Obstacle tabObstacle[100], int& nb_Obstacle) {

	//tracer un rectangle pour la couleur de fond
	SDL_Rect fond; //on définit le rectangle à tracer	
	fond.x = 0;  //coin en haut à gauche
	fond.y = 0;  //coin en haut à gauche
	fond.w = xMaxPartiGauche;		//largeur
	fond.h = HAUTEUR;		//hauteur
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderFillRect(rendu, &fond); //on trace un rectangle plein

	//changer la couleur du pinceau
	SDL_SetRenderDrawColor(rendu, 100, 100, 100, 255);

	//tracer le cadre noir pour délimiter la zonde du déssin
	SDL_RenderDrawLine(rendu, 5, 0, 5, HAUTEUR - 7); //x1,y1,x2,y2 verticale a gauche
	SDL_RenderDrawLine(rendu, xMaxPartiGauche - 5, 0, xMaxPartiGauche - 5, HAUTEUR - 7); //x1,y1,x2,y2 verticale a droite
	SDL_RenderDrawLine(rendu, 5, 0, xMaxPartiGauche - 5, 0); //x1,y1,x2,y2 horizontale en haut
	SDL_RenderDrawLine(rendu, 5, HAUTEUR - 7, xMaxPartiGauche - 5, HAUTEUR - 7); //x1,y1,x2,y2 horizontale en bas


	//texture arrivé
	//image de gare
	SDL_Texture* gare;
	gare = loadImage(rendu, "gare.jpg");
	SDL_Rect positiongare;
	positiongare.w = 120;
	positiongare.h = 90;
	positiongare.x = LARGEUR / 2 + 30;
	positiongare.y = HAUTEUR / 2 - 230;
	SDL_Point centre;
	centre.x = positiongare.x;
	centre.y = positiongare.y;
	SDL_RenderCopyEx(rendu, gare, NULL, &positiongare, NULL, &centre, SDL_FLIP_NONE);
	//stock les coordonnées du rectangle vert
	RctDA[0].x = positiongare.x;
	RctDA[0].y = positiongare.y;
	RctDA[0].w = positiongare.w;
	RctDA[0].h = positiongare.h;

	//texture depart
	//image de train

	SDL_Texture* depart;
	depart = loadImage(rendu, "depart.jpg");
	SDL_Rect positiondepart;
	positiondepart.w = 100;
	positiondepart.h = 70;
	positiondepart.x = LARGEUR / 2 - 350;
	positiondepart.y = HAUTEUR / 2 + 160;
	centre.x = positiondepart.x;
	centre.y = positiondepart.y;
	SDL_RenderCopyEx(rendu, depart, NULL, &positiondepart, NULL, &centre, SDL_FLIP_NONE);
	//stock les coordonnées du rectangle vert
	RctDA[1].x = positiondepart.x;
	RctDA[1].y = positiondepart.y;
	RctDA[1].w = positiondepart.w;
	RctDA[1].h = positiondepart.h;


	if (nb_Obstacle > 0) {
		for (int i = 0; i <= nb_Obstacle; i++) {

			if (tabObstacle[i].type == cercle) {
				SDL_SetRenderDrawColor(rendu, 64, 64, 64, 255);
				Cercle(rendu, tabObstacle[i].x, tabObstacle[i].y, tabObstacle[i].r);
			}
			else {

				Rectangle(rendu, tabObstacle[i].x, tabObstacle[i].y, tabObstacle[i].w, tabObstacle[i].h);
			}
		}
	}
}

// ------------- //
/* MENU OBSTACLE */
// ------------- //

void traceMenuObstacle(SDL_Renderer* rendu, TTF_Font* font) {
	//tracer un rectangle qui va dessiner des rectangles
	SDL_Rect rect1;
	rect1.x = LARGEUR / 2 + 200;
	rect1.y = HAUTEUR / 2 + -200;
	rect1.w = 150;
	rect1.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rect1);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &rect1); //on trace un rectangle vide de couleur blanche au tour du rectangle

	//texte du rectangle 
	SDL_Rect positionTexte_rect;
	positionTexte_rect.x = rect1.x + 20;
	positionTexte_rect.y = rect1.y + 10;

	SDL_Texture* texture = loadText(rendu, "Rectangle", grise, font);
	SDL_QueryTexture(texture, NULL, NULL, &positionTexte_rect.w, &positionTexte_rect.h);
	positionTexte_rect.w = largeur_texte;
	positionTexte_rect.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture, NULL, &positionTexte_rect);
	SDL_DestroyTexture(texture);

	rect_obstacle[0].x = rect1.x;
	rect_obstacle[0].y = rect1.y;
	rect_obstacle[0].h = rect1.h;
	rect_obstacle[0].w = rect1.w;
	//tracer un rectangle qui va dessiner des cercles 
	SDL_Rect cercle;
	cercle.x = LARGEUR / 2 + 200;
	cercle.y = HAUTEUR / 2 + -100;
	cercle.w = 150;
	cercle.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &cercle);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &cercle);

	//Texte du rectangle cercle
	SDL_Rect positionTexte_cercle;
	positionTexte_cercle.x = cercle.x + 20;
	positionTexte_cercle.y = cercle.y + 10;
	SDL_Texture* texture_cercle = loadText(rendu, "Cercle", grise, font);
	SDL_QueryTexture(texture_cercle, NULL, NULL, &positionTexte_cercle.w, &positionTexte_cercle.h);
	positionTexte_cercle.w = largeur_texte;
	positionTexte_cercle.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture_cercle, NULL, &positionTexte_cercle);
	SDL_DestroyTexture(texture_cercle);

	rect_obstacle[1].x = cercle.x;
	rect_obstacle[1].y = cercle.y;
	rect_obstacle[1].h = cercle.h;
	rect_obstacle[1].w = cercle.w;

	//tracer un rectangle modifier
	SDL_Rect modif;
	modif.x = LARGEUR / 2 + 200;
	modif.y = HAUTEUR / 2;
	modif.w = 150;
	modif.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &modif);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &modif);

	//Texte du rectangle modifier
	SDL_Rect positionTexte_modif;
	positionTexte_modif.x = modif.x + 20;
	positionTexte_modif.y = modif.y + 10;
	SDL_Texture* texture_modif = loadText(rendu, "Modifier", grise, font);
	SDL_QueryTexture(texture_modif, NULL, NULL, &positionTexte_modif.w, &positionTexte_modif.h);
	positionTexte_modif.w = largeur_texte;
	positionTexte_modif.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture_modif, NULL, &positionTexte_modif);
	SDL_DestroyTexture(texture_modif);

	rect_obstacle[2].x = modif.x;
	rect_obstacle[2].y = modif.y;
	rect_obstacle[2].h = modif.h;
	rect_obstacle[2].w = modif.w;


	//tracer un rectangle retour
	SDL_Rect retour;
	retour.x = LARGEUR / 2 + 200;
	retour.y = HAUTEUR / 2 + 100;
	retour.w = 150;
	retour.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &retour);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &retour);

	//Texte du rectangle retour
	SDL_Rect positionTexte_retour;
	positionTexte_retour.x = retour.x + 20;
	positionTexte_retour.y = retour.y + 10;
	SDL_Texture* texture_retour = loadText(rendu, "Retour", grise, font);
	SDL_QueryTexture(texture_retour, NULL, NULL, &positionTexte_retour.w, &positionTexte_retour.h);
	positionTexte_retour.w = largeur_texte;
	positionTexte_retour.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture_retour, NULL, &positionTexte_retour);
	SDL_DestroyTexture(texture_retour);

	rect_obstacle[3].x = retour.x;
	rect_obstacle[3].y = retour.y;
	rect_obstacle[3].h = retour.h;
	rect_obstacle[3].w = retour.w;
}

void Etape1(SDL_Renderer* rendu, TTF_Font* font, SDL_Rect RectDroite[], Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* RctDA, SDL_Rect* Ptn, int nb_ptn) {

	ActualiserFenetreObstacle(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
	SDL_RenderPresent(rendu);
	//Boucle évenement
	bool continuer = true;
	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_MOUSEBUTTONUP:

			if (event.button.button == SDL_BUTTON_LEFT) {
				//rectangle rect est click
				if (event.button.x > rect_obstacle[0].x && event.button.x<rect_obstacle[0].x + rect_obstacle[0].w && event.button.y>rect_obstacle[0].y && event.button.y < rect_obstacle[0].y + rect_obstacle[0].h) {
					
					Dessine_rectangle(rendu, font, event.button.x, event.button.y, tabObstacle, nb_Obstacle, Ptn, nb_ptn, RectDroite,continuer);
					if (!continuer) {
						Clear_right(rendu);
						Draw_Right(rendu, font, RectDroite);
					}

				}
				//rectangle cercle est click
				if (event.button.x > rect_obstacle[1].x && event.button.x<rect_obstacle[1].x + rect_obstacle[1].w && event.button.y>rect_obstacle[1].y && event.button.y < rect_obstacle[1].y + rect_obstacle[1].h) {
					
					Dessine_Cercle(rendu, font, event.button.x, event.button.y, tabObstacle, nb_Obstacle, Ptn, nb_ptn, RectDroite,continuer);
					if (!continuer) {
						Clear_right(rendu);
						Draw_Right(rendu, font, RectDroite);
					}
				}
				//rectangle modif est click
				if (event.button.x > rect_obstacle[2].x && event.button.x<rect_obstacle[2].x + rect_obstacle[2].w && event.button.y>rect_obstacle[2].y && event.button.y < rect_obstacle[2].y + rect_obstacle[2].h) {
					
					ModifierObstacle(rendu, Ptn, font, nb_ptn, RectDroite, tabObstacle, nb_Obstacle,continuer);
					if (!continuer) {
						Clear_right(rendu);
						Draw_Right(rendu, font, RectDroite);
					}
				}
				//rectangle retour est click
				if (event.button.x > rect_obstacle[3].x && event.button.x<rect_obstacle[3].x + rect_obstacle[3].w && event.button.y>rect_obstacle[3].y && event.button.y < rect_obstacle[3].y + rect_obstacle[3].h) {

					continuer = false;
					break;
				}
				SDL_RenderPresent(rendu);//on rafraichit
				break;
			}
		}


	}
	Clear_right(rendu);
	Draw_Right(rendu, font, RectDroite);
}

void Dessine_Cercle(SDL_Renderer* rendu, TTF_Font* font, int x, int y, Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int nb_ptn, SDL_Rect RectDroite[], bool& continuer) {

	int taille = 0;

	SDL_Event event;
	int start_x = 0;
	int start_y = 0;
	int finish_x = 0;
	int finish_y = 0;
	int radius = 0;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_MOUSEBUTTONUP:

			if (event.button.button == SDL_BUTTON_LEFT) {
				//quand on clique dans la zone du dessin
				if (taille == 0) {
					if (event.button.x > 0 && event.button.x < xMaxPartiGauche && event.button.y>0 && event.button.y < HAUTEUR) {
						//dessine un cercle, le centre sera le point cliqué

						start_x = event.button.x;
						start_y = event.button.y;
						taille = 1;
					}

				}
				else {

					ActualiserFenetreObstacle(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
					SDL_SetRenderDrawColor(rendu, 64, 64, 64, 255);
					Cercle(rendu, start_x, start_y, radius);
					SDL_RenderPresent(rendu);
					tabObstacle[nb_Obstacle].x = start_x;
					tabObstacle[nb_Obstacle].y = start_y;
					tabObstacle[nb_Obstacle].r = radius;
					tabObstacle[nb_Obstacle].type = cercle;
					tabObstacle[nb_Obstacle].h = 0;
					tabObstacle[nb_Obstacle].w = 0;

					nb_Obstacle++;
					taille = 0;
					//rectangle retour est click

				}
				if (event.button.x > rect_obstacle[0].x && event.button.x<rect_obstacle[0].x + rect_obstacle[0].w && event.button.y>rect_obstacle[0].y && event.button.y < rect_obstacle[0].y + rect_obstacle[0].h) {

					Dessine_rectangle(rendu, font, event.button.x, event.button.y, tabObstacle, nb_Obstacle, Ptn, nb_ptn, RectDroite, continuer);

				}
				if (event.button.x > rect_obstacle[2].x && event.button.x<rect_obstacle[2].x + rect_obstacle[2].w && event.button.y>rect_obstacle[2].y && event.button.y < rect_obstacle[2].y + rect_obstacle[2].h) {

					ModifierObstacle(rendu, Ptn, font, nb_ptn, RectDroite, tabObstacle, nb_Obstacle, continuer);

				}
				if (event.button.x > rect_obstacle[3].x && event.button.x<rect_obstacle[3].x + rect_obstacle[3].w && event.button.y>rect_obstacle[3].y && event.button.y < rect_obstacle[3].y + rect_obstacle[3].h) {
					continuer = false;
					break;
				}
			}
		case SDL_MOUSEMOTION:
			if (taille == 1) {
				finish_x = event.motion.x;
				finish_y = event.motion.y;
				radius = finish_x - start_x;
				if (radius < 0) {

					radius = start_x - finish_x;

				}

				ActualiserFenetreObstacle(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
				SDL_SetRenderDrawColor(rendu, 64, 64, 64, 255);
				Cercle(rendu, start_x, start_y, radius);
				SDL_RenderPresent(rendu);
			}

		}
	}

}

void Dessine_rectangle(SDL_Renderer* rendu, TTF_Font* font, int x, int y, Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int nb_ptn, SDL_Rect RectDroite[], bool& continuer) {

	int taille = 0;
	SDL_Event event;

	int start_x = 0;
	int start_y = 0;
	int finish_x = 0;
	int finish_y = 0;
	float largeur = 0;
	float hauteur = 0;
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_MOUSEBUTTONUP:

			if (event.button.button == SDL_BUTTON_LEFT) {

				if (taille == 0) {
					if (event.button.x > 0 && event.button.x < xMaxPartiGauche && event.button.y>0 && event.button.y < HAUTEUR) {
						start_x = event.button.x;
						start_y = event.button.y;
						taille = 1;
					}
				}
				else {
					ActualiserFenetreObstacle(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
					Rectangle(rendu, start_x, start_y, largeur, hauteur);
					SDL_RenderPresent(rendu);
					tabObstacle[nb_Obstacle].x = start_x;
					tabObstacle[nb_Obstacle].y = start_y;
					tabObstacle[nb_Obstacle].r = 0;
					tabObstacle[nb_Obstacle].h = hauteur;
					tabObstacle[nb_Obstacle].w = largeur;
					tabObstacle[nb_Obstacle].type = rectangle;

					nb_Obstacle++;
					taille = 0;

				}
			}
			//rectangle cercle est click
			if (event.button.x > rect_obstacle[1].x && event.button.x<rect_obstacle[1].x + rect_obstacle[1].w && event.button.y>rect_obstacle[1].y && event.button.y < rect_obstacle[1].y + rect_obstacle[1].h) {

				Dessine_Cercle(rendu, font, event.button.x, event.button.y, tabObstacle, nb_Obstacle, Ptn, nb_ptn, RectDroite, continuer);

			}
			if (event.button.x > rect_obstacle[2].x && event.button.x<rect_obstacle[2].x + rect_obstacle[2].w && event.button.y>rect_obstacle[2].y && event.button.y < rect_obstacle[2].y + rect_obstacle[2].h) {

				ModifierObstacle(rendu, Ptn, font, nb_ptn, RectDroite, tabObstacle, nb_Obstacle, continuer);

			}
			if (event.button.x > rect_obstacle[3].x && event.button.x<rect_obstacle[3].x + rect_obstacle[3].w && event.button.y>rect_obstacle[3].y && event.button.y < rect_obstacle[3].y + rect_obstacle[3].h) {
				continuer = false;
				break;
			}

			break;
		case SDL_MOUSEMOTION:
			if (taille == 1) {

				finish_x = event.motion.x;
				finish_y = event.motion.y;
				largeur = finish_x - start_x;
				hauteur = finish_y - start_y;
				ActualiserFenetreObstacle(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
				Rectangle(rendu, start_x, start_y, largeur, hauteur);
				SDL_RenderPresent(rendu);
			}


		}
	}
}

void Rectangle(SDL_Renderer* rendu, int x, int y, float largeur, float hauteur) {

	//dessinne une brique
	SDL_Texture* brique;
	brique = loadImage(rendu, "brique.png");
	SDL_Rect positionbrique;
	positionbrique.w = largeur;
	positionbrique.h = hauteur;
	positionbrique.x = x;
	positionbrique.y = y;
	SDL_Point centre;
	centre.x = positionbrique.x;
	centre.y = positionbrique.y;
	SDL_RenderCopyEx(rendu, brique, NULL, &positionbrique, NULL, &centre, SDL_FLIP_NONE);

}

void Cercle(SDL_Renderer* rendu, int cx, int cy, int radius) {

	for (double dy = 1; dy <= radius; dy += 1.0)
	{
		double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
		int x = cx - dx;
		SDL_RenderDrawLine(rendu, cx - dx, cy + dy - radius, cx + dx, cy + dy - radius);
		SDL_RenderDrawLine(rendu, cx - dx, cy - dy + radius, cx + dx, cy - dy + radius);
	}
}

double Calcul_angle(int xa, int ya, int xb, int yb) {

	//ac=xb-xa
	//bc=ya-yb

	int bc = ya - yb;
	if (bc < 0) {

		bc = yb - ya;

	}


	int ac = xb - xa;
	if (ac < 0) {

		ac = xa - xb;

	}

	//calcul hypoténus
	float ab = 0;
	ab = hypot(bc, ac);

	float param = ac / ab;
	float angle = acos(param) * 180 / PI;
	
	return angle;
}

void Modifier_rectangle(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font, int& nb_ptn, SDL_Rect RectDroite[], Obstacle tabObstacle[100], int& nb_Obstacle, bool& continuer) {

	int indice; // retenir point a modifier
	int modif = 0;

	int w; int h;

	SDL_Rect temp;
	temp.h = 10;
	temp.w = 10;
	SDL_Event event;


	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > 0 && event.button.x < xMaxPartiGauche && event.button.y>0 && event.button.y < HAUTEUR) {
					int x = event.button.x; int y = event.button.y;
					if (modif == 0) {
						for (int i = 0; i <= nb_Obstacle; i++) { // test tous les obstacles existants

							if (x > tabObstacle[i].x && x < tabObstacle[i].x + tabObstacle[i].w && y> tabObstacle[i].y && y < tabObstacle[i].y + tabObstacle[i].h) { // si je clique sur un point

								w = tabObstacle[i].w; h = tabObstacle[i].h;
								tabObstacle[i].x = 0; tabObstacle[i].y = 0; tabObstacle[i].h = 0; tabObstacle[i].w = 0;

								indice = i; // retient l'indice du point modifier
								modif = 1;
								break;
							}
						}
					}
					else {
						tabObstacle[indice].x = x; tabObstacle[indice].y = y; tabObstacle[indice].w = w; tabObstacle[indice].h = h;


						ActualiserFenetreObstacle(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
						Rectangle(rendu, x, y, w, h);
						SDL_RenderPresent(rendu);

						modif = 0;
						break;
					}

				}


			}//rectangle rect est click
			if (event.button.x > rect_obstacle[0].x && event.button.x<rect_obstacle[0].x + rect_obstacle[0].w && event.button.y>rect_obstacle[0].y && event.button.y < rect_obstacle[0].y + rect_obstacle[0].h) {

				Dessine_rectangle(rendu, font, event.button.x, event.button.y, tabObstacle, nb_Obstacle, Ptn, nb_ptn, RectDroite, continuer);

			}
			//rectangle cercle est click
			if (event.button.x > rect_obstacle[1].x && event.button.x<rect_obstacle[1].x + rect_obstacle[1].w && event.button.y>rect_obstacle[1].y && event.button.y < rect_obstacle[1].y + rect_obstacle[1].h) {

				Dessine_Cercle(rendu, font, event.button.x, event.button.y, tabObstacle, nb_Obstacle, Ptn, nb_ptn, RectDroite, continuer);

			}
			//rectangle modif est click
			if (event.button.x > rect_obstacle[2].x && event.button.x<rect_obstacle[2].x + rect_obstacle[2].w && event.button.y>rect_obstacle[2].y && event.button.y < rect_obstacle[2].y + rect_obstacle[2].h) {

				ModifierObstacle(rendu, Ptn, font, nb_ptn, RectDroite, tabObstacle, nb_Obstacle, continuer);
				if (!continuer) {
					Clear_right(rendu);
					Draw_Right(rendu, font, RectDroite);
				}
			}

			//rectangle retour est click
			if (event.button.x > rect_obstacle[3].x && event.button.x<rect_obstacle[3].x + rect_obstacle[3].w && event.button.y>rect_obstacle[3].y && event.button.y < rect_obstacle[3].y + rect_obstacle[3].h) {

				continuer = false;

				break;
			}
		case SDL_MOUSEMOTION://appui souris
			if (modif == 1) {
				if (event.motion.x > 0 && event.motion.x < xMaxPartiGauche && event.motion.y>0 && event.motion.y < HAUTEUR && modif == 1) { // si on clique sur la partie gauche de la fenetre

					temp.x = event.motion.x; temp.y = event.motion.y; //retenir point de la souris	
					ActualiserFenetreObstacle(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
					Rectangle(rendu, temp.x, temp.y, w, h);
					SDL_RenderPresent(rendu);

					break;
				}
			}

		}
	}
}

void Modifier_cercle(SDL_Renderer* rendu, TTF_Font* font, Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int nb_ptn, SDL_Rect RectDroite[], bool& continuer) {

	int indice; // retenir point a modifier
	int modif = 0;

	Obstacle temp;
	temp.h = 10;
	temp.w = 10;
	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > 0 && event.button.x < xMaxPartiGauche && event.button.y>0 && event.button.y < HAUTEUR) {

					int x = event.button.x; int y = event.button.y;
					if (modif == 0) {
						for (int i = 0; i <= nb_Obstacle; i++) { // test tous les obstacles existants

							if (((x - tabObstacle[i].x) * (x - tabObstacle[i].x)) + ((y - tabObstacle[i].y) * (y - tabObstacle[i].y)) <= (tabObstacle[i].r * tabObstacle[i].r)) { // si je clique sur un cercle

								temp.r = tabObstacle[i].r;
								tabObstacle[i].r = 0;

								indice = i; // retient l'indice du point modifier
								modif = 1;
								break;
							}
						}
					}
					else {
						tabObstacle[indice].x = x; tabObstacle[indice].y = y;
						tabObstacle[indice].r = temp.r;

						SDL_SetRenderDrawColor(rendu, 64, 64, 64, 255);
						Cercle(rendu, tabObstacle[indice].x, tabObstacle[indice].y, tabObstacle[indice].r);
						SDL_RenderPresent(rendu);

						modif = 0;
						break;
					}

				}//rectangle rect est click
				if (event.button.x > rect_obstacle[0].x && event.button.x<rect_obstacle[0].x + rect_obstacle[0].w && event.button.y>rect_obstacle[0].y && event.button.y < rect_obstacle[0].y + rect_obstacle[0].h) {

					Dessine_rectangle(rendu, font, event.button.x, event.button.y, tabObstacle, nb_Obstacle, Ptn, nb_ptn, RectDroite, continuer);

				}
				//rectangle cercle est click
				if (event.button.x > rect_obstacle[1].x && event.button.x<rect_obstacle[1].x + rect_obstacle[1].w && event.button.y>rect_obstacle[1].y && event.button.y < rect_obstacle[1].y + rect_obstacle[1].h) {

					Dessine_Cercle(rendu, font, event.button.x, event.button.y, tabObstacle, nb_Obstacle, Ptn, nb_ptn, RectDroite, continuer);

				}
				//rectangle modif est click
				if (event.button.x > rect_obstacle[2].x && event.button.x<rect_obstacle[2].x + rect_obstacle[2].w && event.button.y>rect_obstacle[2].y && event.button.y < rect_obstacle[2].y + rect_obstacle[2].h) {

					ModifierObstacle(rendu, Ptn, font, nb_ptn, RectDroite, tabObstacle, nb_Obstacle, continuer);
					if (!continuer) {
						Clear_right(rendu);
						Draw_Right(rendu, font, RectDroite);
					}
				}
				//rectangle retour est click
				if (event.button.x > rect_obstacle[3].x && event.button.x<rect_obstacle[3].x + rect_obstacle[3].w && event.button.y>rect_obstacle[3].y && event.button.y < rect_obstacle[3].y + rect_obstacle[3].h) {

					continuer = false;

					break;
				}

			}
		case SDL_MOUSEMOTION://appui souris
			if (event.motion.x > 0 && event.motion.x < xMaxPartiGauche && event.motion.y>0 && event.motion.y < HAUTEUR && modif == 1) { // si on clique sur la partie gauche de la fenetre


				temp.x = event.motion.x; temp.y = event.motion.y; //retenir point de la souris	
				ActualiserFenetreObstacle(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
				SDL_SetRenderDrawColor(rendu, 64, 64, 64, 255);
				Cercle(rendu, temp.x, temp.y, temp.r);
				SDL_RenderPresent(rendu);


				break;
			}
			break;
		}
	}

}

void ModifierObstacle(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font, int& nb_ptn, SDL_Rect RectDroite[], Obstacle tabObstacle[100], int& nb_Obstacle, bool& continuer) {

	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_MOUSEBUTTONUP:

			if (event.button.button == SDL_BUTTON_LEFT) {
				//rectangle rect est click
				if (event.button.x > rect_obstacle[0].x && event.button.x<rect_obstacle[0].x + rect_obstacle[0].w && event.button.y>rect_obstacle[0].y && event.button.y < rect_obstacle[0].y + rect_obstacle[0].h) {

					Dessine_rectangle(rendu, font, event.button.x, event.button.y, tabObstacle, nb_Obstacle, Ptn, nb_ptn, RectDroite, continuer);

				}
				//rectangle cercle est click
				if (event.button.x > rect_obstacle[1].x && event.button.x<rect_obstacle[1].x + rect_obstacle[1].w && event.button.y>rect_obstacle[1].y && event.button.y < rect_obstacle[1].y + rect_obstacle[1].h) {

					Dessine_Cercle(rendu, font, event.button.x, event.button.y, tabObstacle, nb_Obstacle, Ptn, nb_ptn, RectDroite, continuer);

				}
				//rectangle retour est click
				if (event.button.x > rect_obstacle[3].x && event.button.x<rect_obstacle[3].x + rect_obstacle[3].w && event.button.y>rect_obstacle[3].y && event.button.y < rect_obstacle[3].y + rect_obstacle[3].h) {

					continuer = false;
					break;
				}

				//on test si on à cliquer sur un cercle ou un rectangle
				for (int i = 0; i <= nb_Obstacle; i++) {
					if (tabObstacle[i].type == cercle) {

						if (((event.button.x - tabObstacle[i].x) * (event.button.x - tabObstacle[i].x)) + ((event.button.y - tabObstacle[i].y) * (event.button.y - tabObstacle[i].y)) <= (tabObstacle[i].r * tabObstacle[i].r)) {
							Modifier_cercle(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn, RectDroite, continuer);
						}
					}
					else {

						if (event.button.x > tabObstacle[i].x && event.button.x < tabObstacle[i].x + tabObstacle[i].w && event.button.y> tabObstacle[i].y && event.button.y < tabObstacle[i].y + tabObstacle[i].h) {
							Modifier_rectangle(rendu, Ptn, font, nb_ptn, RectDroite, tabObstacle, nb_Obstacle, continuer);
						}

					}

				}
				SDL_RenderPresent(rendu);//on rafraichit
				break;
			}
		}


	}

}

// ------------- //
/* MENU DESSIN */
// ------------- //

void Tracer(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font, SDL_Rect* rectMenu, int& nb_ptn, SDL_Rect RectDroite[], Obstacle tabObstacle[100], int& nb_Obstacle, int& nb_equation, Equation tabEquation[100]) {

	ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle);
	bool continuer = true;
	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{

		case SDL_MOUSEBUTTONUP:

			if (event.button.button == SDL_BUTTON_LEFT) {//si on clique une nouvelle fois sur Tracer

				if (event.button.x > rectMenu[0].x && event.button.x < rectMenu[0].x + rectMenu[0].w && event.button.y>rectMenu[0].y && event.button.y < rectMenu[0].y + rectMenu[0].h) { // si on clique sur tracer

					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);
					Ligne(rendu, Ptn, font, rectMenu, nb_ptn, RectDroite, tabObstacle, nb_Obstacle, nb_equation, tabEquation, continuer); // appelle fonction qui trace des points relié

					if (!continuer) {
						Clear_right(rendu);
						Draw_Right(rendu, font, RectDroite);
					}

				}
				if (event.button.x > rectMenu[1].x && event.button.x < rectMenu[1].x + rectMenu[1].w && event.button.y>rectMenu[1].y && event.button.y < rectMenu[1].y + rectMenu[1].h) { // si on clique sur modifier
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);
					ModifierLigne(rendu, Ptn, font, nb_ptn, RectDroite, rectMenu, tabObstacle, nb_Obstacle, nb_equation, tabEquation, continuer); // fonction modifie les traits
					if (!continuer) {
						Clear_right(rendu);
						Draw_Right(rendu, font, RectDroite);
					}
				}
				SDL_RenderPresent(rendu);
				if (event.button.x > rectMenu[2].x && event.button.x < rectMenu[2].x + rectMenu[2].w && event.button.y>rectMenu[2].y && event.button.y < rectMenu[2].y + rectMenu[2].h) { // si on clique sur effacer
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);
					nb_ptn = effacerDessin(rendu, Ptn, font, nb_ptn, RectDroite, rectMenu, tabObstacle, nb_Obstacle, nb_equation, tabEquation, continuer); // fonction efface tous les traits

					if (!continuer) {
						Clear_right(rendu);
						Draw_Right(rendu, font, RectDroite);
					}
				}
				SDL_RenderPresent(rendu);
				if (event.button.x > rectMenu[3].x && event.button.x < rectMenu[3].x + rectMenu[3].w && event.button.y>rectMenu[3].y && event.button.y < rectMenu[3].y + rectMenu[3].h) { // si on clique sur effacer
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);
					tracerChemin(rendu, Ptn, font, nb_ptn, nb_equation, tabEquation, tabObstacle, nb_Obstacle, rectMenu);
				}
				SDL_RenderPresent(rendu);
				if (event.button.x > rectMenu[4].x && event.button.x <  rectMenu[4].x + rectMenu[4].w && event.button.y> rectMenu[4].y && event.button.y < rectMenu[4].y + rectMenu[4].h) { // si on clique sur retour
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);
					continuer = false; // arrete while
					Clear_right(rendu);
					Draw_Right(rendu, font, RectDroite);
				}
				SDL_RenderPresent(rendu);
				break;
			}
		}
	}
}

void Ligne(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font, SDL_Rect* rectMenu, int& nb_ptn, SDL_Rect RectDroite[], Obstacle tabObstacle[100], int& nb_Obstacle, int& nb_equation, Equation tabEquation[100], bool& continuer) {

	int a = 0;

	SDL_Rect temp;

	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{

		case SDL_MOUSEMOTION:

			if (event.motion.x > 0 && event.motion.x < xMaxPartiGauche && event.motion.y>0 && event.motion.y < HAUTEUR && a == 0) {
				temp.x = event.motion.x; // se souvenir du x de la souris
				temp.y = event.motion.y;// se souvenir du y de la souris
				temp.h = 10;
				temp.w = 10;

				ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle);
				SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau noir
				SDL_RenderFillRect(rendu, &temp); //on trace un rectangle plein
				SDL_RenderDrawLine(rendu, Ptn[nb_ptn - 1].x, Ptn[nb_ptn - 1].y, temp.x, temp.y); //x1,y1,x2,y2 trait entre les deux points
				SDL_RenderPresent(rendu);
			}

		case SDL_MOUSEBUTTONUP://appui souris
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > 0 && event.button.x < xMaxPartiGauche && event.button.y>0 && event.button.y < HAUTEUR && a == 0) { // si on clique sur la partie gauche de la fenetre

					int x = event.button.x; // se souvenir du x de la souris
					int y = event.button.y;// se souvenir du y de la souris

					if (x > RctDA[0].x && x < RctDA[0].x + RctDA[0].w && y > RctDA[0].y && y < RctDA[0].y + RctDA[0].h) {  // si on clique dans le rectangle d'arrivé place un point a l'angle du rectangle 



						Ptn[nb_ptn].x = RctDA[0].x; // recupere les coordonnée du point a l'angle du rectangle d'arrive
						Ptn[nb_ptn].y = RctDA[0].y + RctDA[0].h;
						Ptn[nb_ptn].w = 10;
						Ptn[nb_ptn].h = 10;

						ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle);
						SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau noir
						SDL_RenderFillRect(rendu, &Ptn[nb_ptn]); //on trace un rectangle plein
						SDL_RenderDrawLine(rendu, Ptn[nb_ptn - 1].x, Ptn[nb_ptn - 1].y, Ptn[nb_ptn].x, Ptn[nb_ptn].y); //x1,y1,x2,y2 trait entre les deux points
						SDL_RenderPresent(rendu);
						a = 1;

					}
					else {// sinon trace une ligne du nouveau point jusqua celui d'avant


						Ptn[nb_ptn].x = x; // recupere les coordonnée du point
						Ptn[nb_ptn].y = y;
						Ptn[nb_ptn].w = 10;
						Ptn[nb_ptn].h = 10;

						// sinon trace une ligne du nouveau point jusqua celui d'avant 
						ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle);
						SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau noir
						SDL_RenderFillRect(rendu, &Ptn[nb_ptn]); //on trace un rectangle plein
						SDL_RenderDrawLine(rendu, Ptn[nb_ptn - 1].x, Ptn[nb_ptn - 1].y, Ptn[nb_ptn].x, Ptn[nb_ptn].y); //x1,y1,x2,y2 trait entre les deux points
						SDL_RenderPresent(rendu);
					}

					Equation_Droite(Ptn[nb_ptn - 1].x, Ptn[nb_ptn - 1].y, Ptn[nb_ptn].x, Ptn[nb_ptn].y, tabEquation, nb_equation);
					nb_equation++;
					nb_ptn++; // on met 1 point en plus 
				}
				if (event.button.x > rectMenu[0].x && event.button.x < rectMenu[0].x + rectMenu[0].w && event.button.y>rectMenu[0].y && event.button.y < rectMenu[0].y + rectMenu[0].h) { // si on clique sur tracer
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);
					Ligne(rendu, Ptn, font, rectMenu, nb_ptn, RectDroite, tabObstacle, nb_Obstacle, nb_equation, tabEquation, continuer); // appelle fonction trace des points relié

				}
				if (event.button.x > rectMenu[1].x && event.button.x < rectMenu[1].x + rectMenu[1].w && event.button.y>rectMenu[1].y && event.button.y < rectMenu[1].y + rectMenu[1].h) { // si on clique sur modifier
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);
					ModifierLigne(rendu, Ptn, font, nb_ptn, RectDroite, rectMenu, tabObstacle, nb_Obstacle, nb_equation, tabEquation, continuer);
				}
				SDL_RenderPresent(rendu);
				if (event.button.x > rectMenu[2].x && event.button.x < rectMenu[2].x + rectMenu[2].w && event.button.y>rectMenu[2].y && event.button.y < rectMenu[2].y + rectMenu[2].h) { // si on clique sur effacer
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);
					nb_ptn = effacerDessin(rendu, Ptn, font, nb_ptn, RectDroite, rectMenu, tabObstacle, nb_Obstacle, nb_equation, tabEquation, continuer);
				}
				SDL_RenderPresent(rendu);
				SDL_RenderPresent(rendu);
				if (event.button.x > rectMenu[3].x && event.button.x < rectMenu[3].x + rectMenu[3].w && event.button.y>rectMenu[3].y && event.button.y < rectMenu[3].y + rectMenu[3].h) { // si on clique sur effacer
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);
					tracerChemin(rendu, Ptn, font, nb_ptn, nb_equation, tabEquation, tabObstacle, nb_Obstacle, rectMenu);
				}
				SDL_RenderPresent(rendu);
				if (event.button.x > rectMenu[4].x && event.button.x <  rectMenu[4].x + rectMenu[4].w && event.button.y> rectMenu[4].y && event.button.y < rectMenu[4].y + rectMenu[4].h) { // si on clique sur retour
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);
					continuer = false;
				}
				SDL_RenderPresent(rendu);
				break;
			}
		}
	}
}

void ModifierLigne(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font, int& nb_ptn, SDL_Rect RectDroite[], SDL_Rect* rectMenu, Obstacle tabObstacle[100], int& nb_Obstacle, int& nb_equation, Equation tabEquation[100], bool& continuer) {

	int indice; // retenir point a modifier
	int modif = 0;

	int x; int y; int h; int w;

	SDL_Rect temp;
	temp.h = 10;
	temp.w = 10;
	SDL_Event event;
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > 0 && event.button.x < xMaxPartiGauche && event.button.y>0 && event.button.y < HAUTEUR) {

					int x = event.button.x; int y = event.button.y;
					if (modif == 0) {
						for (int i = 1; i < nb_ptn - 1; i++) { // test tous les points existants

							if (x > Ptn[i].x && x < Ptn[i].x + Ptn[i].w && y> Ptn[i].y && y < Ptn[i].y + Ptn[i].h) { // si je clique sur un point

								ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
								SDL_RenderPresent(rendu);


								indice = i; // retient l'indice du point modifier
								modif = 1;
								break;
							}
						}
					}
					else {
						Ptn[indice].x = x; Ptn[indice].y = y;

						ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
						SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau noir
						SDL_RenderFillRect(rendu, &Ptn[indice]); //on trace un rectangle plein
						SDL_RenderDrawLine(rendu, Ptn[indice - 1].x, Ptn[indice - 1].y, Ptn[indice].x, Ptn[indice].y);
						SDL_RenderDrawLine(rendu, Ptn[indice].x, Ptn[indice].y, Ptn[indice + 1].x, Ptn[indice + 1].y);//x1,y1,x2,y2 
						SDL_RenderPresent(rendu);

						Equation_Droite(Ptn[indice - 1].x, Ptn[indice - 1].y, Ptn[indice].x, Ptn[indice].y, tabEquation, indice - 1);
						Equation_Droite(Ptn[indice].x, Ptn[indice].y, Ptn[indice + 1].x, Ptn[indice + 1].y, tabEquation, indice);
						modif = 0;
						break;
					}

				}

			}if (event.button.x > rectMenu[0].x && event.button.x < rectMenu[0].x + rectMenu[0].w && event.button.y>rectMenu[0].y && event.button.y < rectMenu[0].y + rectMenu[0].h) {
				ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
				SDL_RenderPresent(rendu);
				Ligne(rendu, Ptn, font, rectMenu, nb_ptn, RectDroite, tabObstacle, nb_Obstacle, nb_equation, tabEquation, continuer); // appelle fonction trace des points relié

			}
			if (event.button.x > rectMenu[1].x && event.button.x < rectMenu[1].x + rectMenu[1].w && event.button.y>rectMenu[1].y && event.button.y < rectMenu[1].y + rectMenu[1].h) {
				ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
				SDL_RenderPresent(rendu);
				ModifierLigne(rendu, Ptn, font, nb_ptn, RectDroite, rectMenu, tabObstacle, nb_Obstacle, nb_equation, tabEquation, continuer);
			}
			SDL_RenderPresent(rendu);//on rafraichit
			if (event.button.x > rectMenu[2].x && event.button.x < rectMenu[2].x + rectMenu[2].w && event.button.y>rectMenu[2].y && event.button.y < rectMenu[2].y + rectMenu[2].h) {
				ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
				SDL_RenderPresent(rendu);
				nb_ptn = effacerDessin(rendu, Ptn, font, nb_ptn, RectDroite, rectMenu, tabObstacle, nb_Obstacle, nb_equation, tabEquation, continuer);
			}
			SDL_RenderPresent(rendu);
			if (event.button.x > rectMenu[3].x && event.button.x < rectMenu[3].x + rectMenu[3].w && event.button.y>rectMenu[3].y && event.button.y < rectMenu[3].y + rectMenu[3].h) { // si on clique sur effacer
				ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
				SDL_RenderPresent(rendu);//on rafraichit8
				tracerChemin(rendu, Ptn, font, nb_ptn, nb_equation, tabEquation, tabObstacle, nb_Obstacle, rectMenu);
			}
			SDL_RenderPresent(rendu);//on rafraichit
			if (event.button.x > rectMenu[4].x && event.button.x <  rectMenu[4].x + rectMenu[4].w && event.button.y> rectMenu[4].y && event.button.y < rectMenu[4].y + rectMenu[4].h) {
				ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
				SDL_RenderPresent(rendu);//on rafraichit8
				continuer = false;

			}
			SDL_RenderPresent(rendu);
			break;


		case SDL_MOUSEMOTION://appui souris
			if (event.button.x > 0 && event.button.x < xMaxPartiGauche && event.button.y>0 && event.button.y < HAUTEUR && modif == 1) { // si on clique sur la partie gauche de la fenetre

				temp.x = event.button.x; temp.y = event.button.y; //retenir point de la souris	

				ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
				SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau noir
				SDL_RenderFillRect(rendu, &temp); //on trace un rectangle plein
				SDL_RenderDrawLine(rendu, Ptn[indice - 1].x, Ptn[indice - 1].y, temp.x, temp.y);
				SDL_RenderDrawLine(rendu, temp.x, temp.y, Ptn[indice + 1].x, Ptn[indice + 1].y);//x1,y1,x2,y2 
				SDL_RenderPresent(rendu);//on rafraichit8



				break;
			}

		}
	}
}

int effacerDessin(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font, int& nb_ptn, SDL_Rect RectDroite[], SDL_Rect* rectMenu, Obstacle tabObstacle[100], int& nb_Obstacle, int& nb_equation, Equation tabEquation[100], bool& continuer) {


	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{

		case SDL_MOUSEBUTTONUP:

			if (event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > 0 && event.button.x < xMaxPartiGauche && event.button.y>0 && event.button.y < HAUTEUR) { // si on clique sur la partie gauche
					// efface tous les traits et points deja tracé
					for (int i = 1; i < nb_ptn - 1; i++) {
						SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);	//pinceau blanc
						SDL_RenderFillRect(rendu, &Ptn[i]); //on trace un rectangle plein
						SDL_RenderDrawLine(rendu, Ptn[i - 1].x, Ptn[i - 1].y, Ptn[i].x, Ptn[i].y);
						SDL_RenderDrawLine(rendu, Ptn[i + 1].x, Ptn[i + 1].y, Ptn[i].x, Ptn[i].y);

					}
					SDL_RenderFillRect(rendu, &Ptn[nb_ptn - 1]); //on efface rectangle plein d'arrivé 
					nb_ptn = 1;
					nb_equation = 0;
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle);

					// remet nombre de point a un car on veut changer les coordonnées des points de tableau de point mais on enléve pas le premier point. 
				}
				if (event.button.x > rectMenu[0].x && event.button.x < rectMenu[0].x + rectMenu[0].w && event.button.y>rectMenu[0].y && event.button.y < rectMenu[0].y + rectMenu[0].h) {
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);//on rafraichit8
					Ligne(rendu, Ptn, font, rectMenu, nb_ptn, RectDroite, tabObstacle, nb_Obstacle, nb_equation, tabEquation, continuer); // appelle fonction trace des points relié

				}
				if (event.button.x > rectMenu[1].x && event.button.x < rectMenu[1].x + rectMenu[1].w && event.button.y>rectMenu[1].y && event.button.y < rectMenu[1].y + rectMenu[1].h) {
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);//on rafraichit8
					ModifierLigne(rendu, Ptn, font, nb_ptn, RectDroite, rectMenu, tabObstacle, nb_Obstacle, nb_equation, tabEquation, continuer);
				}
				SDL_RenderPresent(rendu);//on rafraichit
				if (event.button.x > rectMenu[2].x && event.button.x < rectMenu[2].x + rectMenu[2].w && event.button.y>rectMenu[2].y && event.button.y < rectMenu[2].y + rectMenu[2].h) {
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);//on rafraichit8
					nb_ptn = effacerDessin(rendu, Ptn, font, nb_ptn, RectDroite, rectMenu, tabObstacle, nb_Obstacle, nb_equation, tabEquation, continuer);
				}
				SDL_RenderPresent(rendu);//on rafraichit
				if (event.button.x > rectMenu[3].x && event.button.x < rectMenu[3].x + rectMenu[3].w && event.button.y>rectMenu[3].y && event.button.y < rectMenu[3].y + rectMenu[3].h) { // si on clique sur effacer
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);//on rafraichit8
					tracerChemin(rendu, Ptn, font, nb_ptn, nb_equation, tabEquation, tabObstacle, nb_Obstacle, rectMenu);
				}
				SDL_RenderPresent(rendu);//on rafraichit
				if (event.button.x > rectMenu[4].x && event.button.x <  rectMenu[4].x + rectMenu[4].w && event.button.y> rectMenu[4].y && event.button.y < rectMenu[4].y + rectMenu[4].h) {
					ActualiserFenetreTracer(rendu, font, rectMenu, Ptn, nb_ptn, tabObstacle, nb_Obstacle); // trace Fenetre menu tracer
					SDL_RenderPresent(rendu);//on rafraichit8
					continuer = false;
				}
				SDL_RenderPresent(rendu);
				break;
			}
		}
	}

	return nb_ptn;
}

void tracerChemin(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font, int nb_ptn, int& nb_equation, Equation tabEquation[100], Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* rectMenu) {

	float A;
	float B;
	float Y;
	float X;
	float xj;
	float yj;

	//savoir si il y a une diff de 50 entre le Xd et le Xa pour choisir la bonne inconnue
	int diff;

	//savoir si il y a une colision
	bool colision = false;
	for (int i = 1; i < nb_ptn; i++) {
		diff = Ptn[i].x - Ptn[i - 1].x;
		if (diff >= 50) {
			for (X = Ptn[i - 1].x; X <= Ptn[i].x; X++) {
				//calcul du y grace à l'équation de la droite
				Y = tabEquation[i - 1].A * X + tabEquation[i - 1].B;
				if (X >= Ptn[i - 1].x && X < Ptn[i].x) {
					for (int k = 0; k < nb_Obstacle; k++) {
						if (tabObstacle[k].type == rectangle) {
							colision = TestColisionRectangle(tabObstacle, k, X, Y, radius);

						}
						if (tabObstacle[k].type == cercle) {
							colision = TestColisionCercle(tabObstacle, k, X, Y, radius);

						}if (colision) {
							k = nb_Obstacle;
						}
					}
					if (colision) {
						SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
					}
					else {
						SDL_SetRenderDrawColor(rendu, 128, 128, 128, 255);
					}

					Cercle(rendu, X, Y, radius);
					SDL_RenderPresent(rendu);
				}
			}
		}
		else {
			for (Y = Ptn[i - 1].y; Y > Ptn[i].y; Y--) {
				//connaitre le x grace à l'"quation de la droite
				X = (Y - tabEquation[i - 1].B) / tabEquation[i - 1].A;
				if (Y > Ptn[i].y && Y < Ptn[i - 1].y) {
					for (int k = 0; k < nb_Obstacle; k++) {
						if (tabObstacle[k].type == rectangle) {
							colision = TestColisionRectangle(tabObstacle, k, X, Y, radius);

						}
						if (tabObstacle[k].type == cercle) {
							colision = TestColisionCercle(tabObstacle, k, X, Y, radius);

						}if (colision) {
							k = nb_Obstacle;
						}
					}
					if (colision) {
						SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
					}
					else {
						SDL_SetRenderDrawColor(rendu, 128, 128, 128, 255);
					}
					Cercle(rendu, X, Y, radius);
					SDL_RenderPresent(rendu);
				}
			}
		}


	}

}

void Equation_Droite(int xa, int ya, int xb, int yb, Equation tabEquation[100], int nb_equation) {

	float A = (float)(yb - ya) / (float)(xb - xa);
	float B = (float)ya - A * (float)xa;
	tabEquation[nb_equation].A = A;
	tabEquation[nb_equation].B = B;
}

bool TestColisionCercle(Obstacle  tabObstacle[], int indice, int X, float Y, int radius) {

	bool colision = false;

	//connaitre les coordonnees min pour définir le point de départ pour ne pas tester tous les points de la fenetres
	int xmin;
	int ymin;

	//connaitre les coordonnees max pour définir le point d'arriver pour ne pas tester tous les points de la fenetres
	int xmax;
	int ymax;

	if (tabObstacle[indice].x > X) {
		xmin = X;
		xmax = tabObstacle[indice].x;
	}
	else {
		xmin = tabObstacle[indice].r;
		xmax = X;
	}
	if (tabObstacle[indice].y > Y) {
		ymin = Y;
		ymax = tabObstacle[indice].y;
	}
	else {
		ymax = Y;
		ymin = tabObstacle[indice].y;
	}
	//parcourir les points entre l'obsatcles et le cercle que l'on teste pour savoir si le point appartient à la fois à l'obsatcle et au cercle
	for (int xp = xmin; xp <= xmax; xp++) {
		for (int yp = ymin; yp <= ymax; yp++) {
			if (((xp - X) * (xp - X)) + ((yp - Y) * (yp - Y)) <= (radius * radius) && ((xp - tabObstacle[indice].x) * (xp - tabObstacle[indice].x)) + ((yp - tabObstacle[indice].y) * (yp - tabObstacle[indice].y)) <= (tabObstacle[indice].r * tabObstacle[indice].r)) {

				colision = true;
				return colision;

			}
		}
	}
	//retourne true si il y a une colision false sinon
	return colision;
}

bool TestColisionRectangle(Obstacle  tabObstacle[], int indice, int X, float Y, int radius) {

	bool colision = false;

	int xmin;
	int ymin;
	int xmax;
	int ymax;

	if (tabObstacle[indice].x > X) {
		xmin = X;
		xmax = tabObstacle[indice].x;
	}
	else {
		xmin = tabObstacle[indice].r;
		xmax = X;
	}
	if (tabObstacle[indice].y > Y) {
		ymin = Y;
		ymax = tabObstacle[indice].y;
	}
	else {
		ymax = Y;
		ymin = tabObstacle[indice].y;
	}
	//parcourir les points entre l'obsatcles et le cercle que l'on teste pour savoir si le point appartient à la fois à l'obsatcle et au cercle
	for (int xp = xmin; xp <= xmax; xp++) {
		for (int yp = ymin; yp <= ymax; yp++) {

			if (((xp - X) * (xp - X)) + ((yp - Y) * (yp - Y)) <= (radius * radius) && (xp >= tabObstacle[indice].x) && (xp <= tabObstacle[indice].x + tabObstacle[indice].w) && (yp >= tabObstacle[indice].y) && (yp <= (tabObstacle[indice].y + tabObstacle[indice].h))) {

				colision = true;
				return colision;

			}

		}
	}
	return colision;
}

// ------------- //
/* MENU CALCULE */
// ------------- //

void Calcul(SDL_Renderer* rendu, TTF_Font* font, SDL_Rect RectDroite[], Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int& nb_ptn) {

	ActualiserFenetreCalcule(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
	SDL_RenderPresent(rendu);

	bool continuer = true;
	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{

		case SDL_MOUSEBUTTONUP:

			//boutton règle
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > rect_calcul[0].x && event.button.x<rect_calcul[0].x + rect_calcul[0].w && event.button.y>rect_calcul[0].y && event.button.y < rect_calcul[0].y + rect_calcul[0].h) {
					//fonction qui gère tout de la règle
					Deplacement_regle(rendu, font,tabObstacle, nb_Obstacle, Ptn, nb_ptn,continuer);
					if (!continuer) {
						Clear_right(rendu);
						Draw_Right(rendu, font, RectDroite);
					}
				}

				if (event.button.x > rect_calcul[1].x && event.button.x<rect_calcul[1].x + rect_calcul[1].w && event.button.y>rect_calcul[1].y && event.button.y < rect_calcul[1].y + rect_calcul[1].h) {
					//fonction qui gère tout du rapporteur
					Deplacement_rapporteur(rendu, font,tabObstacle, nb_Obstacle, Ptn, nb_ptn,continuer);
					if (!continuer) {
						Clear_right(rendu);
						Draw_Right(rendu, font, RectDroite);
					}
				}

				//boutton retour

				if (event.button.x > rect_calcul[2].x && event.button.x<rect_calcul[2].x + rect_calcul[2].w && event.button.y>rect_calcul[2].y && event.button.y < rect_calcul[2].y + rect_calcul[2].h) {

					Clear_right(rendu);
					Draw_Right(rendu, font, RectDroite);
					continuer = false;

				}
				SDL_RenderPresent(rendu);
				break;

			}
		}

	}
}

void Deplacement_regle(SDL_Renderer* rendu, TTF_Font* font, Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int& nb_ptn, bool& continuer) {

	int depart = 0;

	//image de règle
	SDL_Texture* règle;
	règle = loadImage(rendu, "regle.jpg");
	SDL_Rect positionregle;
	positionregle.w = xMaxPartiGauche;
	positionregle.h = 50;
	positionregle.y = 0;
	positionregle.x = 0;
	SDL_RenderPresent(rendu);

	SDL_Event event;
	int cpt = 0;
	int start_x = 0;
	int start_y = 0;
	int finish_x = 0;
	int finish_y = 0;

	while (continuer) {

		SDL_WaitEvent(&event);
		switch (event.type)
		{

		case SDL_MOUSEBUTTONUP:

			//place la règle ou l'enfant veux aux points données avec un angle de 40 degré
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (depart == 0) {
					if (event.button.x > 0 && event.button.x < xMaxPartiGauche && event.button.y>0 && event.button.y < HAUTEUR) {

						if (event.button.button == SDL_BUTTON_LEFT) {

							start_x = event.button.x;
							start_y = event.button.y;

							depart = 1;
						}

					}
				}
				else {
					//on recupère les position x et y du bouton
					positionregle.x = start_x;
					positionregle.y = start_y;
					finish_x = event.button.x;
					finish_y = event.button.y;
					double angle = Calcul_angle(start_x, start_y, finish_x, finish_y);
					SDL_Point centre;
					centre.x = (positionregle.x + (positionregle.w / 2)) / angle;
					centre.y = (positionregle.y + (positionregle.h / 2)) / angle;

					if (start_x > finish_x) {

						angle = 180 - angle;

					}
					if (start_y < finish_y) {

						angle = 0 - angle;

					}

					//on créé un point qui permet de centrer la rotation autour de lui

					ActualiserFenetreCalcule(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
					SDL_RenderCopyEx(rendu, règle, NULL, &positionregle, -angle, &centre, SDL_FLIP_NONE);
					SDL_RenderPresent(rendu);
					depart = 0;
				}
			}
			if (event.button.x > rect_calcul[1].x && event.button.x<rect_calcul[1].x + rect_calcul[1].w && event.button.y>rect_calcul[1].y && event.button.y < rect_calcul[1].y + rect_calcul[1].h) {
				//fonction qui gère tout du rapporteur
				Deplacement_rapporteur(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn, continuer);

			}
			if (event.button.x > rect_calcul[0].x && event.button.x<rect_calcul[0].x + rect_calcul[0].w && event.button.y>rect_calcul[0].y && event.button.y < rect_calcul[0].y + rect_calcul[0].h) {
				//fonction qui gère tout de la règle
				Deplacement_regle(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn, continuer);

			}if (event.button.x > rect_calcul[2].x && event.button.x<rect_calcul[2].x + rect_calcul[2].w && event.button.y>rect_calcul[2].y && event.button.y < rect_calcul[2].y + rect_calcul[2].h) {

				continuer = false;

			}
			SDL_RenderPresent(rendu);
			break;

		case SDL_MOUSEMOTION:
			if (depart == 1) {
				positionregle.x = start_x;
				positionregle.y = start_y;
				finish_x = event.motion.x;
				finish_y = event.motion.y;

				double angle = Calcul_angle(start_x, start_y, finish_x, finish_y);
				SDL_Point centre;
				centre.x = (positionregle.x + (positionregle.w / 2)) / angle;
				centre.y = (positionregle.y + (positionregle.h / 2)) / angle;

				if (start_x > finish_x) {

					angle = 180 - angle;

				}
				if (start_y < finish_y) {

					angle = 0 - angle;

				}

				//on créé un point qui permet de centrer la rotation autour de lui

				ActualiserFenetreCalcule(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
				SDL_RenderCopyEx(rendu, règle, NULL, &positionregle, -angle, &centre, SDL_FLIP_NONE);
				SDL_RenderPresent(rendu);

			}
		}



	}

}

void Deplacement_rapporteur(SDL_Renderer* rendu, TTF_Font* font, Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int& nb_ptn, bool& continuer) {

	int depart = 0;

	//image de rapporteur
	SDL_Texture* rappo;
	rappo = loadImage(rendu, "rapteur.png");
	SDL_Rect position_rapp;
	position_rapp.w = 250;
	position_rapp.h = 150;

	int start_x = 0;
	int start_y = 0;
	int finish_x = 0;
	int finish_y = 0;

	SDL_Event event;
	SDL_Point centre;
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{

		case SDL_MOUSEBUTTONUP:

			//clique dans le dessin
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (depart == 0) {
					if (event.button.x > 0 && event.button.x < xMaxPartiGauche && event.button.y>0 && event.button.y < HAUTEUR) {
						//place le centre du rapporteur la on on à cliqué 
						start_x = event.button.x;
						start_y = event.button.y;
						depart = 1;
					}
				}
				else {
					//met le curseur au milieu de l'image
					position_rapp.x = start_x - position_rapp.w / 2;
					position_rapp.y = start_y - position_rapp.h / 2;
					double angle = Calcul_angle(start_x, start_y, finish_x, finish_y);
					//defini le centre de rotation
					centre.x = position_rapp.w / 2;
					centre.y = position_rapp.h / 2;

					if (start_x > finish_x) {

						angle = 180 + angle;
						ActualiserFenetreCalcule(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
						SDL_RenderCopyEx(rendu, rappo, NULL, &position_rapp, angle, &centre, SDL_FLIP_NONE);
						SDL_RenderPresent(rendu);
					}
					if (start_y < finish_y) {

						angle = angle - 90;
						ActualiserFenetreCalcule(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
						SDL_RenderCopyEx(rendu, rappo, NULL, &position_rapp, angle, &centre, SDL_FLIP_NONE);
						SDL_RenderPresent(rendu);
					}

					else {
						ActualiserFenetreCalcule(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
						SDL_RenderCopyEx(rendu, rappo, NULL, &position_rapp, -angle, &centre, SDL_FLIP_NONE);
						SDL_RenderPresent(rendu);

					}
					depart = 0;
				}



				if (event.button.x > rect_calcul[0].x && event.button.x<rect_calcul[0].x + rect_calcul[0].w && event.button.y>rect_calcul[0].y && event.button.y < rect_calcul[0].y + rect_calcul[0].h) {
					//fonction qui gère tout de la règle
					Deplacement_regle(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn, continuer);

				}
				if (event.button.x > rect_calcul[2].x && event.button.x<rect_calcul[2].x + rect_calcul[2].w && event.button.y>rect_calcul[2].y && event.button.y < rect_calcul[2].y + rect_calcul[2].h) {

					continuer = false;

				}
				SDL_RenderPresent(rendu);
			}

		case SDL_MOUSEMOTION:
			if (depart == 1) {
				finish_x = event.motion.x;
				finish_y = event.motion.y;
				position_rapp.x = start_x - position_rapp.w / 2;
				position_rapp.y = start_y - position_rapp.h / 2;
				double angle = Calcul_angle(start_x, start_y, finish_x, finish_y);
				centre.x = position_rapp.w / 2;
				centre.y = position_rapp.h / 2;

				if (start_x > finish_x) {

					angle = 180 + angle;
					ActualiserFenetreCalcule(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
					SDL_RenderCopyEx(rendu, rappo, NULL, &position_rapp, angle, &centre, SDL_FLIP_NONE);
					SDL_RenderPresent(rendu);
				}
				if (start_y < finish_y) {

					angle = angle - 90;
					ActualiserFenetreCalcule(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
					SDL_RenderCopyEx(rendu, rappo, NULL, &position_rapp, angle, &centre, SDL_FLIP_NONE);
					SDL_RenderPresent(rendu);
				}

				else {
					ActualiserFenetreCalcule(rendu, font, tabObstacle, nb_Obstacle, Ptn, nb_ptn);
					SDL_RenderCopyEx(rendu, rappo, NULL, &position_rapp, -angle, &centre, SDL_FLIP_NONE);
					SDL_RenderPresent(rendu);

				}
			}
		}
	}
}

void trace_rect_calcul(SDL_Renderer* rendu, TTF_Font* font) {

	Clear_right(rendu);

	//tracer un rectangle règle
	SDL_Rect rect_regle;
	rect_regle.x = LARGEUR / 2 + 200;
	rect_regle.y = HAUTEUR / 2 + -200;
	rect_regle.w = 150;
	rect_regle.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rect_regle);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &rect_regle);

	//texte règle
	SDL_Rect positionTexte_regle;
	positionTexte_regle.x = rect_regle.x + 20;
	positionTexte_regle.y = rect_regle.y + 10;
	SDL_Texture* texture_regle = loadText(rendu, "Regle", grise, font);
	SDL_QueryTexture(texture_regle, NULL, NULL, &positionTexte_regle.w, &positionTexte_regle.h);
	positionTexte_regle.w = largeur_texte;
	positionTexte_regle.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture_regle, NULL, &positionTexte_regle);
	SDL_DestroyTexture(texture_regle);

	rect_calcul[0].x = rect_regle.x;
	rect_calcul[0].y = rect_regle.y;
	rect_calcul[0].w = rect_regle.w;
	rect_calcul[0].h = rect_regle.h;

	//tracer un rectangle rapporteur
	SDL_Rect rapp;
	rapp.x = LARGEUR / 2 + 200;
	rapp.y = HAUTEUR / 2 + -100;
	rapp.w = 150;
	rapp.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rapp);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &rapp);

	//Texte du rectangle rapporteur
	SDL_Rect positionTexte_rapp;
	positionTexte_rapp.x = rapp.x + 20;
	positionTexte_rapp.y = rapp.y + 10;
	SDL_Texture* texture_rapp = loadText(rendu, "Rapporteur", grise, font);
	SDL_QueryTexture(texture_rapp, NULL, NULL, &positionTexte_rapp.w, &positionTexte_rapp.h);
	positionTexte_rapp.w = largeur_texte;
	positionTexte_rapp.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture_rapp, NULL, &positionTexte_rapp);
	SDL_DestroyTexture(texture_rapp);

	rect_calcul[1].x = rapp.x;
	rect_calcul[1].y = rapp.y;
	rect_calcul[1].w = rapp.w;
	rect_calcul[1].h = rapp.h;

	// tracer un rectangle retour
	SDL_Rect retour;
	retour.x = LARGEUR / 2 + 200;
	retour.y = HAUTEUR / 2;
	retour.w = 150;
	retour.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &retour);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &retour); //on trace un rectangle vide

	//Texte du rectangle retour
	SDL_Rect positionTexte_retour;
	positionTexte_retour.x = retour.x + 20;
	positionTexte_retour.y = retour.y + 10;
	SDL_Texture* texture_retour = loadText(rendu, "Retour", grise, font);
	SDL_QueryTexture(texture_retour, NULL, NULL, &positionTexte_retour.w, &positionTexte_retour.h);
	positionTexte_retour.w = largeur_texte;
	positionTexte_retour.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture_retour, NULL, &positionTexte_retour);
	SDL_DestroyTexture(texture_retour);

	rect_calcul[2].x = retour.x;
	rect_calcul[2].y = retour.y;
	rect_calcul[2].w = retour.w;
	rect_calcul[2].h = retour.h;

}

// ------------- //
/* MENU RENDU */
// ------------- //

void deplacement_Train(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font, int nb_ptn, int& nb_equation, Equation tabEquation[100], Obstacle tabObstacle[100], int& nb_Obstacle) {

	float A;
	float B;
	float Y;
	float X;
	float xj;
	float yj;

	int diff;

	//image de train
	SDL_Texture* train;
	train = loadImage(rendu, "train.jpg");
	SDL_Rect positiontrain;
	positiontrain.w = radius;
	positiontrain.h = 30;
	positiontrain.y = Ptn[0].y;
	positiontrain.x = Ptn[0].x;

	bool colision = false;
	for (int i = 1; i < nb_ptn; i++) {
		diff = Ptn[i].x - Ptn[i - 1].x;
		if (diff >= 50) {
			for (X = Ptn[i - 1].x; X <= Ptn[i].x; X++) {
				Y = tabEquation[i - 1].A * X + tabEquation[i - 1].B;
				if (X >= Ptn[i - 1].x && X < Ptn[i].x) {
					positiontrain.x = X;
					positiontrain.y = Y;
					double angle = Calcul_angle(positiontrain.x, positiontrain.y, Ptn[i].x, Ptn[i].y);
					SDL_Point centre;
					centre.x = (positiontrain.x + (positiontrain.w / 2)) / angle;
					centre.y = (positiontrain.y + (positiontrain.h / 2)) / angle;

					tracerFenetre(rendu, font, tabObstacle, nb_Obstacle);
					SDL_RenderCopyEx(rendu, train, NULL, &positiontrain, angle, &centre, SDL_FLIP_NONE);
					SDL_RenderPresent(rendu);
				}
			}
		}
		else {
			for (Y = Ptn[i - 1].y; Y > Ptn[i].y; Y--) {
				X = (Y - tabEquation[i - 1].B) / tabEquation[i - 1].A;
				if (Y > Ptn[i].y && Y < Ptn[i - 1].y) {
					double angle = Calcul_angle(positiontrain.x, positiontrain.y, Ptn[i].x, Ptn[i].y);
					positiontrain.x = X;
					positiontrain.y = Y;
					SDL_Point centre;
					centre.x = (positiontrain.x + (positiontrain.w / 2)) / angle;
					centre.y = (positiontrain.y + (positiontrain.h / 2)) / angle;
					tracerFenetre(rendu, font, tabObstacle, nb_Obstacle);
					SDL_RenderCopyEx(rendu, train, NULL, &positiontrain, angle, &centre, SDL_FLIP_NONE);
					SDL_RenderPresent(rendu);

				}
			}
		}


	}
}

// ------------- //
/* MENU INSTRUCTION */
// ------------- //
	
void Instruction(SDL_Renderer* rendu, TTF_Font* font, SDL_Rect RectDroite[]) {

	Clear_right(rendu);

	//tracer un rectangle avancer
	SDL_Rect rect_avancer;
	rect_avancer.x = LARGEUR / 2 + 200;
	rect_avancer.y = 25;
	rect_avancer.w = 150;
	rect_avancer.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rect_avancer);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &rect_avancer);

	//texte avancer
	SDL_Rect positionTexte_avancer;
	positionTexte_avancer.x = rect_avancer.x + 20;
	positionTexte_avancer.y = rect_avancer.y + 10;
	SDL_Texture* texture_avancer = loadText(rendu, "AV", grise, font);
	SDL_QueryTexture(texture_avancer, NULL, NULL, &positionTexte_avancer.w, &positionTexte_avancer.h);
	positionTexte_avancer.w = largeur_texte;
	positionTexte_avancer.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture_avancer, NULL, &positionTexte_avancer);
	SDL_DestroyTexture(texture_avancer);

	//tracer un rectangle tourner gauche
	SDL_Rect rect_tourner;
	rect_tourner.x = LARGEUR / 2 + 200;
	rect_tourner.y = 100;
	rect_tourner.w = 150;
	rect_tourner.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rect_tourner);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &rect_tourner);

	//texte tourner gauche
	SDL_Rect positionTexte_tourner;
	positionTexte_tourner.x = rect_tourner.x + 20;
	positionTexte_tourner.y = rect_tourner.y + 10;
	SDL_Texture* texture_tourner = loadText(rendu, "TG", grise, font);
	SDL_QueryTexture(texture_tourner, NULL, NULL, &positionTexte_tourner.w, &positionTexte_tourner.h);
	positionTexte_tourner.w = largeur_texte;
	positionTexte_tourner.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture_tourner, NULL, &positionTexte_tourner);
	SDL_DestroyTexture(texture_tourner);

	//tracer un rectangle tourner droit
	SDL_Rect rect_tourner_dr;
	rect_tourner_dr.x = LARGEUR / 2 + 200;
	rect_tourner_dr.y = 175;
	rect_tourner_dr.w = 150;
	rect_tourner_dr.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rect_tourner_dr);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &rect_tourner_dr);

	//texte tourner droit
	SDL_Rect positionTexte_tourner_dr;
	positionTexte_tourner_dr.x = rect_tourner_dr.x + 20;
	positionTexte_tourner_dr.y = rect_tourner_dr.y + 10;
	SDL_Texture* texture_tourner_dr = loadText(rendu, "TD", grise, font);
	SDL_QueryTexture(texture_tourner_dr, NULL, NULL, &positionTexte_tourner_dr.w, &positionTexte_tourner_dr.h);
	positionTexte_tourner_dr.w = largeur_texte;
	positionTexte_tourner_dr.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture_tourner_dr, NULL, &positionTexte_tourner_dr);
	SDL_DestroyTexture(texture_tourner_dr);

	//rectangle ou on notera les instructions
	SDL_Rect rect_instru;
	rect_instru.x = LARGEUR / 2 + 200;
	rect_instru.y = 250;
	rect_instru.w = 150;
	rect_instru.h = 180;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rect_instru);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &rect_instru);

	// tracer un rectangle retour
	SDL_Rect retour;
	retour.x = LARGEUR / 2 + 200;
	retour.y = HAUTEUR - 55;
	retour.w = 150;
	retour.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &retour);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &retour); //on trace un rectangle vide

	//Texte du rectangle retour
	SDL_Rect positionTexte_retour;
	positionTexte_retour.x = retour.x + 20;
	positionTexte_retour.y = retour.y + 10;
	SDL_Texture* texture_retour = loadText(rendu, "Retour", grise, font);
	SDL_QueryTexture(texture_retour, NULL, NULL, &positionTexte_retour.w, &positionTexte_retour.h);
	positionTexte_retour.w = largeur_texte;
	positionTexte_retour.h = hauteur_texte;
	SDL_RenderCopy(rendu, texture_retour, NULL, &positionTexte_retour);
	SDL_DestroyTexture(texture_retour);

	SDL_RenderPresent(rendu);

	ofstream sortie("test.txt", ios::trunc);
	ifstream entree("test.txt", ios::in);
	
	int cpt_inscrpition = 0;
	char tmp[10];
	bool continuer = true;
	SDL_Event event;
	//compter le nombre d'instruction pour pouvoir les affichers les un sous les autres
	int nb_inscrpition = 0;
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{

		case SDL_MOUSEBUTTONDOWN:
			//boutton avancer
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > rect_avancer.x && event.button.x<rect_avancer.x + rect_avancer.w && event.button.y>rect_avancer.y && event.button.y < rect_avancer.y + rect_avancer.h) {
					//fonction pour avancer 
					sortie << "AV";
					//on demande le chiffre qui correspond qui sera stocker dans un fichier
					Pave_numerique(font);
					//on met un espace pour après récuperer les instruction plus facilement
					sortie << " ";
					//on recupère le nombre
					ifstream don("nombre.txt", ios::in);
					don >> tmp;
					//on le stock à la suite de l'instruction
					sortie << tmp;
					sortie << endl;
					cpt_inscrpition++;
					don.close();

				}
				//tourner gauche
				if (event.button.x > rect_tourner.x && event.button.x<rect_tourner.x + rect_tourner.w && event.button.y>rect_tourner.y && event.button.y < rect_tourner.y + rect_tourner.h) {

					sortie << "TG";
					Pave_numerique(font);
					//on recupère le nombre
					ifstream don("nombre.txt", ios::in);
					sortie << " ";
					don >> tmp;
					//on le stock à la suite de l'instruction
					sortie << tmp;
					sortie << endl;
					cpt_inscrpition++;
					don.close();
				}
				//tourner droit
				if (event.button.x > rect_tourner_dr.x && event.button.x<rect_tourner_dr.x + rect_tourner_dr.w && event.button.y>rect_tourner_dr.y && event.button.y < rect_tourner_dr.y + rect_tourner_dr.h) {

					sortie << "TD";
					Pave_numerique(font);
					sortie << " ";
					//on recupère le nombre
					ifstream don("nombre.txt", ios::in);
					don >> tmp;
					//on le stock à la suite de l'instruction
					sortie << tmp;
					sortie << endl;
					cpt_inscrpition++;
					don.close();
				}
				//retour
				if (event.button.x > retour.x && event.button.x<retour.x + retour.w && event.button.y>retour.y && event.button.y < retour.y + retour.h) {

					continuer = false;
					break;
				}
			}

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {

				char tmp[100];

				for (int j = nb_inscrpition; j < cpt_inscrpition; j++) {

					entree.getline(tmp, 100);
					cout << tmp;
					SDL_Rect positionTexte_rect_instru;
					positionTexte_rect_instru.x = rect_instru.x + 5;
					positionTexte_rect_instru.y = (rect_instru.y + 5) + hauteur_texte_instruction_rect * nb_inscrpition;
					SDL_Texture* texture_instru = loadText(rendu, tmp, grise, font);
					SDL_QueryTexture(texture_instru, NULL, NULL, &positionTexte_rect_instru.w, &positionTexte_rect_instru.h);
					positionTexte_rect_instru.w = largeur_texte_instruction + 10;
					//valeur spécial car si on à 3 ou 2 chiffres la distance entre les deux en hauteur varie
					positionTexte_rect_instru.h = hauteur_texte_instruction_rect;

					//si le texte dépasse on le met pas	
					if (positionTexte_rect_instru.y + positionTexte_rect_instru.h > rect_instru.y + rect_instru.h) {

						SDL_DestroyTexture(texture_instru);
						break;

					}
					else {

						SDL_RenderCopy(rendu, texture_instru, NULL, &positionTexte_rect_instru);
						SDL_DestroyTexture(texture_instru);
						nb_inscrpition++;

					}


				}
				SDL_RenderPresent(rendu);
				if (event.button.x > retour.x && event.button.x<retour.x + retour.w && event.button.y>retour.y && event.button.y < retour.y + retour.h) {

					continuer = false;
					break;
				}

			}


		}
	}
	Clear_right(rendu);
	Draw_Right(rendu, font, RectDroite);
	sortie.close();
	entree.close();
}

void Pave_numerique(TTF_Font* font) {

	SDL_Window* pav = SDL_CreateWindow("Pave numerique",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		largeur_calcul,
		hauteur_calcul,
		SDL_WINDOW_SHOWN
	);

	if (pav == NULL)
		cout << "erreur ouverture fenetre";

	SDL_Renderer* vu = SDL_CreateRenderer(
		pav,
		-1,
		SDL_RENDERER_ACCELERATED);

	//on recupère les donnée du fichier donc (les chiffres de 0 à 9 avec leurs lettres associé)
	ifstream nombre("pave.txt", ios::in);
	//creation d'un fichier pour stocker les valeurs à écrire dans le cadre des instructions
	ofstream donnee("nombre.txt", ios::trunc);

	//on test que le fichier est présent, on ne le fait pas pour les autres car on sait qu'ils existent
	if (!nombre) {

		cout << "p";

	}

	char nom_rect[2];
	char numero[2];
	//variable tmp ou on stock les nombres écris pour les affichers en temps réel 
	char nb_ecris_doc[10] = "4";
	//on regroupe les rectangles pour pouvoir récupérer leurs coordonnée plus tard 
	Pave tab[10];

	int j = 0;
	//on compte le nombre de rectangles
	int cpt = 0;
	//tous les 3 rectangles on saute une ligne et on a 4 lignes
	for (int i = 0; j < 3; i++) {

		if (i == 3) {

			j++;
			i = 0;
		}

		nombre >> nom_rect;
		nombre >> numero;
		// tracer un rectangle 
		SDL_Rect nom_rect;
		nom_rect.w = 50;
		nom_rect.h = 50;
		nom_rect.x = nom_rect.w * i;
		nom_rect.y = nom_rect.h * j;
		SDL_SetRenderDrawColor(vu, 0, 0, 0, 255);
		SDL_RenderFillRect(vu, &nom_rect);
		SDL_SetRenderDrawColor(vu, 255, 255, 255, 255);
		SDL_RenderDrawRect(vu, &nom_rect); //on trace un rectangle vide

		//Texte
		SDL_Rect positionTexte_retour;
		positionTexte_retour.x = nom_rect.x + nom_rect.w / 4;
		positionTexte_retour.y = nom_rect.y + nom_rect.h / 4;
		SDL_Texture* texture_retour = loadText(vu, numero, grise, font);
		SDL_QueryTexture(texture_retour, NULL, NULL, &positionTexte_retour.w, &positionTexte_retour.h);
		positionTexte_retour.w = largeur_texte_instruction;
		positionTexte_retour.h = hauteur_texte_instruction;
		SDL_RenderCopy(vu, texture_retour, NULL, &positionTexte_retour);
		SDL_DestroyTexture(texture_retour);

		strcpy_s(tab[cpt].nom, numero);
		tab[cpt].x = nom_rect.x;
		tab[cpt].y = nom_rect.y;
		tab[cpt].w = nom_rect.w;
		tab[cpt].h = nom_rect.h;
		cpt++;

	}

	// tracer un rectangle valider
	SDL_Rect valider;
	valider.x = 50;
	valider.y = 150;
	valider.w = 100;
	valider.h = 50;
	SDL_SetRenderDrawColor(vu, 0, 0, 0, 255);
	SDL_RenderFillRect(vu, &valider);
	SDL_SetRenderDrawColor(vu, 255, 255, 255, 255);
	SDL_RenderDrawRect(vu, &valider); //on trace un rectangle vide

	//Texte du rectangle valider
	SDL_Rect positionTexte_valider;
	positionTexte_valider.x = valider.x + 7;
	positionTexte_valider.y = valider.y + 10;
	SDL_Texture* texture_valider = loadText(vu, "Entree", grise, font);
	SDL_QueryTexture(texture_valider, NULL, NULL, &positionTexte_valider.w, &positionTexte_valider.h);
	positionTexte_valider.w = largeur_texte;
	positionTexte_valider.h = hauteur_texte;
	SDL_RenderCopy(vu, texture_valider, NULL, &positionTexte_valider);
	SDL_DestroyTexture(texture_valider);

	SDL_RenderPresent(vu);//on rafraichit

	bool continuer = true;
	SDL_Event event;//gestion des évènements souris/clavier, 

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT: //clic sur la croix de fermeture
			continuer = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {

				if (event.button.x > tab[0].x && event.button.x<tab[0].x + tab[0].w && event.button.y>tab[0].y && event.button.y < tab[0].y + tab[0].h) {
					//on met à jour le fichier 
					ofstream donnee("nombre.txt", ios::app);
					donnee << "0";
					donnee.close();
				}
				if (event.button.x > tab[1].x && event.button.x<tab[1].x + tab[1].w && event.button.y>tab[1].y && event.button.y < tab[1].y + tab[1].h) {
					ofstream donnee("nombre.txt", ios::app);
					donnee << "1";
					donnee.close();
				}
				if (event.button.x > tab[2].x && event.button.x<tab[2].x + tab[2].w && event.button.y>tab[2].y && event.button.y < tab[2].y + tab[2].h) {
					ofstream donnee("nombre.txt", ios::app);
					donnee << "2";

				}
				if (event.button.x > tab[3].x && event.button.x<tab[3].x + tab[3].w && event.button.y>tab[3].y && event.button.y < tab[3].y + tab[3].h) {

					ofstream donnee("nombre.txt", ios::app);
					donnee << "3";
					donnee.close();

				}
				if (event.button.x > tab[4].x && event.button.x<tab[4].x + tab[4].w && event.button.y>tab[4].y && event.button.y < tab[4].y + tab[4].h) {

					ofstream donnee("nombre.txt", ios::app);
					donnee << "4";
					donnee.close();

				}
				if (event.button.x > tab[5].x && event.button.x<tab[5].x + tab[5].w && event.button.y>tab[5].y && event.button.y < tab[5].y + tab[5].h) {

					ofstream donnee("nombre.txt", ios::app);
					donnee << "5";
					donnee.close();

				}
				if (event.button.x > tab[6].x && event.button.x<tab[6].x + tab[6].w && event.button.y>tab[6].y && event.button.y < tab[6].y + tab[6].h) {

					ofstream donnee("nombre.txt", ios::app);
					donnee << "6";
					donnee.close();

				}
				if (event.button.x > tab[7].x && event.button.x<tab[7].x + tab[7].w && event.button.y>tab[7].y && event.button.y < tab[7].y + tab[7].h) {

					ofstream donnee("nombre.txt", ios::app);
					donnee << "7";
					donnee.close();

				}
				if (event.button.x > tab[8].x && event.button.x<tab[8].x + tab[8].w && event.button.y>tab[8].y && event.button.y < tab[8].y + tab[8].h) {

					ofstream donnee("nombre.txt", ios::app);
					donnee << "8";
					donnee.close();
				}
				if (event.button.x > tab[9].x && event.button.x<tab[9].x + tab[9].w && event.button.y>tab[9].y && event.button.y < tab[9].y + tab[9].h) {

					ofstream donnee("nombre.txt", ios::app);
					donnee << "9";
					donnee.close();

				}
				if (event.button.x > valider.x && event.button.x<valider.x + valider.w && event.button.y>valider.y && event.button.y < valider.y + valider.h) {

					continuer = false;
					break;
				}

			}

		case SDL_MOUSEBUTTONUP:

			if (event.button.button == SDL_BUTTON_LEFT) {

				ifstream lecture("nombre.txt", ios::in);
				lecture >> nb_ecris_doc;
				cout << nb_ecris_doc;

				// tracer un rectangle vu nombre
				SDL_Rect vu_nb;
				vu_nb.x = largeur_calcul / 2;
				vu_nb.y = 10;
				vu_nb.w = 150;
				vu_nb.h = 50;
				SDL_SetRenderDrawColor(vu, 255, 255, 255, 255);
				SDL_RenderFillRect(vu, &vu_nb);
				SDL_SetRenderDrawColor(vu, 0, 0, 0, 255);
				SDL_RenderDrawRect(vu, &vu_nb); //on trace un rectangle vide

				//Texte du rectangle vu nombre
				SDL_Rect positionTexte_vu_nb;
				positionTexte_vu_nb.x = vu_nb.x + 10;
				positionTexte_vu_nb.y = vu_nb.y + 10;
				SDL_Texture* texture_vu_nb = loadText(vu, nb_ecris_doc, grise, font);
				SDL_QueryTexture(texture_vu_nb, NULL, NULL, &positionTexte_vu_nb.w, &positionTexte_vu_nb.h);
				positionTexte_vu_nb.w = largeur_texte;
				positionTexte_vu_nb.h = hauteur_texte;
				SDL_RenderCopy(vu, texture_vu_nb, NULL, &positionTexte_vu_nb);
				SDL_DestroyTexture(texture_vu_nb);

			}
			SDL_RenderPresent(vu);//on rafraichit

		}
	}
	donnee.close();
	nombre.close();
	SDL_DestroyRenderer(vu);
	//destruction à la fin
	SDL_DestroyWindow(pav);

}










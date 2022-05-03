const int LARGEUR = 750; //largeur fenetre
const int HAUTEUR = 500;  //hauteur fenetre
const int largeur_calcul = 320; //largeur fenettre instruction
const int hauteur_calcul = 230; //hauteur fenettre instruction
const int xMaxPartiGauche = 547; //x ne pas depasser de la feuille
const int  radius = 20; // rayon des cercles test et w du train 
const int  largeur_texte = 90; // largeur texte
const int  hauteur_texte = 35; // hauteur texte
const int  largeur_texte_instruction = 30; // largeur texte des instructions
const int  hauteur_texte_instruction = 20;// hauteur texte des instructions
const int  hauteur_texte_instruction_rect = 25;
const int  largeur_texte_welc = 250; // texte welcome
const int  hauteur_texte_welc = 150; // texte welcome
int  const nb_rectMenu2 = 5; // nombre rectangle sur le deuxieme menu de tracer
SDL_Color violet = { 255,0,255 }; // couleur violete
SDL_Color grise = { 100,100,100 };// couleur grise
SDL_Color marron = { 110,51,0 }; // couleur marron
SDL_Color blanc = { 250,240,197 }; // couleur blanc
SDL_Rect rect_obstacle[4]; // menu rectangle dans obstacle
SDL_Rect rect_calcul[4];  // menu rectangle dans calcul
const int PI = 3.14159265; // pi 
enum TypeObstacle {cercle,rectangle}; // type entre deux obstacle
struct Obstacle {

	int x;
	int y;
	int r;
	float h;
	float w;
	TypeObstacle type;

}; // structure pour obstacle
//calculer le a et le b de ax+b
struct Equation {
	float A;
	float B;
}; 
//sert à récuperer les coordonnées des touches du pavé numérique
struct Pave {

	char nom[3];
	int x;
	int y;
	int h;
	int w;
}; 
int const nb_RectDeAr = 2; // rectangle depart et arrivé
SDL_Rect RctDA[nb_RectDeAr];

// ------------- //
/* FENETRE DEMMARAGE */
// ------------- //

void Fenetre_demmarage(TTF_Font* font); // fonction demmarrage programme fenetre
void Fenetre_regle(TTF_Font* font); // affiche les regles


// ------------- //
/* MENU DE DEPART */
// ------------- //

void Clear_right(SDL_Renderer* rendu); // efface rectangle droite
void tracerFenetre(SDL_Renderer* rendu, TTF_Font* font,Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int& nb_ptn); // fait fenetre gauche
void TracerRectangleMenu(SDL_Renderer* rendu, char* rect1, char* rect2, char* rect3, char* rect4, SDL_Rect* rectMenu2, TTF_Font* font); // dessinne les rectangles de base
void Draw_Right(SDL_Renderer* rendu, TTF_Font* font,SDL_Rect RectDroite[]); // dessine droite de la fenetre de base
void ActualiserFenetreTracer(SDL_Renderer* rendu, TTF_Font* font, SDL_Rect* rectMenu, SDL_Rect* Ptn, int nb_ptn, Obstacle tabObstacle[100], int& nb_Obstacle);//actualise la partie dessin
void ActualiserFenetreObstacle(SDL_Renderer* rendu, TTF_Font* font0, Obstacle tabObstacle[100], int& nb_Obstacle,SDL_Rect* Ptn, int nb_ptn);
void ActualiserFenetreCalcule(SDL_Renderer* rendu, TTF_Font* font, Obstacle tabObstacle[100], int& nb_Obstacle,SDL_Rect* Ptn, int nb_ptn);
void tracerFenetre(SDL_Renderer* rendu, TTF_Font* font, Obstacle tabObstacle[100], int& nb_Obstacle);//trace fenettre de gauche pour le rendu final

// ------------- //
/* MENU OBSTACLE */
// ------------- //

void traceMenuObstacle(SDL_Renderer* rendu, TTF_Font* font);//trace la partie de droite pour les obstacles
void Etape1(SDL_Renderer* rendu, TTF_Font* font,SDL_Rect RectDroite[], Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* RctDA, SDL_Rect* Ptn, int nb_ptn); // apres bouton Obstacle
void Dessine_Cercle(SDL_Renderer* rendu, TTF_Font* font,int x, int y,Obstacle tabObstacle[100], int& nb_Obstacle,SDL_Rect* Ptn, int nb_ptn, SDL_Rect RectDroite[], bool& continuer); // l'orsqu'on clique Cercle
void Dessine_rectangle(SDL_Renderer* rendu, TTF_Font* font,int x, int y, Obstacle tabObstacle[100], int& nb_Obstacle,SDL_Rect* Ptn, int nb_ptn, SDL_Rect RectDroite[], bool& continuer); // lorsqu'on clique sur rectangle
void Rectangle(SDL_Renderer* rendu, int x, int y, float largeur, float hauteur); // trace un rectangle
void Cercle(SDL_Renderer* rendu, int cx, int cy, int radius); // trace un cercle 
double Calcul_angle(int xa, int ya, int xb, int yb); // calcule un angle demande
void Modifier_rectangle(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font, int& nb_ptn, SDL_Rect RectDroite[], Obstacle tabObstacle[100], int& nb_Obstacle, bool& continuer);//redessine le rectangle choisi
void Modifier_cercle(SDL_Renderer* rendu, TTF_Font* font, Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int nb_ptn, SDL_Rect RectDroite[], bool& continuer);//redessine le cercle choisi
void ModifierObstacle(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font, int& nb_ptn, SDL_Rect RectDroite[], Obstacle tabObstacle[100], int& nb_Obstacle, bool& continuer);//defini si on à clique dans un rectangle ou un cercle

// ------------- //
/* MENU DESSIN */
// ------------- //

void Tracer(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font,SDL_Rect* rectMenu2, int& nb_ptn, SDL_Rect RectDroite[],Obstacle tabObstacle[100], int& nb_Obstacle, int &nb_equation, Equation tabEquation[100]); // apres avoir cliqu� sur Dessin
void Ligne(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font,SDL_Rect* rectMenu,  int& nb_ptn, SDL_Rect RectDroite[],Obstacle tabObstacle[100], int& nb_Obstacle, int &nb_equation, Equation tabEquation[100], bool& continuer); // Lorsqu'on clique sur tracer
void ModifierLigne(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font,int& nb_ptn, SDL_Rect RectDroite[],SDL_Rect* rectMenu, Obstacle tabObstacle[100], int& nb_Obstacle, int &nb_equation, Equation tabEquation[100], bool& continuer); // lorsqu'on clique sur modifier
int effacerDessin(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font,int& nb_ptn, SDL_Rect RectDroite[],SDL_Rect* rectMenu, Obstacle tabObstacle[100], int& nb_Obstacle, int &nb_equation, Equation tabEquation[100], bool& continuer); // efface tous les traits
void tracerChemin(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font,int nb_ptn, int& nb_equation, Equation tabEquation[100], Obstacle tabObstacle[100], int& nb_Obstacle,SDL_Rect* rectMenu); // rendu
void Equation_Droite(int xa, int ya, int xb, int yb, Equation tabEquation[100], int nb_equation);//calcul l'équation d'une droite
bool TestColisionCercle(Obstacle  tabObstacle[], int indice, int X, float Y, int radius); // test si il y a colision entre cercle et trait
bool TestColisionRectangle(Obstacle  tabObstacle[], int indice, int X, float Y, int radius); //test s'il y a colision entre rectangle et trait

// ------------- //
/* MENU CALCULE */
// ------------- //

void Calcul(SDL_Renderer* rendu,TTF_Font* font,SDL_Rect RectDroite[],Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int& nb_ptn); // lorsqu'on clique sur calcule
void Deplacement_regle(SDL_Renderer* rendu, TTF_Font* font,Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int& nb_ptn , bool& continuer); // lorsqu'on clique sur regle
void Deplacement_rapporteur(SDL_Renderer* rendu, TTF_Font* font,Obstacle tabObstacle[100], int& nb_Obstacle, SDL_Rect* Ptn, int& nb_ptn , bool& continuer); // clique sur rapporteur 
void trace_rect_calcul(SDL_Renderer* rendu, TTF_Font* font);

// ------------- //
/* MENU RENDU */
// ------------- //
void deplacement_Train(SDL_Renderer* rendu, SDL_Rect* Ptn, TTF_Font* font,int nb_ptn, int& nb_equation, Equation tabEquation[100], Obstacle tabObstacle[100], int& nb_Obstacle);//définis le trajet du train

// ------------- //
/* MENU INSTRUCTION */
// ------------- //

void Instruction(SDL_Renderer* rendu,TTF_Font* font,SDL_Rect RectDroite[]); // lorsqu'on clique sur instruction
void Pave_numerique(TTF_Font* font); // fenetre avec le pavé numerique



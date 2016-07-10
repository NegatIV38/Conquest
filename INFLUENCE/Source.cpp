#include <SFML/Graphics.hpp>
#include <ctime>
#include <Windows.h>

using namespace sf;

#define L 20
#define JOUEURS 2 //1-5
#define MAXCELL 20
#define DELTA 6
#define __BLACK
#define GRID
#define NUMBERS
//#define RANDOM
#define BEST

enum COULEUR{
	BLEU,ROUGE,VERT,JAUNE,RED,VIDE,BLACK
};

enum DIRECTION{
	HAUT,BAS,GAUCHE,DROITE
};

struct t_case{
	std::pair<int, int> coordonees;
	COULEUR col;
	int puissance;
	int maxPuissance;
};

void rendu(RenderWindow* window, Text puis_tab[L][L], t_case tab[L][L]);

void makeGrid(RenderWindow* win){
	RectangleShape H(Vector2f(win->getSize().x, 2));
	RectangleShape V(Vector2f(2,win->getSize().y));
	H.setFillColor(Color::Black);
	V.setFillColor(Color::Black);
	H.setPosition(0, -1);
	V.setPosition(-1, 0);
	for (int i = 0; i < L; i++){
		win->draw(H);
		win->draw(V);
		H.setPosition(H.getPosition().x, H.getPosition().y + win->getSize().y/L);
		V.setPosition(V.getPosition().x + win->getSize().x / L, V.getPosition().y);
	}
}

void initPuisTab(Text tab[L][L], Font& font, RenderWindow* win){
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++){
			tab[i][j].setFont(font);
			tab[i][j].setString("0");
			tab[i][j].setCharacterSize(-L+32);
			tab[i][j].setColor(Color::Black);
			tab[i][j].setPosition((i*win->getSize().x / L) + win->getSize().x / (2 * L), (j*win->getSize().y / L) + win->getSize().y / (2*L));
		}
	}
}

bool isFree(t_case cas){
	return cas.col == COULEUR::VIDE;
}

bool isCaptured(t_case cas, COULEUR c){
	return cas.col == c || cas.col == COULEUR::BLACK;
}

void placeJoueurs(t_case tab[L][L]){
	int x, y;
	for (int i = 0; i < JOUEURS; i++){
		do{
			 x = rand() % L;
			 y = rand() % L;
		} while (!isFree(tab[x][y]));
			tab[x][y].col = COULEUR(i);
			tab[x][y].puissance = 2;
		
		
	}
}

void initTab(t_case tab[L][L]){
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++){
			tab[i][j].col = COULEUR::VIDE;
#ifdef __BLACK
			tab[i][j].col = rand()%16 == 0 ?COULEUR::BLACK : COULEUR::VIDE;
#endif 

			tab[i][j].puissance = 0;
			tab[i][j].coordonees = std::pair<int, int>(i, j);
			tab[i][j].maxPuissance = MAXCELL + rand() % DELTA;
		}
	}
	placeJoueurs(tab);
}



void renduTab(RenderWindow* win, t_case tab[L][L], Text puis_tab[L][L]){
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++){
			puis_tab[i][j].setString(std::to_string(tab[i][j].puissance));
			RectangleShape rect(Vector2f(win->getSize().x / L, win->getSize().y / L));
				rect.setPosition((i*win->getSize().x / L), (j*win->getSize().y / L));
			switch (tab[i][j].col)
			{
				
			case COULEUR::BLEU:
				rect.setFillColor(Color(0,140,255));
				break;
			case COULEUR::JAUNE:
				rect.setFillColor(Color(255, 190, 0));
				break;
			case COULEUR::ROUGE:
				rect.setFillColor(Color(190, 0, 255));
				break;
			case COULEUR::VERT:
				rect.setFillColor(Color(40, 180,0));
				break;
			case COULEUR::RED:
				rect.setFillColor(Color(200, 0, 0));
				break;
			case COULEUR::BLACK:
				rect.setFillColor(Color::Black);
				break;
			case COULEUR::VIDE:
				rect.setFillColor(Color::White);
				break;
			default:
				break;
			}
			win->draw(rect);
		}
	}
}

bool bordure(t_case cas){
	return cas.coordonees.first == 0 || cas.coordonees.second == 0 || cas.coordonees.first == L - 1 || cas.coordonees.second == L - 1;
}

std::vector<std::pair<std::pair<int, int>,std::pair<int,int>>> possibilityCases(COULEUR c, t_case tab[L][L]){
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> vect;
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++){
			if (tab[i][j].col == c && tab[i][j].puissance > 1){
				if (bordure(tab[i][j])){
					if (tab[i][j].coordonees.first == 0){
						if (tab[i][j].coordonees.second == 0){
							if (!isCaptured(tab[i + 1][j],c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i + 1][j].coordonees, tab[i][j].coordonees));
							}
							if (!isCaptured(tab[i][j + 1],c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j + 1].coordonees, tab[i][j].coordonees));
							}
						}
						else if (tab[i][j].coordonees.second == L - 1){
							if (!isCaptured(tab[i + 1][j],c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i + 1][j].coordonees, tab[i][j].coordonees));
							}
							if (!isCaptured(tab[i][j - 1],c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j - 1].coordonees, tab[i][j].coordonees));
							}
						}
						else{
							if (!isCaptured(tab[i + 1][j],c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i + 1][j].coordonees, tab[i][j].coordonees));
							}

							if (!isCaptured(tab[i][j + 1],c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j + 1].coordonees, tab[i][j].coordonees));
							}
							if (!isCaptured(tab[i][j - 1],c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j - 1].coordonees, tab[i][j].coordonees));
							}
						}
					}
					if (tab[i][j].coordonees.second == 0){
						if (tab[i][j].coordonees.first == 0){

							if (!isCaptured(tab[i + 1][j], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i + 1][j].coordonees, tab[i][j].coordonees));
							}
							if (!isCaptured(tab[i][j + 1], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j + 1].coordonees, tab[i][j].coordonees));
							}
						}
						else if (tab[i][j].coordonees.first == L - 1){
							if (!isCaptured(tab[i - 1][j], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i - 1][j].coordonees, tab[i][j].coordonees));
							}
							if (!isCaptured(tab[i][j + 1], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j + 1].coordonees, tab[i][j].coordonees));
							}
						}
						else{
							if (!isCaptured(tab[i + 1][j], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i + 1][j].coordonees, tab[i][j].coordonees));
							}

							if (!isCaptured(tab[i][j + 1], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j + 1].coordonees, tab[i][j].coordonees));
							}
							if (!isCaptured(tab[i- 1][j ], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i - 1][j].coordonees, tab[i][j].coordonees));
							}
						}
					}
					if (tab[i][j].coordonees.first == L-1){
						if (tab[i][j].coordonees.second == 0){
							if (!isCaptured(tab[i - 1][j], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i - 1][j].coordonees, tab[i][j].coordonees));
							}
							if (!isCaptured(tab[i][j + 1], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j + 1].coordonees, tab[i][j].coordonees));
							}
						}
						else if (tab[i][j].coordonees.second == L - 1){
							if (!isCaptured(tab[i - 1][j], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i - 1][j].coordonees, tab[i][j].coordonees));
							}
							if (!isCaptured(tab[i][j - 1], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j - 1].coordonees, tab[i][j].coordonees));
							}
						}
						else{
							if (!isCaptured(tab[i - 1][j], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i - 1][j].coordonees, tab[i][j].coordonees));
							}

							if (!isCaptured(tab[i][j + 1], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j + 1].coordonees, tab[i][j].coordonees));
							}
							if (!isCaptured(tab[i][j - 1], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j - 1].coordonees, tab[i][j].coordonees));
							}
						}
					}
					if (tab[i][j].coordonees.second == L-1){
						if (tab[i][j].coordonees.first == 0){
							if (!isCaptured(tab[i + 1][j], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i + 1][j].coordonees, tab[i][j].coordonees));
							}
							if (!isCaptured(tab[i][j - 1], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j - 1].coordonees, tab[i][j].coordonees));
							}
						}
						else if (tab[i][j].coordonees.second == L - 1){
							if (!isCaptured(tab[i - 1][j], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i - 1][j].coordonees, tab[i][j].coordonees));
							}
							if (!isCaptured(tab[i][j - 1], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j - 1].coordonees, tab[i][j].coordonees));
							}
						}
						else{
							if (!isCaptured(tab[i + 1][j], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i + 1][j].coordonees, tab[i][j].coordonees));
							}

							if (!isCaptured(tab[i - 1][j ], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i - 1][j].coordonees, tab[i][j].coordonees));
							}
							if (!isCaptured(tab[i][j - 1], c)){
								vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j - 1].coordonees, tab[i][j].coordonees));
							}
						}
					}
					
				}
				else{
					if (!isCaptured(tab[i - 1][j],c)){
						vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i - 1][j].coordonees, tab[i][j].coordonees));
					}
					if (!isCaptured(tab[i + 1][j],c)){
						vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i + 1][j].coordonees, tab[i][j].coordonees));
					}
					if (!isCaptured(tab[i][j - 1],c)){
						vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j - 1].coordonees, tab[i][j].coordonees));
					}
					if (!isCaptured(tab[i][j + 1],c)){
						vect.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(tab[i][j + 1].coordonees, tab[i][j].coordonees));
					}
				}
			}
		}
	}

	/*for (int x = 0; x < vect.size(); x++){
		for (std::vector<std::pair<int, int>>::iterator it = vect.begin() + x + 1; it != vect.end(); it++){
			if (vect.at(x) == *it){
				vect.erase(it);
			}
		}
	}*/

	return vect;
}

void filtrePoss(std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>* poss, t_case tab[L][L]){
	for (int i = 0; i < poss->size(); i++){
		if (tab[poss->at(i).first.first][poss->at(i).first.second].puissance > tab[poss->at(i).second.first][poss->at(i).second.second].puissance || tab[poss->at(i).second.first][poss->at(i).second.second].puissance < 2){
			poss->erase(poss->begin() + i);
		}
	}

}

int findBest(std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& poss, t_case tab[L][L]){
	int u = 0;
	for (int i = 0; i < poss.size(); i++){
		if (tab[poss.at(i).first.first][poss.at(i).first.second].puissance < tab[poss.at(u).first.first][poss.at(u).first.second].puissance){
			u = i;
		}
	}
	std::vector<int> list;
	for (int i = 0; i < poss.size(); i++){
		if (tab[poss.at(i).first.first][poss.at(i).first.second].puissance == tab[poss.at(u).first.first][poss.at(u).first.second].puissance){
			list.push_back(i);
		}
	}
	return list.at(rand()%list.size());
}

bool randMove(COULEUR c, t_case tab[L][L]){
	auto poss = possibilityCases(c, tab);
	filtrePoss(&poss, tab);
	if (poss.size() > 0){
#ifdef RANDOM
		int r = rand() % poss.size();
#endif
#ifdef BEST
		int r = findBest(poss,tab);
#endif
		if (tab[poss.at(r).first.first][poss.at(r).first.second].col == COULEUR::VIDE){
			tab[poss.at(r).first.first][poss.at(r).first.second].puissance = tab[poss.at(r).second.first][poss.at(r).second.second].puissance - 1;
			tab[poss.at(r).first.first][poss.at(r).first.second].col = tab[poss.at(r).second.first][poss.at(r).second.second].col;
			tab[poss.at(r).second.first][poss.at(r).second.second].puissance = 1;
		}
		else{
			if (tab[poss.at(r).first.first][poss.at(r).first.second].puissance == tab[poss.at(r).second.first][poss.at(r).second.second].puissance){
				tab[poss.at(r).first.first][poss.at(r).first.second].puissance = 1;
				tab[poss.at(r).second.first][poss.at(r).second.second].puissance = 1;
			}
			else if (tab[poss.at(r).first.first][poss.at(r).first.second].puissance > tab[poss.at(r).second.first][poss.at(r).second.second].puissance){
				tab[poss.at(r).first.first][poss.at(r).first.second].puissance -= tab[poss.at(r).second.first][poss.at(r).second.second].puissance;
				if (tab[poss.at(r).first.first][poss.at(r).first.second].puissance < 2 && rand() % 2){
					tab[poss.at(r).first.first][poss.at(r).first.second].col = c;
				}
				tab[poss.at(r).second.first][poss.at(r).second.second].puissance = 1;
			}
			else{
				tab[poss.at(r).first.first][poss.at(r).first.second].puissance -= tab[poss.at(r).second.first][poss.at(r).second.second].puissance;
				tab[poss.at(r).first.first][poss.at(r).first.second].puissance *= -1;
				tab[poss.at(r).first.first][poss.at(r).first.second].col = c;
				tab[poss.at(r).second.first][poss.at(r).second.second].puissance = 1;
			}
			
		}
		return true;
	}
	return false;
}

bool cellSupOne(COULEUR c, t_case tab[L][L]){
	for (int i = 0; i < L; i++){
		for (int j = 0; j < L; j++){
			if (tab[i][j].col == c){
				if (tab[i][j].puissance > 1){
					return true;
				}
			}
		}
	}
	return false;
}

int sumPuiss(COULEUR c, t_case tab[L][L]){
	int sum = 0;
	for (int i = 0; i < L; i++){
		for (int j = 0; j < L; j++){
			if (tab[i][j].col == c){
				sum += tab[i][j].puissance;
			}
		}
	}
	return sum;
}

std::vector<std::pair<int, int>> getCases(COULEUR c, t_case tab[L][L]){
	std::vector<std::pair<int, int>> sum;
	for (int i = 0; i < L; i++){
		for (int j = 0; j < L; j++){
			if (tab[i][j].col == c){
				if (tab[i][j].puissance < tab[i][j].maxPuissance){
					sum.push_back(tab[i][j].coordonees);
				}
			}
		}
	}
	return sum;
}

int findBestOwn(std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& poss, t_case tab[L][L]){
	int u = 0;
	for (int i = 0; i < poss.size(); i++){
		if (tab[poss.at(i).second.first][poss.at(i).second.second].puissance < tab[poss.at(u).second.first][poss.at(u).second.second].puissance){
			u = i;
		}
	}
	std::vector<int> list;
	for (int i = 0; i < poss.size(); i++){
		if (tab[poss.at(i).second.first][poss.at(i).second.second].puissance == tab[poss.at(u).second.first][poss.at(u).second.second].puissance){
			list.push_back(i);
		}
	}
	return list.at(rand() % list.size());
}

bool frontLineFullPts(COULEUR c, t_case tab[L][L]){
	auto poss = possibilityCases(c, tab);
	for (int i = 0; i < poss.size(); i++){
		if (tab[poss.at(i).second.first][poss.at(i).second.second].puissance != tab[poss.at(i).second.first][poss.at(i).second.second].maxPuissance){
			return false;
		}
	}
	return true;
}

bool attributionPts(COULEUR c, t_case tab[L][L]){
	if (frontLineFullPts(c, tab)){
		std::vector<std::pair<int, int>> ownList = getCases(c, tab);
		if (ownList.size() > 0){
			int r = rand() % ownList.size();
			if (tab[ownList.at(r).first][ownList.at(r).second].puissance < tab[ownList.at(r).first][ownList.at(r).second].maxPuissance){
				tab[ownList.at(r).first][ownList.at(r).second].puissance++;
				return true;
			}
		}
		return false;
	}
	else {
		auto poss = possibilityCases(c, tab);
//#ifdef RANDOM
		int r = rand() % poss.size();
//#endif
//#ifdef BEST
	//	int r = findBestOwn(poss, tab);
//#endif
		if (tab[poss.at(r).second.first][poss.at(r).second.second].puissance < tab[poss.at(r).second.first][poss.at(r).second.second].maxPuissance){
			tab[poss.at(r).second.first][poss.at(r).second.second].puissance++;
			return true;
		}
		return false;
	}
}

void moves(COULEUR c, t_case tab[L][L], RenderWindow* window, Text puis_tab[L][L]){
	auto poss = possibilityCases(c, tab);
	filtrePoss(&poss, tab);
	bool b = true;
	while (cellSupOne(c, tab) && poss.size() > 0 && b)
	{
		b = randMove(c, tab);
		rendu(window, puis_tab, tab);
		Sleep(10);
	}
	int pts = sumPuiss(c, tab);
	while (pts != 0)
	{
		if (attributionPts(c, tab)){ 
			pts--;
		}
		else{
			if (getCases(c, tab).size() == 0){
				pts = 0;
			}
		}
	}
}

void rendu(RenderWindow* window, Text puis_tab[L][L], t_case tab[L][L]){
	window->clear(Color::White);
	renduTab(window, tab, puis_tab);
#ifdef GRID
	makeGrid(window);
#endif
#ifdef NUMBERS
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++){
			window->draw(puis_tab[i][j]);
		}
	}
#endif
	window->display();
}

int main()
{
	srand(time(NULL));
	RenderWindow window(VideoMode(600, 600), "SFML works!");
	Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		
	}
	Text puis_tab[L][L];
	initPuisTab(puis_tab, font,&window);
	t_case tab[L][L];
	initTab(tab);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		rendu(&window, puis_tab, tab);

		for (int i = 0; i < JOUEURS; i++){
			//randMove(COULEUR(i), tab);
			moves(COULEUR(i), tab,&window,puis_tab);
		}

		
	}

	return 0;
}
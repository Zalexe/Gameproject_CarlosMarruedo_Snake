

#include <stdlib.h> 
#include <math.h>
#include <stdio.h> 
#include "GUI.h"
#include "Player.h"
#include "System.h"
#include "Logger.h"
#include "EasyScene.h"
#include "DifficultyScene.h"
#include "Snake.h"
#include "food.h"
#include "GameOverScene.h"
#include <vector>
#include <XML/rapidxml_utils.hpp>



using namespace Logger;



snake snak; //inicializar la serpiente
Player player;//inicializar el jugador
food apple;//inicializar la comida

EasyScene::EasyScene(void) {
	
	
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BG_00 }; //inicializar el background
	
	s_head = { {0,0,30,30},ObjectID::SNAKE_HEAD_RIGHT }; //inicializar sprites cabeza al empezar
	s_body = { { 0,0,30,30 },ObjectID::SNAKE_BODY };//inicializar sprites cuerpo al empezar
	s_tail = { { 0,0,30,30 },ObjectID::SNAKE_TAIL };//inicializar sprites cola al empezar
	s_food = { { apple.position_x,apple.position_y,30,30 },ObjectID::FOOD};//inicializar sprites cabeza al empezar
	m_empty = { {0,0,30,30},ObjectID::CELL_EMPTY };
	Wall1 = { {20,650,950,50 },ObjectID::WALL };//abajo inclinado
	Wall2 = { { 20,50,950,50 },ObjectID::WALL };//arriba inclinado
	Wall3 = { { 950,50,50,650 },ObjectID::WALL };//derecha recto
	Wall4 = { { 20,50,50,600 },ObjectID::WALL };//izquierda recto
	timer = { { 20,700,tiempo,20 },ObjectID::TIMER };//barra de tiempo
	nivel = 1;
	alimentosPorObt = 3 + 1 * nivel;
}

EasyScene::~EasyScene(void) {
}

void EasyScene::OnEntry(void) {
	snak.lifes = 3; //inicializar vidas de la vida
	snak.alimentosObt = 0;
	snak.init_list(); //inicializar la lista de las posiciones del cuerpo de la serpiente
	apple.FoodSpawn();
	sizeChekpoint = 3;
	while (snak.segments.size() > sizeChekpoint) {
		snak.segments.pop_back();
	}

	while ((apple.position_x == snak.head.x || (apple.position_x < snak.head.x+15 && apple.position_x > snak.head.x-15))
		|| (apple.position_y == snak.head.y || (apple.position_y < snak.head.y + 15 && apple.position_y > snak.head.y - 15))) {
		apple.FoodSpawn();
	}
	TM.n = 8;//para controlar el tiempo y a su vez la velocidad de la serpiente
	tiempomax = 1000;
	tiempo = tiempomax;
}

void EasyScene::OnExit(void) {
}

void EasyScene::Update(void) {
		
		snak.getKey();//detecta los controles
		snak.v =30+(player.score/1000);//inicia la velocidad
		snak.move();//activa la funcion de movimiento
		
		snake::COORD dead; //inicializa las cordenadas de una variable auxiliar para la muerte de la serpiente
		dead.x = 590; dead.y = 390; //  ^
		
		if ((950 > snak.head.x && 20<snak.head.x) && (120>snak.head.y && 30 < snak.head.y)) {//collison pared arriba
			
			snak.head.x=dead.x;//cambia las cordenadas de la cabeza al morir
			snak.head.y = dead.y;
			snak.lifes--; //resta el numero de vidas restantes
			while (snak.segments.size() > sizeChekpoint) {
				snak.segments.pop_back();
			}
		}
		if ((50 > snak.head.x && 0<snak.head.x) && (650>snak.head.y && 50 < snak.head.y)) {//colision pared izquierda
			snak.lifes--;
			snak.head.x = dead.x;
			snak.head.y = dead.y;
			while (snak.segments.size() > sizeChekpoint) {
				snak.segments.pop_back();
			}
		}
		if ((970 > snak.head.x && 920<snak.head.x) && (650>snak.head.y && 50 < snak.head.y)) {//collision pared derecha
			snak.lifes--;
			snak.head.x = dead.x;
			snak.head.y = dead.y;
			while (snak.segments.size() > sizeChekpoint) {
				snak.segments.pop_back();
			}
		}
		if ((950 > snak.head.x && 20<snak.head.x) && (700>snak.head.y && 650 < snak.head.y)) {//collision pared abajo
			snak.lifes--;
			snak.head.x = dead.x;
			snak.head.y = dead.y;
			while (snak.segments.size() > sizeChekpoint) {
				snak.segments.pop_back();
			}
		}
		if (snak.Collision(dead)) {
			snak.lifes--;
			snak.head.x = dead.x;
			snak.head.y = dead.y;
			while (snak.segments.size() > sizeChekpoint) {
				snak.segments.pop_back();
			}
		}

		player.score++;//incrementa la puntuacion del jugador constantemente
		


		if (snak.lifes == 0) { //detecta si el jugador ya ha perdido
			SM.SetCurScene <GameOverScene>(); //acaba el juego 
		}

		if (apple.isCollision(snak.head)) {
			snak.alimentosObt++;
			snak.segments.push_front(snak.head);
			apple.FoodSpawn();
			while ((apple.position_x == snak.head.x || (apple.position_x < snak.head.x + 5 && apple.position_x > snak.head.x - 5))
				&& (apple.position_y == snak.head.y || (apple.position_y < snak.head.y + 5 && apple.position_y > snak.head.y - 5))) {
				apple.FoodSpawn();
			}

		}
		if (snak.alimentosObt == alimentosPorObt) {
			GUI::DrawTextShaded<FontID::ARIAL>("Has completado el nivel!",
			{ W.GetWidth() >> 2, int(W.GetHeight()*.4f), 4, 3 },
			{ 190, 150, 150 }, { 0, 0, 0 });
			
			sizeChekpoint = snak.segments.size();
			

			nivel++;//aumenta el nivel
			snak.lifes = 3;//restaura las vidas
			player.score += 600;
			snak.alimentosObt=0;//restaura el valor para el nuevo nivel
			alimentosPorObt = 3 + 1 * nivel;//aumenta la lista de alimentos por obtener
			printf("Nivel %d\n", nivel);
			tiempo = tiempomax;
		}

		if (tiempo <= 20) {//comprobacion del tiempo
			snak.lifes = 0;
		}
		tiempo--;//pasas el tiempo
		score = player.score; //le pasas el score al static score de las escenas de ranking
		


		timer = { { 20,700,tiempo,20 },ObjectID::TIMER };//barra de tiempo
}


void EasyScene::Draw(void) {
	m_background.Draw(); // Render background
	snake::COORD pos; //inicializa cordenadas 
	pos = snak.segments.front(); //se guarda la primera posicion de la lista


	if (snak.dir == 1) { //comprueba la direccion
		s_head = { { snak.getint(pos.x),snak.getint(pos.y),30,30 },ObjectID::SNAKE_HEAD_DOWN }; //pinta en la posicion del primer elemento de la lista y cambia el sprite dependiendo de su direccion



	}
	else if (snak.dir == 0) {
		s_head = { { snak.getint(pos.x),snak.getint(pos.y), 30, 30 },ObjectID::SNAKE_HEAD_RIGHT };

	}
	else if (snak.dir == 2) {
		s_head = { { snak.getint(pos.x),snak.getint(pos.y),30,30 },ObjectID::SNAKE_HEAD_LEFT };

	}
	else if (snak.dir == 3) {
		s_head = { { snak.getint(pos.x),snak.getint(pos.y),30,30 },ObjectID::SNAKE_HEAD };

	}
	s_head.Draw(); //pinta la cabeza
	list<snake::COORD>::iterator posi;  //inicializa otro iterador
	posi = snak.segments.begin(); //guarda el iterador de la primera posicion de la lista
	posi++; //incrementas al siguiente iterador de la lista

	while (posi != --snak.segments.end()) { //mientras el iterador no apunte a la ultima posicion

		snak.aux2 = *posi; //guardamos la posicion en una variable auxiliar



		if (snak.dir == 1) {
			s_body = { { snak.getint(snak.aux2.x),snak.getint(snak.aux2.y),30,30 },ObjectID::SNAKE_BODY_DOWN }; //pintamos el sprite dependiendo de la direccion y la posicion de ese segmento

		}
		else if (snak.dir == 0) {
			s_body = { { snak.getint(snak.aux2.x),snak.getint(snak.aux2.y), 30, 30 },ObjectID::SNAKE_BODY_RIGHT };


		}
		else if (snak.dir == 2) {
			s_body = { { snak.getint(snak.aux2.x),snak.getint(snak.aux2.y),30,30 },ObjectID::SNAKE_BODY_LEFT };

		}
		else if (snak.dir == 3) {
			s_body = { { snak.getint(snak.aux2.x),snak.getint(snak.aux2.y),30,30 },ObjectID::SNAKE_BODY };
		}

		posi++;//pasamos al siguiente iterador de la lista
		s_body.Draw();//pintamos este segmento del cuerpo por cada iteracion
	}

	snak.aux2 = snak.segments.back(); //al acabar de recorrrer el cuerpo recogemos la ultima posicion de la lista



	if (snak.dir == 1) { //pintamos la cola dependiendo de su direccion en la coordenada final de la lista
		s_tail = { { snak.getint(snak.aux2.x),snak.getint(snak.aux2.y),30,30 },ObjectID::SNAKE_TAIL_DOWN };

	}
	else if (snak.dir == 0) {
		s_tail = { { snak.getint(snak.aux2.x),snak.getint(snak.aux2.y), 30, 30 },ObjectID::SNAKE_TAIL_RIGHT };

	}
	else if (snak.dir == 2) {
		s_tail = { { snak.getint(snak.aux2.x),snak.getint(snak.aux2.y),30,30 },ObjectID::SNAKE_TAIL_LEFT };

	}
	else if (snak.dir == 3) {
		s_tail = { { snak.getint(snak.aux2.x),snak.getint(snak.aux2.y),30,30 },ObjectID::SNAKE_TAIL };
	}
	s_food = { { apple.position_x,apple.position_y,30,30 },ObjectID::FOOD };




	s_tail.Draw(); //pintamos la cola
	s_food.Draw(); //pintamos la comida
	Wall1.Draw(); //pintamos las paredes
	Wall2.Draw();
	Wall3.Draw();
	Wall4.Draw();
	timer.Draw();



}

#include "GUI.h"
#include "Player.h"
#include "System.h"
#include "Logger.h"
#include "NormalScene.h"
#include "DifficultyScene.h"
#include "Snake.h"
#include "food.h"
#include "GameOverScene.h"

using namespace Logger;



snake snaki; //inicializar la serpiente
Player playerd;//inicializar el jugador
food appled;//inicializar la comida


NormalScene::NormalScene(void) {


	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BG_00 }; //inicializar el background

	s_head = { { 0,0,30,30 },ObjectID::SNAKE_HEAD_RIGHT }; //inicializar sprites cabeza al empezar
	s_body = { { 0,0,30,30 },ObjectID::SNAKE_BODY };//inicializar sprites cuerpo al empezar
	s_tail = { { 0,0,30,30 },ObjectID::SNAKE_TAIL };//inicializar sprites cola al empezar
	s_food = { { appled.position_x,appled.position_y,30,30 },ObjectID::FOOD };//inicializar sprites cabeza al empezar
	m_empty = { { 0,0,30,30 },ObjectID::CELL_EMPTY };
	Wall1 = { { 20,650,950,50 },ObjectID::WALL };//abajo inclinado
	Wall2 = { { 20,50,950,50 },ObjectID::WALL };//arriba inclinado
	Wall3 = { { 950,50,50,650 },ObjectID::WALL };//derecha recto
	Wall4 = { { 20,50,50,600 },ObjectID::WALL };//izquierda recto
	timer = { { 20,700,tiempo,20 },ObjectID::TIMER };//barra de tiempo
	nivel = 1;
	alimentosPorObt = 5 + 1 * nivel;
}

NormalScene::~NormalScene(void) {
}

void NormalScene::OnEntry(void) {
	snaki.lifes = 3; //inicializar vidas de la vida
	snaki.alimentosObt = 0;
	snaki.init_list(); //inicializar la lista de las posiciones del cuerpo de la serpiente
	appled.FoodSpawn();
	sizeChekpoint = 3;
	while (snaki.segments.size() > sizeChekpoint) {
		snaki.segments.pop_back();
	}

	while ((appled.position_x == snaki.head.x || (appled.position_x < snaki.head.x + 15 && appled.position_x > snaki.head.x - 15))
		|| (appled.position_y == snaki.head.y || (appled.position_y < snaki.head.y + 15 && appled.position_y > snaki.head.y - 15))) {
		appled.FoodSpawn();
	}
	TM.n = 6;//para controlar el tiempo y a su vez la velocidad de la serpiente
	tiempomax = 1350;
	tiempo = tiempomax;
}

void NormalScene::OnExit(void) {
}

void NormalScene::Update(void) {

	snaki.getKey();//detecta los controles
	snaki.v = 30 + (playerd.score / 1000);//inicia la velocidad
	snaki.move();//activa la funcion de movimiento

	snake::COORD dead; //inicializa las cordenadas de una variable auxiliar para la muerte de la serpiente
	dead.x = 590; dead.y = 390; //  ^

	if ((950 > snaki.head.x && 20<snaki.head.x) && (120>snaki.head.y && 30 < snaki.head.y)) {//collison pared arriba

		snaki.head.x = dead.x;//cambia las cordenadas de la cabeza al morir
		snaki.head.y = dead.y;
		snaki.lifes--; //resta el numero de vidas restantes
		while (snaki.segments.size() > sizeChekpoint) {
			snaki.segments.pop_back();
		}
	}
	if ((50 > snaki.head.x && 0<snaki.head.x) && (650>snaki.head.y && 50 < snaki.head.y)) {//colision pared izquierda
		snaki.lifes--;
		snaki.head.x = dead.x;
		snaki.head.y = dead.y;
		while (snaki.segments.size() > sizeChekpoint) {
			snaki.segments.pop_back();
		}

	}
	if ((970 > snaki.head.x && 920<snaki.head.x) && (650>snaki.head.y && 50 < snaki.head.y)) {//collision pared derecha
		snaki.lifes--;
		snaki.head.x = dead.x;
		snaki.head.y = dead.y;
		while (snaki.segments.size() > sizeChekpoint) {
			snaki.segments.pop_back();
		}
	}
	if ((950 > snaki.head.x && 20<snaki.head.x) && (700>snaki.head.y && 650 < snaki.head.y)) {//collision pared abajo
		snaki.lifes--;
		snaki.head.x = dead.x;
		snaki.head.y = dead.y;
		while (snaki.segments.size() > sizeChekpoint) {
			snaki.segments.pop_back();
		}
	}
	if (snaki.Collision(dead)) {
		snaki.lifes--;
		snaki.head.x = dead.x;
		snaki.head.y = dead.y;
		while (snaki.segments.size() > sizeChekpoint) {
			snaki.segments.pop_back();
		}
	}

	playerd.score++;//incrementa la puntuacion del jugador constantemente



	if (snaki.lifes == 0) { //detecta si el jugador ya ha perdido
		SM.SetCurScene <GameOverScene>(); //acaba el juego 
	}

	if (appled.isCollision(snaki.head)) {
		snaki.alimentosObt++;
		snaki.segments.push_front(snaki.head);
		appled.FoodSpawn();
		while ((appled.position_x == snaki.head.x || (appled.position_x < snaki.head.x + 5 && appled.position_x > snaki.head.x - 5))
			&& (appled.position_y == snaki.head.y || (appled.position_y < snaki.head.y + 5 && appled.position_y > snaki.head.y - 5))) {
			appled.FoodSpawn();
		}
	}
	if (snaki.alimentosObt == alimentosPorObt) {
		GUI::DrawTextShaded<FontID::ARIAL>("Has completado el nivel!",
		{ W.GetWidth() >> 2, int(W.GetHeight()*.4f), 4, 3 },
		{ 190, 150, 150 }, { 0, 0, 0 });
		
		sizeChekpoint = snaki.segments.size();


		nivel++;//aumenta el nivel
		snaki.lifes = 3;//restaura las vidas
		playerd.score += 1000;//suma el score
		snaki.alimentosObt = 0;//restaura el valor para el nuevo nivel
		alimentosPorObt = 3 + 2 * nivel;//aumenta la lista de alimentos por obtener
		printf("Nivel %d\n", nivel);
		tiempo += 150;
	}

	if (tiempo <= 20) {
		snaki.lifes = 0;
	}
	tiempo--;//pasas el tiempo
	score = playerd.score;//le pasas el score al static score de las escenas de ranking


	timer = { { 20,700,tiempo/2,20 },ObjectID::TIMER };//barra de tiempo
}


void NormalScene::Draw(void) {
	m_background.Draw(); // Render background
	snake::COORD pos; //inicializa cordenadas 
	pos = snaki.segments.front(); //se guarda la primera posicion de la lista


	if (snaki.dir == 1) { //comprueba la direccion
		s_head = { { snaki.getint(pos.x),snaki.getint(pos.y),30,30 },ObjectID::SNAKE_HEAD_DOWN }; //pinta en la posicion del primer elemento de la lista y cambia el sprite dependiendo de su direccion



	}
	else if (snaki.dir == 0) {
		s_head = { { snaki.getint(pos.x),snaki.getint(pos.y), 30, 30 },ObjectID::SNAKE_HEAD_RIGHT };

	}
	else if (snaki.dir == 2) {
		s_head = { { snaki.getint(pos.x),snaki.getint(pos.y),30,30 },ObjectID::SNAKE_HEAD_LEFT };

	}
	else if (snaki.dir == 3) {
		s_head = { { snaki.getint(pos.x),snaki.getint(pos.y),30,30 },ObjectID::SNAKE_HEAD };

	}
	s_head.Draw(); //pinta la cabeza
	list<snake::COORD>::iterator posi;  //inicializa otro iterador
	posi = snaki.segments.begin(); //guarda el iterador de la primera posicion de la lista
	posi++; //incrementas al siguiente iterador de la lista

	while (posi != --snaki.segments.end()) { //mientras el iterador no apunte a la ultima posicion

		snaki.aux2 = *posi; //guardamos la posicion en una variable auxiliar



		if (snaki.dir == 1) {
			s_body = { { snaki.getint(snaki.aux2.x),snaki.getint(snaki.aux2.y),30,30 },ObjectID::SNAKE_BODY_DOWN }; //pintamos el sprite dependiendo de la direccion y la posicion de ese segmento

		}
		else if (snaki.dir == 0) {
			s_body = { { snaki.getint(snaki.aux2.x),snaki.getint(snaki.aux2.y), 30, 30 },ObjectID::SNAKE_BODY_RIGHT };


		}
		else if (snaki.dir == 2) {
			s_body = { { snaki.getint(snaki.aux2.x),snaki.getint(snaki.aux2.y),30,30 },ObjectID::SNAKE_BODY_LEFT };

		}
		else if (snaki.dir == 3) {
			s_body = { { snaki.getint(snaki.aux2.x),snaki.getint(snaki.aux2.y),30,30 },ObjectID::SNAKE_BODY };
		}

		posi++;//pasamos al siguiente iterador de la lista
		s_body.Draw();//pintamos este segmento del cuerpo por cada iteracion
	}

	snaki.aux2 = snaki.segments.back(); //al acabar de recorrrer el cuerpo recogemos la ultima posicion de la lista



	if (snaki.dir == 1) { //pintamos la cola dependiendo de su direccion en la coordenada final de la lista
		s_tail = { { snaki.getint(snaki.aux2.x),snaki.getint(snaki.aux2.y),30,30 },ObjectID::SNAKE_TAIL_DOWN };

	}
	else if (snaki.dir == 0) {
		s_tail = { { snaki.getint(snaki.aux2.x),snaki.getint(snaki.aux2.y), 30, 30 },ObjectID::SNAKE_TAIL_RIGHT };

	}
	else if (snaki.dir == 2) {
		s_tail = { { snaki.getint(snaki.aux2.x),snaki.getint(snaki.aux2.y),30,30 },ObjectID::SNAKE_TAIL_LEFT };

	}
	else if (snaki.dir == 3) {
		s_tail = { { snaki.getint(snaki.aux2.x),snaki.getint(snaki.aux2.y),30,30 },ObjectID::SNAKE_TAIL };
	}
	s_food = { { appled.position_x,appled.position_y,30,30 },ObjectID::FOOD };




	s_tail.Draw(); //pintamos la cola
	s_food.Draw(); //pintamos la comida
	Wall1.Draw(); //pintamos las paredes
	Wall2.Draw();
	Wall3.Draw();
	Wall4.Draw();
	timer.Draw();
}
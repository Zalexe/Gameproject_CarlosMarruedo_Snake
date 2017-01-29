
#include "GUI.h"
#include "Player.h"
#include "System.h"
#include "Logger.h"
#include "HardScene.h"
#include "DifficultyScene.h"
#include "Snake.h"
#include "food.h"
#include "GameOverScene.h"

using namespace Logger;



snake snaku; //inicializar la serpiente
Player players;//inicializar el jugador
food apples;//inicializar la comida


HardScene::HardScene(void) {


	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BG_00 }; //inicializar el background

	s_head = { { 0,0,30,30 },ObjectID::SNAKE_HEAD_RIGHT }; //inicializar sprites cabeza al empezar
	s_body = { { 0,0,30,30 },ObjectID::SNAKE_BODY };//inicializar sprites cuerpo al empezar
	s_tail = { { 0,0,30,30 },ObjectID::SNAKE_TAIL };//inicializar sprites cola al empezar
	s_food = { { apples.position_x,apples.position_y,30,30 },ObjectID::FOOD };//inicializar sprites cabeza al empezar
	m_empty = { { 0,0,30,30 },ObjectID::CELL_EMPTY };
	Wall1 = { { 20,650,950,50 },ObjectID::WALL };//abajo inclinado
	Wall2 = { { 20,50,950,50 },ObjectID::WALL };//arriba inclinado
	Wall3 = { { 950,50,50,650 },ObjectID::WALL };//derecha recto
	Wall4 = { { 20,50,50,600 },ObjectID::WALL };//izquierda recto
	timer = { { 20,700,tiempo,20 },ObjectID::TIMER };//barra de tiempo
	nivel = 1;
	alimentosPorObt = 6 + 1 * nivel;
	
}

HardScene::~HardScene(void) {
}

void HardScene::OnEntry(void) {
	snaku.lifes = 3; //inicializar vidas de la vida
	snaku.alimentosObt = 0;
	snaku.init_list(); //inicializar la lista de las posiciones del cuerpo de la serpiente
	apples.FoodSpawn();
	
	sizeChekpoint = 3;
	while (snaku.segments.size() > sizeChekpoint) {
		snaku.segments.pop_back();
	}

	while ((apples.position_x == snaku.head.x || (apples.position_x < snaku.head.x + 15 && apples.position_x > snaku.head.x - 15))
		|| (apples.position_y == snaku.head.y || (apples.position_y < snaku.head.y + 15 && apples.position_y > snaku.head.y - 15))) {
		apples.FoodSpawn();
	}
	TM.n = 4;//para controlar el tiempo y a su vez la velocidad de la serpiente
	tiempomax = 1900;
	tiempo = tiempomax;
	
}

void HardScene::OnExit(void) {
}

void HardScene::Update(void) {

	snaku.getKey();//detecta los controles
	snaku.v = 30 + (players.score / 1000);//inicia la velocidad
	snaku.move();//activa la funcion de movimiento

	snake::COORD dead; //inicializa las cordenadas de una variable auxiliar para la muerte de la serpiente
	dead.x = 590; dead.y = 390; //  ^

	if ((950 > snaku.head.x && 20<snaku.head.x) && (120>snaku.head.y && 30 < snaku.head.y)) {//collison pared arriba

		snaku.head.x = dead.x;//cambia las cordenadas de la cabeza al morir
		snaku.head.y = dead.y;
		snaku.lifes--; //resta el numero de vidas restantes
		while (snaku.segments.size() > sizeChekpoint) {
			snaku.segments.pop_back();
		}
	}
	if ((50 > snaku.head.x && 0<snaku.head.x) && (650>snaku.head.y && 50 < snaku.head.y)) {//colision pared izquierda
		snaku.lifes--;
		snaku.head.x = dead.x;
		snaku.head.y = dead.y;
		while (snaku.segments.size() > sizeChekpoint) {
			snaku.segments.pop_back();
		}
	}
	if ((970 > snaku.head.x && 920<snaku.head.x) && (650>snaku.head.y && 50 < snaku.head.y)) {//collision pared derecha
		snaku.lifes--;
		snaku.head.x = dead.x;
		snaku.head.y = dead.y;
		while (snaku.segments.size() > sizeChekpoint) {
			snaku.segments.pop_back();
		}
	}
	if ((950 > snaku.head.x && 20<snaku.head.x) && (700>snaku.head.y && 650 < snaku.head.y)) {//collision pared abajo
		snaku.lifes--;
		snaku.head.x = dead.x;
		snaku.head.y = dead.y;
		while (snaku.segments.size() > sizeChekpoint) {
			snaku.segments.pop_back();
		}
	}
	if (snaku.Collision(dead)) {
		snaku.lifes--;
		snaku.head.x = dead.x;
		snaku.head.y = dead.y;
		while (snaku.segments.size() > sizeChekpoint) {
			snaku.segments.pop_back();
		}
	}

	players.score++;//incrementa la puntuacion del jugador constantemente



	if (snaku.lifes == 0) { //detecta si el jugador ya ha perdido
		SM.SetCurScene <GameOverScene>(); //acaba el juego 
	}

	if (apples.isCollision(snaku.head)) {//collision con el cuerpo
		snaku.alimentosObt++;
		snaku.segments.push_front(snaku.head);
		apples.FoodSpawn();
		while ((apples.position_x == snaku.head.x || (apples.position_x < snaku.head.x + 5 && apples.position_x > snaku.head.x - 5))
			&& (apples.position_y == snaku.head.y || (apples.position_y < snaku.head.y + 5 && apples.position_y > snaku.head.y - 5))) {
			apples.FoodSpawn();
		}
	}
	if (snaku.alimentosObt == alimentosPorObt) {//cuando completa el nivel
		GUI::DrawTextShaded<FontID::ARIAL>("Has completado el nivel!",
		{ W.GetWidth() >> 2, int(W.GetHeight()*.4f), 4, 3 },
		{ 190, 150, 150 }, { 0, 0, 0 });
		
		sizeChekpoint = snaku.segments.size();//check point

		nivel++;//aumenta el nivel
		snaku.lifes = 3;//restaura las vidas
		players.score += 1400;//suma el score
		snaku.alimentosObt = 0;//restaura el valor para el nuevo nivel
		alimentosPorObt = 3 + 3 * nivel;//aumenta la lista de alimentos por obtener
		printf("Nivel %d\n", nivel);
		tiempo +=100;//reinicia el tiempo
	}
	if (tiempo<= 20) {
		snaku.lifes = 0;
	}
	tiempo--;//pasas el tiempo
	score = players.score; //le pasas el score al static score de las escenas de ranking

	timer = { { 20,700,tiempo/2,20 },ObjectID::TIMER };//barra de tiempo
}


void HardScene::Draw(void) {
	m_background.Draw(); // Render background
	snake::COORD pos; //inicializa cordenadas 
	pos = snaku.segments.front(); //se guarda la primera posicion de la lista


	if (snaku.dir == 1) { //comprueba la direccion
		s_head = { { snaku.getint(pos.x),snaku.getint(pos.y),30,30 },ObjectID::SNAKE_HEAD_DOWN }; //pinta en la posicion del primer elemento de la lista y cambia el sprite dependiendo de su direccion



	}
	else if (snaku.dir == 0) {
		s_head = { { snaku.getint(pos.x),snaku.getint(pos.y), 30, 30 },ObjectID::SNAKE_HEAD_RIGHT };

	}
	else if (snaku.dir == 2) {
		s_head = { { snaku.getint(pos.x),snaku.getint(pos.y),30,30 },ObjectID::SNAKE_HEAD_LEFT };

	}
	else if (snaku.dir == 3) {
		s_head = { { snaku.getint(pos.x),snaku.getint(pos.y),30,30 },ObjectID::SNAKE_HEAD };

	}
	s_head.Draw(); //pinta la cabeza
	list<snake::COORD>::iterator posi;  //inicializa otro iterador
	posi = snaku.segments.begin(); //guarda el iterador de la primera posicion de la lista
	posi++; //incrementas al siguiente iterador de la lista

	while (posi != --snaku.segments.end()) { //mientras el iterador no apunte a la ultima posicion

		snaku.aux2 = *posi; //guardamos la posicion en una variable auxiliar



		if (snaku.dir == 1) {
			s_body = { { snaku.getint(snaku.aux2.x),snaku.getint(snaku.aux2.y),30,30 },ObjectID::SNAKE_BODY_DOWN }; //pintamos el sprite dependiendo de la direccion y la posicion de ese segmento

		}
		else if (snaku.dir == 0) {
			s_body = { { snaku.getint(snaku.aux2.x),snaku.getint(snaku.aux2.y), 30, 30 },ObjectID::SNAKE_BODY_RIGHT };


		}
		else if (snaku.dir == 2) {
			s_body = { { snaku.getint(snaku.aux2.x),snaku.getint(snaku.aux2.y),30,30 },ObjectID::SNAKE_BODY_LEFT };

		}
		else if (snaku.dir == 3) {
			s_body = { { snaku.getint(snaku.aux2.x),snaku.getint(snaku.aux2.y),30,30 },ObjectID::SNAKE_BODY };
		}

		posi++;//pasamos al siguiente iterador de la lista
		s_body.Draw();//pintamos este segmento del cuerpo por cada iteracion
	}

	snaku.aux2 = snaku.segments.back(); //al acabar de recorrrer el cuerpo recogemos la ultima posicion de la lista



	if (snaku.dir == 1) { //pintamos la cola dependiendo de su direccion en la coordenada final de la lista
		s_tail = { { snaku.getint(snaku.aux2.x),snaku.getint(snaku.aux2.y),30,30 },ObjectID::SNAKE_TAIL_DOWN };

	}
	else if (snaku.dir == 0) {
		s_tail = { { snaku.getint(snaku.aux2.x),snaku.getint(snaku.aux2.y), 30, 30 },ObjectID::SNAKE_TAIL_RIGHT };

	}
	else if (snaku.dir == 2) {
		s_tail = { { snaku.getint(snaku.aux2.x),snaku.getint(snaku.aux2.y),30,30 },ObjectID::SNAKE_TAIL_LEFT };

	}
	else if (snaku.dir == 3) {
		s_tail = { { snaku.getint(snaku.aux2.x),snaku.getint(snaku.aux2.y),30,30 },ObjectID::SNAKE_TAIL };
	}
	s_food = { { apples.position_x,apples.position_y,30,30 },ObjectID::FOOD };




	s_tail.Draw(); //pintamos la cola
	s_food.Draw(); //pintamos la comida
	Wall1.Draw(); //pintamos las paredes
	Wall2.Draw();
	Wall3.Draw();
	Wall4.Draw();
	timer.Draw();
}
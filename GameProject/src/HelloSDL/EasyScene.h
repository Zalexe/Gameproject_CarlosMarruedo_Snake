#pragma once
#include "Sprite.h"
#include "Scene.h"
#include "TimeManager.h"
// MenuScene class with the main gameplay mode
class EasyScene : public Scene {
public:
	explicit EasyScene();
	~EasyScene() override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	//iniciacion de los sprites que usaremos en el nivel
	Sprite m_background;
	Sprite s_head;
	Sprite s_body;
	Sprite s_tail;
	Sprite s_food;
	Sprite m_empty;
	Sprite Wall1;
	Sprite Wall2;
	Sprite Wall3;
	Sprite Wall4;
	Sprite timer;
	//tiempo limite
	int tiempo;
	int tiempomax;
	int sizeChekpoint;
	int nivel;

	int alimentosPorObt;
	
};
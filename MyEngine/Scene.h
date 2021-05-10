#pragma once


enum Scenes
{
	title,
	game,
	gameover,
	clear
};


class Scene
{
public:
	Scene(){}
	~Scene(){}

	void SetTitle() { scene = title; }
	void SetGame() { scene = game; }
	void SetGameover() { scene = gameover; }
	void SetClear() { scene = clear; }

	int GetNowScene() { return scene; }


	// ƒV[ƒ“•ÏX‚É‰Šú‰»ˆ—‚ğ‚·‚é‚©‚Ç‚¤‚©
	void SetInitTitle(bool a) { titleInit = a; }
	void SetInitGame(bool a) { gameInit = a; }
	void SetInitEnd(bool a) { endInit = a; }

	bool GetInitTitle() { return titleInit; }
	bool GetInitGame() { return gameInit; }
	bool GetInitEnd() { return endInit; }

private:



	Scenes scene = title;
	
	bool titleInit = true;
	bool gameInit = true;
	bool endInit = true;

};


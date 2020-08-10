#pragma once
#include "Vei2.h"
#include "Colors.h"
#include "Mouse.h"
#include "Graphics.h"
#include "SpriteCodex.h"

class Rect {
	Vei2 spot;
public:
	enum class type {
		hidden,
		revealed,
		tile1,
		tile2,
		tile3,
		tile4,
		tile5,
		tile6,
		tile7,
		tile8,
		bomb
	};
	Rect() = default;
	Rect(int one, int two);
	static constexpr int width = 16;
	static constexpr int height = 16;
	const Vei2 getSpot() const;
	type getType() const;
	void setPress();
	bool getCon() const;
	void setState(type t);
	void setNeighbor(int n);
	int getNeighbor() const;
private:
	type state = type::hidden;
	bool pressed = false;
	int neighbors;
};


class Rects {
	Rect rects[805];
	bool initialized = false;
public:
	Rects();
	Rect* getRect();
	void evaluateMouse(const Mouse& mouse, bool &gameIsOver);
	void draw(Graphics& gfx);
	int convert(int x, int y);
	void tileFate(int i, int j);
	void chooseTiles(int i, int j);
	void setGameOver();
};
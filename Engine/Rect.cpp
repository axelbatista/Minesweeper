#include "Rect.h"
#include <random>
#include <ctime>

namespace randl {
	std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
}

Rects::Rects()
{
	int n = 0;
	for (int j = 120; j < 488; j += Rect::height) {
		for (int i = 120; i < 680; i += Rect::width) {
			rects[n] = { i,j };
			n++;
		}
	}
}

Rect* Rects::getRect()
{
	return rects;
}

void Rects::draw(Graphics& gfx)
{
	for (const auto& each : rects) {
		if (each.getType() == Rect::type::hidden) {
			SpriteCodex::DrawTileButton(each.getSpot(), gfx);
		}
		else if (each.getType() == Rect::type::revealed) {
			SpriteCodex::DrawTile0(each.getSpot(), gfx);
		}
		else if (each.getType() == Rect::type::bomb) {
			if (each.getCon()) {
				SpriteCodex::DrawTileBomb(each.getSpot(), gfx);
			}	else {
				SpriteCodex::DrawTileButton(each.getSpot(), gfx);
			}
		}	else if (each.getType() == Rect::type::tile1) {
			SpriteCodex::DrawTile1(each.getSpot(), gfx);
		}
		else if (each.getType() == Rect::type::tile2) {
			SpriteCodex::DrawTile2(each.getSpot(), gfx);
		}
		else if (each.getType() == Rect::type::tile3) {
			SpriteCodex::DrawTile3(each.getSpot(), gfx);
		}
		else if (each.getType() == Rect::type::tile4) {
			SpriteCodex::DrawTile4(each.getSpot(), gfx);
		}
		else if (each.getType() == Rect::type::tile5) {
			SpriteCodex::DrawTile5(each.getSpot(), gfx);
		}
		else if (each.getType() == Rect::type::tile6) {
			SpriteCodex::DrawTile6(each.getSpot(), gfx);
		}
		else if (each.getType() == Rect::type::tile7) {
			SpriteCodex::DrawTile7(each.getSpot(), gfx);
		}
		else if (each.getType() == Rect::type::tile8) {
			SpriteCodex::DrawTile8(each.getSpot(), gfx);
		}
	}
}

int Rects::convert(int x, int y)
{
	return y * 35 + x;
}

void Rects::tileFate(int i, int j)
{
	switch (rects[convert(i,j)].getNeighbor()) {
	case 0:
		rects[convert(i, j)].setState(Rect::type::revealed);
		break;
	case 1:
		rects[convert(i, j)].setState(Rect::type::tile1);
		break;
	case 2:
		rects[convert(i, j)].setState(Rect::type::tile2);
		break;
	case 3:
		rects[convert(i, j)].setState(Rect::type::tile3);
		break;
	case 4:
		rects[convert(i, j)].setState(Rect::type::tile4);
		break;
	case 5:
		rects[convert(i, j)].setState(Rect::type::tile5);
		break;
	case 6:
		rects[convert(i, j)].setState(Rect::type::tile6);
		break;
	case 7:
		rects[convert(i, j)].setState(Rect::type::tile7);
		break;
	case 8:
		rects[convert(i, j)].setState(Rect::type::tile8);
		break;
	}
}

void Rects::chooseTiles(int i, int j)
{
	if (rects[convert(i, j)].getType() == Rect::type::hidden) {
		int n = 0;
		int xStart;
		int xEnd;
		int yStart = j - 1;
		int yEnd = j + 2;
		if (i == 34) {
			xStart = i - 1;
			xEnd = i + 1;
		}
		else if (i == 0) {
			xStart = i;
			xEnd = i + 2;
		}
		else {
			xStart = i - 1;
			xEnd = i + 2;
		}
		for (int x = xStart; x < xEnd; x++) {
			for (int y = yStart; y < yEnd; y++) {
				if (x == i && j == y) 
					continue;
				if (rects[convert(x, y)].getType() == Rect::type::bomb) {
					n++;
				}
			}
		}
		/*if (n == 0) {
			for (int m = xStart; m < xEnd; m++) {
				for (int n = yStart; n < yEnd && convert(m, n) >= 0 && convert(m, n) <= 805; n++) {
					chooseTiles(m,n);
				}
			}
		}*/
		rects[convert(i, j)].setNeighbor(n);
		tileFate(i, j);
	}
				
}

void Rects::setGameOver()
{
	for (auto& each : rects) {
		if (each.getType() == Rect::type::bomb) {
			each.setPress();
		}
	}
}

void Rects::evaluateMouse(const Mouse& mouse, bool &gameIsOver)
{
		if (mouse.LeftIsPressed()) {
			for (int i = 0; i < 35; i++) {
				for (int j = 0; j < 23; j++) {
					if (mouse.GetPosX() >= rects[convert(i, j)].getSpot().x && mouse.GetPosX() <= rects[convert(i, j)].getSpot().x + Rect::width && mouse.GetPosY() >= rects[convert(i, j)].getSpot().y && mouse.GetPosY() <= rects[convert(i, j)].getSpot().y + Rect::height) {
						if (rects[convert(i, j)].getType() == Rect::type::bomb) {
							gameIsOver = true;
							setGameOver();
						}else {
							chooseTiles(i, j);
						}
					}
				}
			}
	}
}

Rect::Rect(int one, int two) : spot{one, two}
{
	std::uniform_int_distribution<int> choice(1, 10);
	if (choice(randl::mt) == 1)
		state = Rect::type::bomb;
}

const Vei2 Rect::getSpot() const
{
	return spot;
}

Rect::type Rect::getType() const
{
	return state;
}

void Rect::setPress()
{
	pressed = true;
}

bool Rect::getCon() const
{
	return pressed;
}

void Rect::setState(type t)
{
	state = t;
}

void Rect::setNeighbor(int n)
{
	neighbors = n;
}

int Rect::getNeighbor() const
{
	return neighbors;
}




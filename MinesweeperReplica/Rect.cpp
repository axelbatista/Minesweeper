#include "Rect.h"
#include <random>
#include <ctime>

namespace randl {
	std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };		//Pseudo random value for generation
}

Rects::Rects()
{
	int n = 0;
	for (int j = 120; j < 488; j += Rect::height) {
		for (int i = 120; i < 680; i += Rect::width) {			//initialize array for dimensions for every single rect via the numbers shown here
			rects[n] = { i,j };									//only stores dimensions to know where to draw the tiles.
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
		switch (each.getType()) {
		case Rect::type::hidden:
			SpriteCodex::DrawTileButton(each.getSpot(), gfx);				
			break;
		case Rect::type::revealed:
			SpriteCodex::DrawTile0(each.getSpot(), gfx);
			break;
		case Rect::type::bomb:
			if (each.getCon())
				SpriteCodex::DrawTileBomb(each.getSpot(), gfx);
			else 
				SpriteCodex::DrawTileButton(each.getSpot(), gfx);
			break;
		case Rect::type::tile1:
			SpriteCodex::DrawTile1(each.getSpot(), gfx);
			break;
		case Rect::type::tile2:
			SpriteCodex::DrawTile2(each.getSpot(), gfx);
			break;
		case Rect::type::tile3:
			SpriteCodex::DrawTile3(each.getSpot(), gfx);
			break;
		case Rect::type::tile4:
			SpriteCodex::DrawTile4(each.getSpot(), gfx);
			break;
		case Rect::type::tile5:
			SpriteCodex::DrawTile5(each.getSpot(), gfx);
			break;
		case Rect::type::tile6:
			SpriteCodex::DrawTile6(each.getSpot(), gfx);
			break;
		case Rect::type::tile7:
			SpriteCodex::DrawTile7(each.getSpot(), gfx);
			break;
		case Rect::type::tile8:
			SpriteCodex::DrawTile8(each.getSpot(), gfx);
			break;
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
		rects[convert(i, j)].setState(Rect::type::tile2);			//the type of the tile is determined by it's "neighbors"
		break;														//not so much the empty tiles but the ones that have bombs in them
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
	if (rects[convert(i, j)].getType() == Rect::type::revealed) return;				//recursively we may do added work by checking on something that we
	int n{};																		//already determined is just a revealed tile. We skip it with a return.
	for (int x = i - 1; x < i + 2; x++) {
		for (int y = j - 1; y < j + 2; y++) {
			if (x == i && y == j) continue;
			else if (x < 0) continue;
			else if (x >= iDim) continue;									//check all the neighboring tiles and check which have bombs, from which
			else if (y < 0) continue;										//we tally it up and give it to the setneighbor function. Due to the container being
			else if (y >= jDim) continue;									//one dimensional and checking for obbious cases of outside the grid, we have to do some bounds checking, which is what the continues are for.
			if (rects[convert(x, y)].getType() == Rect::type::bomb) {
				n++;
			}
		}
	}
	rects[convert(i, j)].setNeighbor(n);				//have to set the tiles here before continuing on with the recursion or else we get an infinite loop
	tileFate(i, j);										//where a revealed tile keeps being worked on and so on. The premise is the same as the neighbor checking from
	if (n == 0) {										//before, but it's important to distinguish that we only do recursion if it is a redundant tile so to speak, and to keep on checking for redundant tiles.
		for (int x = i - 1; x < i + 2; x++) {
			for (int y = j - 1; y < j + 2; y++) {
				if (x == i && y == j) continue;
				else if (x < 0) continue;
				else if (x >= iDim) continue;
				else if (y < 0) continue;
				else if (y >= jDim) continue;
				chooseTiles(x, y);
			}
		}
	}
}

void Rects::setGameOver()
{
	for (auto& each : rects) {
		if (each.getType() == Rect::type::bomb) {			//when game is over from you touching a bomb, this just reveals all the bombs on the map.
			each.setPress();								//once the game is in a game over state (unplayable).
		}
	}
}

void Rects::evaluateMouse(const Mouse& mouse, bool &gameIsOver)
{
		bool value1 = mouse.LeftIsPressed();
		if (value1) {
			for (int i = 0; i < iDim; i++) {
				for (int j = 0; j < jDim; j++) {
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

// ^^^^^^^^^^^^^ The above could be improved by using the location of the mouse into an entry/index into the container instead, but
//it would require a refactoring of the whole code so it is left as is for now.

Rect::Rect(int one, int two) : spot{one, two}
{
	std::uniform_int_distribution<int> choice(1, 10);
	if (choice(randl::mt) == 1)								//when the container is being filled up with values, those values are being 
		state = Rect::type::bomb;							//constructed here based on some arguments, and what we do is that we check upon
}															//a random value to see whether that tile is one to be a bomb, thereby simulating a new game each time.

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




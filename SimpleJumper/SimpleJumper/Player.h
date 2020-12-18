#include <Windows.h>

class Player {
public:
	bool exists = false;
	bool jump = false;
	int x;
	int y;
	int width;
	int height;
	int step;
	RECT prevRect;
	COLORREF color;
public:
	Player() {
		x = 0;y = 0;width = 0;height = 0;
	}
	void fillPlayer( RECT gameRect, COLORREF Color) {
		prevRect = gameRect;
		this->exists = true;
		this->color = Color;
		this->height = (gameRect.bottom - gameRect.top) / 4;
		this->width = (gameRect.right - gameRect.left) / 20;
		this->x = gameRect.left + (gameRect.right - gameRect.left) / 12;
		this->y = gameRect.bottom - this->height;
		step = (gameRect.right - gameRect.left) / 100;

	}
	void Resize(RECT gameRect) {
		width = (gameRect.right - gameRect.left) / 20;
		height = (gameRect.bottom - gameRect.top) / 4;
		this->x = gameRect.left + (gameRect.right - gameRect.left) / 12;
		y = gameRect.bottom - this->height;
		step = (gameRect.right - gameRect.left) / 100;
		prevRect = gameRect;
	}
	void emptyPlayer() {
		exists = false;
	}
	void DrawPlayer(HDC hdc) {
		HBRUSH brush = CreateSolidBrush(color);
		SelectObject(hdc, brush);
		Rectangle(hdc, x, y, x + width, y + height);
		DeleteObject(brush);
	}
	void Die() {
		exists = false;
	}
	void Jump() {
		jump = true;
	}
	void Decending() {
		if (jump) {
			if (y > prevRect.bottom - height * 3) {
				y -= 2 * step;
			}
			else {
				jump = false;
			}
		}
		else {
			if (y < prevRect.bottom - height)
				y += step;
		}
	}
	void ChangeColor(COLORREF color) {
		this->color = color;
	}
};
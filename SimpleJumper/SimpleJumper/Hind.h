#include <Windows.h>

enum class HindType {
	onGroundShort = 0,
	onGroundLong = 1,
	FlyingShort = 2,
} ;



class Hind{
public:
	bool exists = false;
	int x;
	int y;
	int width;
	int height;
	int step;
	RECT prevRect;
	COLORREF color;
	HindType type;
public:
	Hind() {
		x = 0;y = 0;width = 0;height = 0;step = 0;
	}
	void fillHind(HindType type,RECT gameRect,COLORREF Color ) {
		prevRect = gameRect;
		this->exists = true;
		this->type = type;
		this->color = Color;
		step = (gameRect.right - gameRect.left) / 100;
		switch (type)
		{
		case HindType::onGroundShort:
			width = (gameRect.right - gameRect.left )/20;
			height = (gameRect.bottom-gameRect.top)/3;
			x = gameRect.right;
			y = gameRect.bottom - height;
			break;
		case HindType::onGroundLong:
			width = (gameRect.right - gameRect.left) / 20;
			height = (gameRect.bottom - gameRect.top);
			x = gameRect.right;
			y = gameRect.bottom - height;
			break;
		case HindType::FlyingShort:
			width = (gameRect.right - gameRect.left) / 20;
			height = (gameRect.bottom - gameRect.top) / 3;
			x = gameRect.right;
			y = gameRect.bottom - 2* height;
			break;
		default:
			width = (gameRect.right - gameRect.left) / 20;
			height = (gameRect.bottom - gameRect.top) / 3;
			x = gameRect.right;
			y = gameRect.bottom - height;
			break;
		}

	}
	void Resize(RECT gameRect) {
		step = (gameRect.right - gameRect.left) / 100;
		x = (x * (gameRect.right - gameRect.left)) / (prevRect.right - prevRect.left);
		width = (gameRect.right - gameRect.left) / 20;
		height = (gameRect.bottom - gameRect.top) / 3;
		switch (type)
		{
		case HindType::onGroundShort:
			y = gameRect.bottom - height;
			break;
		case HindType::onGroundLong:
			y = gameRect.bottom - height;
			break;
		case HindType::FlyingShort:
			y = gameRect.bottom - 2*height;
			break;
		default:
			y = gameRect.bottom - height;
			break;
		}
		
		prevRect = gameRect;
	}
	void emptyHind() {
		exists = false;
	}
	void DrawHind(HDC hdc) {
		HBRUSH brush = CreateSolidBrush(color);
		SelectObject(hdc, brush);
		Rectangle(hdc, x, y, x+ width, y+height);
		DeleteObject(brush);
	}
	void MoveHind() {
		x -= step;
	}
};
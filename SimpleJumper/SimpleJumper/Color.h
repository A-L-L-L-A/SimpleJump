#include <Windows.h>
#include <stdlib.h>
struct Color
{
public:
	static const COLORREF red = RGB(255, 0, 0);
	static const COLORREF blue = RGB(0, 0, 255);
	static const COLORREF green = RGB(0, 255, 0);
	static const COLORREF white = RGB(255, 255, 255);
	static const COLORREF black = RGB(0, 0, 0);

public:
	COLORREF getRandColor() {
		switch (rand()%5)
		{
		case 0:
			return red;
		case 1:
			return blue;
		case 2:
			return black;
		case 3:
			return green;
		case 4:
			return white;
		}
	}
};
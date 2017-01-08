#include "BitMap.h"
#include <iostream>


using namespace BitMap_Lib;
using namespace std;

int main(int argc, char* argv[])
{
	string path = "image.bmp";

	try
	{
		BitMap* map = new BitMap(path);

		auto x = map->width() / 2;
		auto y = map->height() / 2;
		auto channel = 0;

		auto pixle = map->data(x, y, channel);

		cout << *pixle << endl;

		delete map;
	}
	catch (const BitMapIOException e)
	{
		cout << e.what() << endl;
	}
}

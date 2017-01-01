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
		auto pixle = map->data(map->width() / 2, map->height() / 2, 0);

		cout << "pixel: " << pixle << endl;

		delete map;
	}
	catch (const BitMapIOException e)
	{
		cout << e.what() << endl;
	}
}

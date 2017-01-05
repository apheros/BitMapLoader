#include "BitMap.h"


using namespace std;


namespace BitMap_Lib
{
	const uint_32 BITMAP_HEADER_LENGTH = 54;


	void BitMapFileHeader::unpack(const BYTE* buff)
	{
		bfType = WORD(buff[0] <<8 | buff[1]);
		bfSize = DWORD(buff[2] | buff[3] << 8 | buff[4] << 16 | buff[5] << 24);
		bfReserved1 = WORD(buff[6] | buff[7] << 8);
		bfReserved2 = WORD(buff[8] | buff[9] << 8);
		bfOffBits = DWORD(buff[10] | buff[11] << 8 | buff[12] << 16 | buff[13] << 24);
	}

	void BitMapInfoHeader::unpack(const BYTE* buff)
	{
		biSize = DWORD(buff[14] | buff[15] << 8 | buff[16] << 16 | buff[17] << 24);
		biWidth = DWORD(buff[18] | buff[19] << 8 | buff[20] << 16 | buff[21] << 24);
		biHeight = DWORD(buff[22] | buff[23] << 8 | buff[24] << 16 | buff[25] << 24);
		biPlanes = WORD(buff[26] | buff[27] << 8);
		biBitCount = WORD(buff[28] | buff[29] << 8);
		biCompression = DWORD(buff[30] | buff[31] << 8 | buff[32] << 16 | buff[33] << 24);
		biSizeImage = DWORD(buff[34] | buff[35] << 8 | buff[36] << 16 | buff[37] << 24);
		biXPelsPerMeter = DWORD(buff[38] | buff[39] << 8 | buff[40] << 16 | buff[41] << 24);
		biYPelsPerMeter = DWORD(buff[42] | buff[43] << 8 | buff[44] << 16 | buff[45] << 24);
		biClrUsed = DWORD(buff[46] | buff[47] << 8 | buff[48] << 16 | buff[49] << 24);
		biClrImportant = DWORD(buff[50] | buff[51] << 8 | buff[52] << 16 | buff[53] << 24);
	}


	BitMap::BitMap()
		:_file_header(nullptr)
		,_info_header(nullptr)
		,_data(nullptr)
		,_width(0)
		,_height(0)
		,_depth(0)
		,_spectrum(0)
	{
	}

	BitMap::BitMap(const string& path_str)
		:_file_header(nullptr)
		, _info_header(nullptr)
		, _data(nullptr)
		, _width(0)
		, _height(0)
		, _depth(0)
		, _spectrum(0)
	{
		_loadBitMap(path_str);
	}

	BitMap::BitMap(const BYTE* map_buff, uint_32 length)
	{
		_unpackBitMap(map_buff, length);
	}

	//spectrum is the bits count of bmp
	BitMap::BitMap(const BYTE* pixle_buff, unsigned long width, unsigned long height, unsigned long spectrum)
		:_file_header(nullptr)
		,_info_header(nullptr)
		, _data(pixle_buff)
		,_width(width)
		,_height(height)
		,_depth(1)
		,_spectrum(spectrum)
	{
	}

	BitMap::~BitMap()
	{
		delete _file_header;
		delete _info_header;
		delete[] _data;
	}

	void BitMap::_loadBitMap(const string& file_path)
	{
		FILE* file = fopen(file_path.c_str(), "rb");
		if (file == nullptr)
		{
			throw BitMapIOException("File in Path: " + file_path + " is not existed!");
		}

		fseek(file, 0, SEEK_END);
		auto length = ftell(file);
		char* buff = new char[length+1];
		fseek(file, 0, SEEK_SET);
		fgets(buff, length, file);
		buff[length] = '\0';
		fclose(file);

		auto map_buff = reinterpret_cast<BYTE*>(buff);

		_unpackBitMap(map_buff, length);
	}

	unique_ptr<BYTE> BitMap::data(uint_32 x, uint_32 y, uint_32 channel)
	{
		x = x < 0 ? 0 : x;
		x = x > _width - 1 ? _width - 1 : x;
		y = y < 0 ? 0 : y;
		y = y > _height - 1 ? _height - 1 : y;

		unique_ptr<BYTE> pixle = nullptr;
		uint_64 distance = 0;
		if (_spectrum == 1)
		{
			distance = x + (_height - y) * _width;
			pixle = unique_ptr<BYTE>(new BYTE(*(_data + distance / 8)));
			*pixle = *pixle << distance % 8;
			*pixle = *pixle >> 7;
		}
		if (_spectrum == 4)
		{
			distance = x + (_height - y) * _width;
			pixle = unique_ptr<BYTE>(new BYTE(*(_data + distance / 2)));
			*pixle = *pixle << (distance % 2) * 4;
			*pixle = *pixle >> (4 - (distance % 2) * 4);
		}
		else if (_spectrum >= 8)
		{
			distance = (x + (_height - y) * _width) * (_spectrum/8) + channel;
			pixle = unique_ptr<BYTE>(new BYTE(*(_data + distance)));
		}

		return pixle;
	}
}

void BitMap_Lib::BitMap::_unpackBitMap(const BYTE* map_buff, unsigned long length)
{
	if (length > BITMAP_HEADER_LENGTH && map_buff[0] == 'B' && map_buff[1] == 'M')
	{
		_file_header = new BitMapFileHeader();
		_info_header = new BitMapInfoHeader();

		_file_header->unpack(map_buff);
		_info_header->unpack(map_buff);

		if (length >= _file_header->bfSize)
		{
			_data = (map_buff + _file_header->bfOffBits);
			_width = _info_header->biWidth;
			_height = _info_header->biHeight;
			_depth = _info_header->biPlanes;
			_spectrum = _info_header->biBitCount;
		}
	}
}

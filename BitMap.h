#pragma once
#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <string>


namespace BitMap_Lib
{
#define int_8 char
#define int_16 short
#define int_32 long
#define int_64 long long
#define uint_8 unsigned char
#define uint_16 unsigned short
#define uint_32 unsigned long
#define uint_64 unsigned long long

#define BYTE uint_8
#define WORD  uint_16
#define DWORD uint_32


#define DEFINE_PROPERTY(Type, value); \
	private: \
		Type _##value; \
	public: \
		Type value() { return _##value; }

	class BitMapIOException : public std::exception
	{
	public:
		BitMapIOException(const std::string& msg) : _err_msg(msg) {}
		~BitMapIOException() {}

	private:
		std::string _err_msg;

	public:
		virtual const char* what() const throw()
		{
			return _err_msg.c_str();
		}
	};

	struct BitMapFileHeader
	{
		WORD  bfType;
		DWORD bfSize;
		WORD  bfReserved1;
		WORD  bfReserved2;
		DWORD bfOffBits;

		void unpack(const BYTE* buff);
	};

	struct BitMapInfoHeader
	{
		DWORD biSize;
		DWORD  biWidth;
		DWORD  biHeight;
		WORD  biPlanes;
		WORD  biBitCount;
		DWORD biCompression;
		DWORD biSizeImage;
		DWORD  biXPelsPerMeter;
		DWORD  biYPelsPerMeter;
		DWORD biClrUsed;
		DWORD biClrImportant;

		void unpack(const BYTE* buff);
	};

	class BitMap
	{
	public:
		BitMap();
		BitMap(const std::string& path_str);
		BitMap(const BYTE* map_buff, uint_32 length);
		BitMap(const BYTE* pixle_buff, uint_32 width, uint_32 height, uint_32 spectrum);
		~BitMap();

	public:
		const BYTE* data(uint_32 x, uint_32 y, uint_32 channel = 0);
		void setPixleData(const BYTE* pixle_data);

	private:
		void _loadBitMap(const std::string& file_path);
		void _unpackBitMap(const BYTE* map_buff, uint_32 length);

	private:
		BitMapFileHeader* _file_header;
		BitMapInfoHeader* _info_header;
		const BYTE* _data;

	private:
		DEFINE_PROPERTY(uint_32, width);
		DEFINE_PROPERTY(uint_32, height);
		DEFINE_PROPERTY(uint_32, depth);
		DEFINE_PROPERTY(uint_32, spectrum);
	};
}

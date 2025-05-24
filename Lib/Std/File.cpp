#include "File.h"
#include <fstream>

Str ReadFileToStr(FilePath path)
{
	Str sFile;
	const auto sz = std::filesystem::file_size(path);
	if ( sz != static_cast<std::uintmax_t>(-1) )
	{
		sFile.resize(sz);
		std::ifstream f(path, std::ios::in | std::ios::binary);
		f.read(sFile.data(), sz);
	}
	return sFile;
}

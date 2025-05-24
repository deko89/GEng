#ifndef File_H
#define File_H

#include <filesystem>
#include "Base.h"

typedef std::filesystem::path FilePath;

/// Прочитать файл в строку.
Str ReadFileToStr(FilePath path);

#endif

#ifndef DlgFile_H
#define DlgFile_H

#include "nativefiledialog/src/include/nfd.h"
#include "Std/File.h"

namespace GEng
{

/// Диалог выбора файла.
class DlgFile
{	public:
	FilePath path = ".";			///< Стартовая папка. После итоговый путь.
	Str filter = "txt";				///< Фильтр расширений. Пример: txt;png,jpg
	/**	\brief Показать диалог.
		\param[out] path - Итоговый путь.
		\return 1 - нажата ок; 0 - выбрана отмена. */
	bool Open() {return ShowDlg<NFD_OpenDialog>();}
	bool Save() {return ShowDlg<NFD_SaveDialog>();}
private:
	template<nfdresult_t (*F)(const nfdchar_t*, const nfdchar_t*, nfdchar_t**)>
	bool ShowDlg()
	{   nfdchar_t* outPath = nullptr;
		nfdresult_t result = F(filter.c_str(), path.string().c_str(), &outPath);
		if (result == NFD_OKAY)
		{   path = outPath;
			free(outPath);
			return 1;
		}
		return 0;
	}
};

}

#endif

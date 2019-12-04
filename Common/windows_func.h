#ifndef WINDOWS_FUNC_H
#define WINDOWS_FUNC_H

#include <Windows.h>

namespace c2::window
{
	wchar_t* GetProcessNameW();
	bool UTF16toUTF8(wchar_t* wide_char_set, char* utf8_char_set, size_t utf8_char_set_capacity);
	bool UTF8toUTF16(char* utf8_char_set, wchar_t* wide_char_set, size_t wide_char_set_capacity);
	bool MultibyteToUTF16(char* multi_char_set, wchar_t* wide_char_buffer, size_t wide_char_buffer_capacity);

}

#endif
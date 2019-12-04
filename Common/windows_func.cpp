#include "windows_func.h"

namespace c2::window
{
	wchar_t* GetProcessNameW()
	{
		const size_t max_path = 260;

		wchar_t* processName		{ nullptr };
		wchar_t* slashIdx			{ nullptr };
		wchar_t* commaIdx			{ nullptr };
		wchar_t szBuffer[max_path]	{  };
		int pathLength				{ 0 };
		int processNameLength		{ 0 };

		pathLength = GetModuleFileNameW(NULL, szBuffer, max_path);
		if (0 >= pathLength)
		{
			// error handling....
		}

		slashIdx = wcsrchr(szBuffer, L'\\');
		commaIdx = wcsrchr(szBuffer, L'.');

		processNameLength = commaIdx - slashIdx - 1/* ( '.'���� ����� ����...) */;

		processName = new wchar_t[processNameLength + 1]{};
		// �Ҵ� �������� üũ.
		//if( nullptr != processName )
		/*{
			return nullptr;
		} */

		memcpy(processName, slashIdx + 1, processNameLength * sizeof(wchar_t));

		//wprintf(L"FILENAME : %s\n", processName);
		return processName;
	}


	bool UTF16toUTF8(wchar_t* wide_char_set, char* utf8_char_set, size_t utf8_char_set_capacity)
	{
		// CP_ACP�� �Ϲ� mutli_byte
		// CP_UTF8
		int ret = WideCharToMultiByte(CP_UTF8, 0, wide_char_set, wcslen(wide_char_set), utf8_char_set, utf8_char_set_capacity, NULL, NULL);
		
		if (0 == ret) // ����.
			return false; // nullptr

		if (ret < utf8_char_set_capacity)
			utf8_char_set[ret] = '\0';

		return true;
	}

	bool UTF8toUTF16(char* utf8_char_set, wchar_t* wide_char_buffer, size_t wide_char_buffer_capacity)
	{
		// CP_ACP�� �Ϲ� mutli_byte
	// CP_UTF8
		int ret = MultiByteToWideChar(CP_UTF8, 0, utf8_char_set, strlen(utf8_char_set), wide_char_buffer, wide_char_buffer_capacity);

		if (0 == ret) // ����.
			return false; // nullptr

		if (ret < wide_char_buffer_capacity)
			wide_char_buffer[ret] = L'\0';

		return true;
	}
	
	bool MultibyteToUTF16(char* multi_char_set, wchar_t* wide_char_buffer, size_t wide_char_buffer_capacity)
	{
		// CP_ACP�� �Ϲ� mutli_byte
	// CP_UTF8
		int ret = MultiByteToWideChar(CP_ACP, 0, multi_char_set, strlen(multi_char_set), wide_char_buffer, wide_char_buffer_capacity);

		if (0 == ret) // ����.
			return false; // nullptr

		if (ret < wide_char_buffer_capacity)
			wide_char_buffer[ret] = L'\0';

		return true;
	}


}
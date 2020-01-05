#pragma once

#include <Windows.h>
#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct file_handle;
	}

	class fog {
	public:
		static char* get_base();

		static bool mpq_open_file(char* path, structures::file_handle** outHandle);
		static bool mpq_close_file(structures::file_handle* handle);
		static bool mpq_read_file(structures::file_handle* handle, void* buffer, size_t size, size_t* bytesToRead);
		static size_t mpq_get_file_size(structures::file_handle* handle, size_t* compressedSize);
	};
}
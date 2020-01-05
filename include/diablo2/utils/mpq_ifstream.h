#pragma once

#include <istream>

namespace diablo2 {
	namespace structures {
		struct file_handle;
	}

	namespace utils {
		class mpq_ifstream : public std::istream {
			class mpq_streambuf : public std::streambuf {
				structures::file_handle* m_handle;
				char m_data;
			public:
				explicit mpq_streambuf(const std::string& path);
				~mpq_streambuf();
				
			protected:
				int_type underflow() override;
			};

			mpq_streambuf m_streambuf;
		public:
			explicit mpq_ifstream(const std::string& path);


		};
	}
}

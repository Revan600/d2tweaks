#include <diablo2/utils/mpq_ifstream.h>
#include <diablo2/fog.h>

#include <spdlog/spdlog.h>

diablo2::utils::mpq_ifstream::mpq_streambuf::mpq_streambuf(const std::string& path) {
	fog::mpq_open_file(const_cast<char*>(path.c_str()), &m_handle);
}

diablo2::utils::mpq_ifstream::mpq_streambuf::~mpq_streambuf() {
	fog::mpq_close_file(m_handle);
}

std::basic_streambuf<char>::int_type diablo2::utils::mpq_ifstream::mpq_streambuf::underflow() {
	if (!m_handle)
		return EOF;
	
	if (!fog::mpq_read_file(m_handle, &m_data, 1, nullptr))
		return EOF;

	setg(&m_data, &m_data, &m_data + 1);

	return m_data;
}

diablo2::utils::mpq_ifstream::mpq_ifstream(const std::string& path) : std::istream(&m_streambuf), m_streambuf(path) {

}

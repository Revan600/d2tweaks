#include <common/string_utils.h>

#include <Windows.h>

std::wstring string_utils::string_to_wstring(const std::string& str) {
	if (str.empty())
		return std::wstring();

	const auto sizeNeeded = MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), nullptr, 0);
	std::wstring wstrTo(sizeNeeded, 0);
	MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), &wstrTo[0], sizeNeeded);

	return wstrTo;
}

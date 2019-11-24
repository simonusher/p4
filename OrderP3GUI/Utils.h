#pragma once
#include <vector>
#include <qtextstream.h>

namespace utils {
	template <typename T>
	QTextStream& operator<<(QTextStream& os, const std::vector<T>& vec) {
		os << "[ ";
		for (int i = 0; i < vec.size(); i++) {
			os << vec[i];
			if (i != vec.size() - 1) {
				os << ", ";
			}
		}
		os << " ]";
		return os;
	}
}
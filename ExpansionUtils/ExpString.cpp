#include "ExpString.h"
#include "pch.h"

std::string WordUpcase(std::string toUpcase) {
	char buffer = ' ';
	for (int i = 0; i < toUpcase.size(); i++) {
		if (buffer == toUpcase[i]) {
			toUpcase[i] = toupper(toUpcase[i]);
		}
			buffer = toUpcase[i];
	}
}
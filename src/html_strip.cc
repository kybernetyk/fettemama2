#include "html_strip.h"

//quick and dirtey
//i hope no one sees this ... ever :]
std::string html_strip(std::string &input) {
	const char *pinput = input.c_str();
	std::string output;

	int invalid = 0;
	for (;;) {
		if (*pinput == 0x00)
			break;

		if (*pinput == '<') {
			invalid++;
			pinput++;
			continue;
		}
		if (*pinput == '>') {
			invalid--;
			pinput++;
			continue;
		}
		char c = *pinput++;
		if (invalid == 0)
			output += c;
	}
	return output;
}

#if 0
int main(int argc, char **argv) {
	std::string html = "<a href='http://google.com'>This is a link</a>. <b>Links rock!</b>";
	printf("%s\n", html_strip(html).c_str());
	return 0;
}
#endif

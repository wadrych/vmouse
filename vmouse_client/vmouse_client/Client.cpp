#include <stdexcept>
#include <iostream>

#include "Mouse.h"

int main() {
	std::cout << "VMouse started" << std::endl;
	Mouse mouse;
	try {
		mouse.Initialize();
	}
	catch(const std::runtime_error& e) {
		std::cout << std::string("Mouse initialization: ") + e.what() << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "VMouse started" << std::endl;

	auto quit{ false };
	while(!quit) {
		char command_type;

		std::cin >> command_type;

		switch(command_type) {
		case 'm':
			int x, y;
			std::cin >> x >> y;
			mouse.MoveCursor(x, y);
			break;
		case 'r':
			mouse.RightBtnClick();
			break;
		case 'l':
			mouse.LeftBtnClick();
			break;
		case 'q':
			quit = true;
			break;
		default:
			std::cout << "Invalid command" << std::endl;
			break;
		}
	}

	return EXIT_SUCCESS;
}

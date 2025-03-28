#include "BackEnd/BackEnd.h"
#include <iostream>

int main()
{

	if (!BackEnd::Init(WindowedMode::WINDOWED)) {
		std::cout << "BankEnd::Init() FAILED.\n";
		return -1;
	}

	while (BackEnd::WindowIsOpen()) {
		BackEnd::BeginFrame();

		// Load
		
		// Renderer

		BackEnd::EndFrame();
	}

	BackEnd::CleanUp();

	return 0;
}
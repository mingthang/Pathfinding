#include "BackEnd/BackEnd.h"
#include <AssetManager/AssetManager.h>
#include <Renderer/Renderer.h>
#include <iostream>

int main()
{

	if (!BackEnd::Init(WindowedMode::WINDOWED)) {
		std::cout << "BankEnd::Init() FAILED.\n";
		return -1;
	}

	while (BackEnd::WindowIsOpen()) {
		BackEnd::BeginFrame();

		// Render Loading Screen
		if (!AssetManager::LoadingComplete()) {
			AssetManager::UpdateLoading();
			Renderer::RenderLoadingScreen();

			//if(AssetManager::LoadingComplete())
		}
		// Update/Render Game
		else {
			//BackEnd::UpdateGame();
			//Renderer::
		}

		BackEnd::EndFrame();
	}

	BackEnd::CleanUp();

	return 0;
}
#include "BackEnd/BackEnd.h"
#include <AssetManager/AssetManager.h>
#include <Renderer/Renderer.h>
#include <Core/Game.h>
#include <iostream>

int main()
{

	if (!BackEnd::Init(WindowedMode::WINDOWED)) {
		std::cout << "BankEnd::Init() FAILED.\n";
		return -1;
	}

	while (BackEnd::WindowIsOpen()) {
		BackEnd::BeginFrame();
		BackEnd::UpdateSubSystems();

		// Render Loading Screen
		if (!AssetManager::LoadingComplete()) {
			AssetManager::UpdateLoading();
			Renderer::RenderLoadingScreen();

			if (AssetManager::LoadingComplete())
				Game::Create();
		}
		// Update/Render Game
		else {
			BackEnd::UpdateGame();
			Renderer::RenderGame();
		}

		BackEnd::EndFrame();
	}

	BackEnd::CleanUp();

	return 0;
}
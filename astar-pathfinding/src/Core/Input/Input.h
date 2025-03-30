#pragma once

#include "Input_keycodes.h"

namespace Input {
	void Init(void* glfwWindow);
	void Update();

	void SetCursorPosition(int x, int y);

	void PreventRightMouseHold();
	void DisableCursor();
	void HideCursor();
	void ShowCursor();
	void CenterMouseCursor();

	bool KeyPressed(unsigned int keyCode);
	bool KeyDown(unsigned int keyCode);
	bool LeftMouseDown();
	bool RightMouseDown();
	bool MiddleMouseDown();
	bool LeftMousePressed();
	bool RightMousePressed();
	bool MiddleMousePressed();
	bool MouseWheelUp();
	bool MouseWheelDown();

	int GetMouseX();
	int GetMouseY();
	int GetMouseXPreviousFrame();
	int GetMouseYPreviousFrame();
	int GetCursorScreenX();
	int GetCursorScreenY();
	float GetMouseOffsetX();
	float GetMouseOffsetY();
	int GetMouseWheelValue();
}

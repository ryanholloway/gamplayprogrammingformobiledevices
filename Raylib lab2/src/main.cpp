/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

For a C++ project simply rename the file to .cpp and re-run the build script

-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you
--  wrote the original software. If you use this software in a product, an acknowledgment
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

*/

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"// utility header for SearchAndSetResourceDir
#include <iostream>

void drawArray(int count, int size, int arr[])
{
	char textcharArr[4];
	for (int rectangle = 0; rectangle < size; rectangle++)
	{
		Vector2 pos = { 65 * rectangle + 10, 20 };
		if (rectangle < count)
		{
			snprintf(textcharArr, 4, "%d", arr[rectangle]);
			DrawRectangle(pos.x, pos.y, 60, 60, RED);
			DrawText(textcharArr, pos.x + 10, pos.y + 20, 25, WHITE);
		}
		else
			DrawRectangleLines(pos.x, pos.y, 60, 60, BLUE);
	}
}

void insertElement(int item, int size, int arr[], int& count)
{
	if (count < size)
	{
		arr[count] = item;
		count++;
	}
}

int main()
{
	srand(time(NULL));
	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	int count = 4;
	int size = 6;

	int array[6] = { 1,34,156,2 };
	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{

		// drawing
		BeginDrawing();

		// Setup the backbuffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		drawArray(count, size, array);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			insertElement(rand() & 999, size, array, count);
		}

		// draw some text using the default font
		DrawText("Hello Raylib", 200, 200, 20, WHITE);


		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// destory the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
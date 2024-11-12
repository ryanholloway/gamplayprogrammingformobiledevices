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
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <iostream>

int main()
{

	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");
	Vector2 vec1 = { 100,100 };
	Vector2 vec2 = { 200,200 };

	Vector2 mousePos;
	Vector2 objectPos = { 500,500 };



	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	Texture plane = LoadTexture("ship_0002.png");

	Vector2 textureSize = { (float)plane.width, (float)plane.height };

	Rectangle sourceRec = { 0.0f, 0.0f, textureSize.x, textureSize.y };
	Rectangle destRec = { objectPos.x, objectPos.y, textureSize.x, textureSize.y };
	Vector2 origin = { textureSize.x / 2, textureSize.y / 2 };

	float direction;

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{


		// drawing
		BeginDrawing();
		mousePos = GetMousePosition();
		// Setup the backbuffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);
		destRec.x = objectPos.x;
		destRec.y = objectPos.y;

		float angle = atan2(mousePos.y - objectPos.y, mousePos.x - objectPos.x);
		angle *= RAD2DEG;
		angle += 90.0f;


		DrawTexturePro(plane, sourceRec, destRec, origin, angle, WHITE);
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destory the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
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

int main()
{

	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");
	Vector2 pos = { 400,200 };
	Vector2 enemyPos = { 200,400 };
	Vector2 up = { 0,-1 };
	Vector2 down = { 0,1 };
	Vector2 left = { -1,0 };
	Vector2 right = { 1,0 };
	Vector2 speed = { 3,3 };
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{

		// drawing
		BeginDrawing();

		// Setup the backbuffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);



		// draw some text using the default font
		DrawText("Hello Raylib", 200, 200, 20, WHITE);
		Vector2 playerVelocity = { 0,0 };



		if (IsKeyDown(KEY_A)) {
			playerVelocity = left;
		}
		if (IsKeyDown(KEY_W)) {
			playerVelocity = up;
		}
		if (IsKeyDown(KEY_S)) {
			playerVelocity = down;
		}
		if (IsKeyDown(KEY_D)) {
			playerVelocity = right;
		}

		DrawFPS(0, 0);
		DrawCircle(pos.x, pos.y, 50, GREEN);
		DrawCircle(enemyPos.x, enemyPos.y, 50, RED);
		// draw our texture to the screen

		Vector2 dir = Vector2Subtract(pos, enemyPos);
		//float distance = Vector2Length(dir);

		Vector2 normalised = Vector2Normalize(dir);

		playerVelocity = Vector2Multiply(speed, playerVelocity);
		normalised = Vector2Multiply(speed, normalised);

		pos = Vector2Add(pos, playerVelocity);
		enemyPos = Vector2Add(enemyPos, normalised);

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
#include "raylib.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "raygui.h"

#define GRID_SIZE 16      // 16x16 grid
#define CELL_SIZE 30      // Each cell is 30x30 pixels
#define SIDEBAR_WIDTH 150
#define BUTTON_HEIGHT 40

typedef struct PixelData {
	Vector2 position;
	Color color;
}; PixelData;

Color colors[] = { RED, GREEN, BLUE };

void SavePixelArt(const char* filename, PixelData* pixels, int pixelCount) {
	FILE* file = fopen(filename, "wb");
	if (file == NULL) return;

	fwrite(&pixelCount, sizeof(int), 1, file);
	for (int i = 0; i < pixelCount; i++) {
		fwrite(&pixels[i].position, sizeof(Vector2), 1, file);
		fwrite(&pixels[i].color, sizeof(Color), 1, file);
	}

	fclose(file);
}

PixelData* LoadPixelArt(const char* filename, int* pixelCount) {
	FILE* file = fopen(filename, "rb");
	if (file == NULL) return NULL;

	fread(pixelCount, sizeof(int), 1, file);
	PixelData* pixels = (PixelData*)malloc(sizeof(PixelData) * (*pixelCount));

	for (int i = 0; i < *pixelCount; i++) {
		fread(&pixels[i].position, sizeof(Vector2), 1, file);
		fread(&pixels[i].color, sizeof(Color), 1, file);
	}

	fclose(file);
	return pixels;
}

void saveFile(Color  grid[16][16], const char* filename)
{
	int pixelCount = 0;

	// Count non-empty cells
	for (int x = 0; x < GRID_SIZE; x++) {
		for (int y = 0; y < GRID_SIZE; y++) {
			if (grid[x][y].a > 0) { // Only save colored cells
				pixelCount++;
			}
		}
	}

	PixelData* pixels = (PixelData*)malloc(sizeof(PixelData) * pixelCount);
	int index = 0;

	// Store each colored cell's position and color
	for (int x = 0; x < GRID_SIZE; x++) {
		for (int y = 0; y < GRID_SIZE; y++) {
			if (grid[x][y].a > 0) { // Non-empty cell
				Vector2 vec = { x,y };
				pixels[index].position = vec;
				pixels[index].color = grid[x][y];
				index++;
			}
		}
	}

	// Save to file
	SavePixelArt(filename, pixels, pixelCount);
	free(pixels);
}

void loadFile(const char* filename, Color  grid[16][16])
{
	int pixelCount;
	PixelData* loadedPixels = LoadPixelArt(filename, &pixelCount);

	// Reset the grid
	for (int x = 0; x < GRID_SIZE; x++) {
		for (int y = 0; y < GRID_SIZE; y++) {
			grid[x][y] = BLANK; // Clear each cell
		}
	}

	// Populate the grid with loaded pixel data
	for (int i = 0; i < pixelCount; i++) {
		int x = (int)loadedPixels[i].position.x;
		int y = (int)loadedPixels[i].position.y;
		grid[x][y] = loadedPixels[i].color;
	}

	free(loadedPixels);
}

int main()
{
	const char* filename = "pixelart.rch";
	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(GRID_SIZE * CELL_SIZE + SIDEBAR_WIDTH, GRID_SIZE * CELL_SIZE, "Pixel Art Program");
	SetTargetFPS(60);

	Color grid[GRID_SIZE][GRID_SIZE] = { 0 };  // 2D array to store pixel colors
	int selectedColorIndex = 0;                // Start with the first color in palette
	bool clickedSave = false, clickedLoad = false;
	bool clickedColor1 = false, clickedColor2 = false, clickedColor3 = false;
	// Main game loop
	while (!WindowShouldClose()) {
		// Change color with keys 1-3
		if (IsKeyPressed(KEY_ONE)) selectedColorIndex = 0;
		if (IsKeyPressed(KEY_TWO)) selectedColorIndex = 1;
		if (IsKeyPressed(KEY_THREE)) selectedColorIndex = 2;

		// Draw pixel on grid with mouse click
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePos = GetMousePosition();
			int x = mousePos.x / CELL_SIZE;
			int y = mousePos.y / CELL_SIZE;
			if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
				grid[x][y] = colors[selectedColorIndex];
			}
		}

		if (IsKeyPressed(KEY_S)) {
			saveFile(grid, filename);
		}

		if (IsKeyPressed(KEY_L)) {
			loadFile(filename, grid);
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);

		for (int x = 0; x < GRID_SIZE; x++) {
			for (int y = 0; y < GRID_SIZE; y++) {
				DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, grid[x][y]);
				DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GRAY);
			}
		}

		DrawText("Press 1 for RED, 2 for GREEN, 3 for BLUE", 10, 10, 10, BLACK);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
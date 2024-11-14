#include "raylib.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define GRID_SIZE 32      
#define CELL_SIZE 15       
#define SIDEBAR_WIDTH 150
#define BUTTON_HEIGHT 40

typedef struct PixelData {
	Vector2 position;
	Color color;
} PixelData;

// Expanded color palette
Color colors[] = { RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE, DARKBLUE, DARKGREEN, BROWN, PINK, MAROON, BEIGE, SKYBLUE, LIME, VIOLET, WHITE };

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

void saveFile(Color grid[GRID_SIZE][GRID_SIZE], const char* filename) {
	int pixelCount = 0;

	for (int x = 0; x < GRID_SIZE; x++) {
		for (int y = 0; y < GRID_SIZE; y++) {
			if (grid[x][y].a > 0) {
				pixelCount++;
			}
		}
	}

	PixelData* pixels = (PixelData*)malloc(sizeof(PixelData) * pixelCount);
	int index = 0;

	for (int x = 0; x < GRID_SIZE; x++) {
		for (int y = 0; y < GRID_SIZE; y++) {
			if (grid[x][y].a > 0) {
				Vector2 vec = { x, y };
				pixels[index].position = vec;
				pixels[index].color = grid[x][y];
				index++;
			}
		}
	}

	SavePixelArt(filename, pixels, pixelCount);
	free(pixels);
}

void loadFile(const char* filename, Color grid[GRID_SIZE][GRID_SIZE]) {
	int pixelCount;
	PixelData* loadedPixels = LoadPixelArt(filename, &pixelCount);

	for (int x = 0; x < GRID_SIZE; x++) {
		for (int y = 0; y < GRID_SIZE; y++) {
			grid[x][y] = BLANK;
		}
	}

	for (int i = 0; i < pixelCount; i++) {
		int x = (int)loadedPixels[i].position.x;
		int y = (int)loadedPixels[i].position.y;
		grid[x][y] = loadedPixels[i].color;
	}

	free(loadedPixels);
}

void resetGrid(Color grid[GRID_SIZE][GRID_SIZE]) {
	for (int x = 0; x < GRID_SIZE; x++) {
		for (int y = 0; y < GRID_SIZE; y++) {
			grid[x][y] = BLANK;
		}
	}
}

int main() {
	const char* filename = "pixelart.rch";
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(GRID_SIZE * CELL_SIZE + SIDEBAR_WIDTH, GRID_SIZE * CELL_SIZE, "Pixel Art Program");
	SetTargetFPS(60);

	Color grid[GRID_SIZE][GRID_SIZE] = { 0 };
	int selectedColorIndex = 0;

	int colorCount = sizeof(colors) / sizeof(colors[0]);
	int colorsPerColumn = 4;
	int colorButtonSize = 25;

	// Calculate text position based on the color grid height
	int rowsNeeded = (colorCount + colorsPerColumn - 1) / colorsPerColumn; // Rounds up to next row if not a full row
	int sidebarHeight = rowsNeeded * (colorButtonSize + 10);

	while (!WindowShouldClose()) {
		Vector2 mousePos = GetMousePosition();

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			int x = mousePos.x / CELL_SIZE;
			int y = mousePos.y / CELL_SIZE;
			if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
				grid[x][y] = colors[selectedColorIndex];
			}
		}

		for (int i = 0; i < colorCount; i++) {
			int col = i % colorsPerColumn;
			int row = i / colorsPerColumn;
			Rectangle colorRect = { GRID_SIZE * CELL_SIZE + 10 + col * (colorButtonSize + 10), 10 + row * (colorButtonSize + 10), colorButtonSize, colorButtonSize };

			if (CheckCollisionPointRec(mousePos, colorRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				selectedColorIndex = i;
			}
		}

		BeginDrawing();
		ClearBackground(WHITE);

		for (int x = 0; x < GRID_SIZE; x++) {
			for (int y = 0; y < GRID_SIZE; y++) {
				DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, grid[x][y]);
				DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
			}
		}

		for (int i = 0; i < colorCount; i++) {
			int col = i % colorsPerColumn;
			int row = i / colorsPerColumn;
			Rectangle colorRect = { GRID_SIZE * CELL_SIZE + 10 + col * (colorButtonSize + 10), 10 + row * (colorButtonSize + 10), colorButtonSize, colorButtonSize };

			DrawRectangleRec(colorRect, colors[i]);
			if (i == selectedColorIndex) {
				DrawRectangleLinesEx(colorRect, 3, BLACK);
			}
		}

		// GUI Buttons for Save, Load, and Reset
		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 20, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, "Save")) {
			saveFile(grid, filename);
		}

		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 70, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, "Load")) {
			loadFile(filename, grid);
		}

		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10,(float)sidebarHeight + 120, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, "Reset")) {
			resetGrid(grid);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

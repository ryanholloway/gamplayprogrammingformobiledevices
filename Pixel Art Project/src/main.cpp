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
Color lightGray = Color{ 220, 220, 220, 255 }; // Light gray
Color darkGray = Color{ 180, 180, 180, 255 }; // Dark gray

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

Color colourSelector(Color grid[GRID_SIZE][GRID_SIZE], Vector2 mousePos) {
	int x = mousePos.x / CELL_SIZE;
	int y = mousePos.y / CELL_SIZE;

	return grid[x][y];
}

int main() {
	const char* filename = "pixelart.rch";
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(GRID_SIZE * CELL_SIZE + SIDEBAR_WIDTH, GRID_SIZE * CELL_SIZE, "Pixel Art Program");
	SetTargetFPS(60);

	Color grid[GRID_SIZE][GRID_SIZE] = { 0 };
	int selectedColorIndex = 0;
	Color customColor = WHITE;
	int colorCount = sizeof(colors) / sizeof(colors[0]);
	int colorsPerColumn = 4;
	int colorButtonSize = 25;

	// Variables for rectangle tool
	int startX = -1, startY = -1;
	bool isDrawing = false;
	bool rectToolEnabled = false;
	int endX, endY;

	// Calculate text position based on the color grid height
	int rowsNeeded = (colorCount + colorsPerColumn - 1) / colorsPerColumn; // Rounds up to next row if not a full row
	int sidebarHeight = rowsNeeded * (colorButtonSize + 10);
	bool selectingColor = false;
	Color selectedColor;

	while (!WindowShouldClose()) {
		Vector2 mousePos = GetMousePosition();

		// Handle Color Picker
		for (int i = 0; i < colorCount; i++) {
			int col = i % colorsPerColumn;
			int row = i / colorsPerColumn;
			Rectangle colorRect = { GRID_SIZE * CELL_SIZE + 10 + col * (float)(colorButtonSize + 10), 10 + row * (float)(colorButtonSize + 10), colorButtonSize, colorButtonSize };

			if (CheckCollisionPointRec(mousePos, colorRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				selectingColor = false;
				selectedColorIndex = i;

			}
		}

		// Handle Eraser Button
		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 20, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, "Eraser")) {
			selectedColorIndex = -1;
			rectToolEnabled = false;
		}

		// Handle Save, Load, and Reset
		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 70, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, "Save")) {
			saveFile(grid, filename);
		}
		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 120, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, "Load")) {
			loadFile(filename, grid);
		}
		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 170, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, "Reset")) {
			resetGrid(grid);
		}

		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 220, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, "Rectangle")) {
			rectToolEnabled = !rectToolEnabled;
		}


		// Start Rectangle Drawing
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && rectToolEnabled) {
			startX = mousePos.x / CELL_SIZE;
			startY = mousePos.y / CELL_SIZE;
			isDrawing = true;
		}
		else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !rectToolEnabled) {
			int x = mousePos.x / CELL_SIZE;
			int y = mousePos.y / CELL_SIZE;
			if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
				if (selectedColorIndex >= 0) {
					grid[x][y] = colors[selectedColorIndex];
				}
				else {
					grid[x][y] = BLANK; // Erase
				}
			}
		}
		if (isDrawing && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			endX = mousePos.x / CELL_SIZE;
			endY = mousePos.y / CELL_SIZE;
		}

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isDrawing && !selectingColor) {
			isDrawing = false;
			for (int x = std::min(startX, endX); x <= std::max(startX, endX); x++) {
				for (int y = std::min(startY, endY); y <= std::max(startY, endY); y++) {
					if (selectedColorIndex >= 0) {
						selectedColor = colors[selectedColorIndex];
						grid[x][y] = selectedColor;
					}
				}
			}
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			selectingColor = true;
			selectedColor = colourSelector(grid, mousePos);
		}
		if (selectingColor)
		{
			DrawRectangle(GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 270, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT, selectedColor);
			DrawText("Selected Color", GRID_SIZE * CELL_SIZE + 35, (float)sidebarHeight + 270 + BUTTON_HEIGHT / 2.0f - 5, 10, BLACK);
		}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && selectingColor) {
			int x = mousePos.x / CELL_SIZE;
			int y = mousePos.y / CELL_SIZE;
			if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
				if (selectedColorIndex >= 0) {
					grid[x][y] = selectedColor;
				}
				else {
					grid[x][y] = BLANK; // Erase
				}
			}
		}




		// Drawing the grid
		BeginDrawing();
		ClearBackground(WHITE);

		for (int x = 0; x < GRID_SIZE; x++) {
			for (int y = 0; y < GRID_SIZE; y++) {
				// Alternate between gray and white
				Color cellBackground = ((x + y) % 2 == 0) ? lightGray : darkGray;
				DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, cellBackground); // Background
				DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, grid[x][y]); // Color
			}
		}

		DrawText("Pick a color:", GRID_SIZE * CELL_SIZE + 10, sidebarHeight, 20, BLACK);

		// Draw the color grid
		for (int i = 0; i < colorCount; i++) {
			int col = i % colorsPerColumn;
			int row = i / colorsPerColumn;
			Rectangle colorRect = { GRID_SIZE * CELL_SIZE + 10 + col * (colorButtonSize + 10), 10 + row * (colorButtonSize + 10), colorButtonSize, colorButtonSize };

			DrawRectangleRec(colorRect, colors[i]);
			if (i == selectedColorIndex) {
				DrawRectangleLinesEx(colorRect, 3, BLACK);
			}
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

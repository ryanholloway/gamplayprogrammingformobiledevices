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

void saveFile(int grid[GRID_SIZE][GRID_SIZE], const char* filename) {
	FILE* file = fopen(filename, "wb");
	if (!file) return;

	fwrite(grid, sizeof(int), GRID_SIZE * GRID_SIZE, file);
	fclose(file);
}

void loadFile(const char* filename, int grid[GRID_SIZE][GRID_SIZE]) {
	FILE* file = fopen(filename, "rb");
	if (!file) return;

	fread(grid, sizeof(int), GRID_SIZE * GRID_SIZE, file);
	fclose(file);
}

void resetGrid(int grid[GRID_SIZE][GRID_SIZE]) {
	for (int x = 0; x < GRID_SIZE; x++) {
		for (int y = 0; y < GRID_SIZE; y++) {
			grid[x][y] = -1;
		}
	}
}

int colourSelector(int grid[GRID_SIZE][GRID_SIZE], Vector2 mousePos) {
	int x = mousePos.x / CELL_SIZE;
	int y = mousePos.y / CELL_SIZE;
	return grid[x][y];
}

int main() {
	const char* filename = "pixelart.rch";
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(GRID_SIZE * CELL_SIZE + SIDEBAR_WIDTH, GRID_SIZE * CELL_SIZE, "Pixel Art Program");
	SetTargetFPS(155);

	int grid[GRID_SIZE][GRID_SIZE] = { 0 };
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
	resetGrid(grid);
	while (!WindowShouldClose()) {
		Vector2 mousePos = GetMousePosition();

		// Handle Color Picker
		for (int i = 0; i < colorCount; i++) {
			int col = i % colorsPerColumn;
			int row = i / colorsPerColumn;
			Rectangle colorRect = { GRID_SIZE * CELL_SIZE + 10 + col * (float)(colorButtonSize + 10), 10 + row * (float)(colorButtonSize + 10), colorButtonSize, colorButtonSize };

			if (CheckCollisionPointRec(mousePos, colorRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
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
					grid[x][y] = selectedColorIndex;
				}
				else {
					grid[x][y] = -1; // Erase
				}
			}
		}
		if (isDrawing && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			endX = mousePos.x / CELL_SIZE;
			endY = mousePos.y / CELL_SIZE;
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			selectedColorIndex = colourSelector(grid, mousePos);
		}
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isDrawing) {
			isDrawing = false;
			for (int x = std::min(startX, endX); x <= std::max(startX, endX); x++) {
				for (int y = std::min(startY, endY); y <= std::max(startY, endY); y++) {
					if (selectedColorIndex >= 0) {
						grid[x][y] = selectedColorIndex;
					}
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
				switch (grid[x][y])
				{

				}
				DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, colors[grid[x][y]]); // Color
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

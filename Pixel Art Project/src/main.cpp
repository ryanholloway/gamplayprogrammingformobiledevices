
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raylib.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#include "raygui.h"
#include <fstream>
#include <vector>

#define GRID_SIZE 32
#define CELL_SIZE 15
#define SIDEBAR_WIDTH 150
#define BUTTON_HEIGHT 30


typedef struct CopiedRect {
	int width, height;
	std::vector<std::vector<int>> data;
	bool isCopied = false;
};

typedef struct PixelData {
	Vector2 position;
	Color color;
} PixelData;

// Expanded color palette
Color colors[] = { RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE, DARKBLUE, DARKGREEN, BROWN, PINK, MAROON, BEIGE, SKYBLUE, LIME, VIOLET, WHITE };
Color lightGray = Color{ 220, 220, 220, 255 }; // Light gray
Color darkGray = Color{ 180, 180, 180, 255 }; // Dark gray

void saveFile(int grid[GRID_SIZE][GRID_SIZE], const char* filename) {
	std::ofstream file(filename);
	if (!file) return;

	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			file << grid[i][j];
			if (j < GRID_SIZE - 1) file << " ";
		}
		file << "\n";
	}
}

void loadFile(const char* filename, int grid[GRID_SIZE][GRID_SIZE]) {
	std::ifstream file(filename);
	if (!file) return;

	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			file >> grid[i][j];
		}
	}
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

void copyRectangle(int grid[GRID_SIZE][GRID_SIZE], int startX, int startY, int endX, int endY, CopiedRect& copiedRect)
{
	int minX = std::min(startX, endX);
	int minY = std::min(startY, endY);
	int maxX = std::max(startX, endX);
	int maxY = std::max(startY, endY);

	copiedRect.width = maxX - minX + 1;
	copiedRect.height = maxY - minY + 1;
	copiedRect.data.clear();
	copiedRect.data.resize(copiedRect.height, std::vector<int>(copiedRect.width));

	// Copy based on (x, y) grid
	for (int y = 0; y < copiedRect.height; ++y) {
		for (int x = 0; x < copiedRect.width; ++x) {
			copiedRect.data[y][x] = grid[minX + x][minY + y];
		}
	}
	copiedRect.isCopied = true;

}

void pasteRectangle(int grid[GRID_SIZE][GRID_SIZE], int startX, int startY, CopiedRect& copiedRect) {
	if (!copiedRect.isCopied) return;

	// Paste based on (x, y) grid
	for (int y = 0; y < copiedRect.height; ++y) {
		for (int x = 0; x < copiedRect.width; ++x) {
			int targetX = startX + x;
			int targetY = startY + y;

			if (targetX >= 0 && targetX < GRID_SIZE && targetY >= 0 && targetY < GRID_SIZE) {
				grid[targetX][targetY] = copiedRect.data[y][x];
			}
		}
	}
}



int main() {
	const char* filename = "pixelart.rch";
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);
	InitWindow(GRID_SIZE * CELL_SIZE + SIDEBAR_WIDTH, GRID_SIZE * CELL_SIZE, "Pixel Art Program");
	GuiLoadStyleDefault();
	Color background = WHITE;
	int grid[GRID_SIZE][GRID_SIZE] = { 0 };
	int selectedColorIndex = 0;
	Color customColor = WHITE;
	int colorCount = sizeof(colors) / sizeof(colors[0]);
	int colorsPerColumn = 4;
	int colorButtonSize = 25;

	bool darkmode = false;
	bool copying = false;

	// Variables for rectangle tool
	int startX = -1, startY = -1;
	bool isDrawing = false;
	bool rectToolEnabled = false;
	int endX, endY;
	CopiedRect copiedRect;


	// Calculate text position based on the color grid height
	int rowsNeeded = (colorCount + colorsPerColumn - 1) / colorsPerColumn; // Rounds up to next row if not a full row
	int sidebarHeight = rowsNeeded * (colorButtonSize + 10);
	resetGrid(grid);
	while (!WindowShouldClose()) {
		Vector2 mousePos = GetMousePosition();
		int mouseX = (int)mousePos.x / CELL_SIZE;
		int mouseY = (int)mousePos.y / CELL_SIZE;
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
		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 20.0f, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B0, " Eraser"))) {
			selectedColorIndex = -1;
			rectToolEnabled = false;
		}

		// Handle Save, Load, and Reset
		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 55, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B1, " Save"))) {
			saveFile(grid, filename);
		}
		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 90, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B2, " Load"))) {
			loadFile(filename, grid);
		}
		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 125, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B3, " Reset"))) {
			resetGrid(grid);
		}
		//Rectangle Tool
		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 160, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B4, " Rectangle"))) {
			rectToolEnabled = true;
		}
		//Selector tool
		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 195.0f, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B5, " Selector")))
		{
			copying = true;
		}
		//dark mode
		if (GuiButton(Rectangle{ GRID_SIZE * CELL_SIZE + 10, (float)sidebarHeight + 230, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B6, " Dark Mode")))
		{
			darkmode = !darkmode;
		}

		// Start Rectangle Drawing
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && rectToolEnabled) {
			startX = mouseX;
			startY = mouseY;
			isDrawing = true;
		}
		else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&!rectToolEnabled&&copying) {
			startX = mouseX;
			startY = mouseY;
		}
		else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !rectToolEnabled&&!copying) {
			int x = mouseX;
			int y = mouseY;
			if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
				if (selectedColorIndex >= 0) {
					grid[x][y] = selectedColorIndex;
				}
				else {
					grid[x][y] = -1; // Erase
				}
			}
		}
		
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			endX = mouseX;
			endY = mouseY;
		}

		if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_C)) {
			if (startX != -1 && startY != -1 && endX != -1 && endY != -1) {
				copyRectangle(grid, startX, startY, endX, endY, copiedRect);
				copying = false;
				startX = startY = endX = endY = -1;
			}
		}

		if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V)) {
			if (copiedRect.isCopied) {
				pasteRectangle(grid, mouseX, mouseY, copiedRect);
			}
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
			startX = startY = endX = endY = -1;
			rectToolEnabled = false;
		}

		switch (darkmode)
		{
		case true:
			background = BLACK;
			break;
		case false:
			background = RAYWHITE;
			break;
		default:
			background = RAYWHITE;
			break;
		}

		// Drawing the grid
		BeginDrawing();

		ClearBackground(background);

		for (int x = 0; x < GRID_SIZE; x++) {
			for (int y = 0; y < GRID_SIZE; y++) {
				// Alternate between gray and white
				Color cellBackground = ((x + y) % 2 == 0) ? lightGray : darkGray;
				if (grid[x][y] < 0) //dont draw if there is something on the grid
				{
					DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, cellBackground); // Background
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
		if (isDrawing || (startX != -1 && startY != -1 && endX != -1 && endY != -1)) {
			int rectX = std::min(startX, endX) * CELL_SIZE;
			int rectY = std::min(startY, endY) * CELL_SIZE;
			int rectWidth = (std::abs(endX - startX) + 1) * CELL_SIZE;
			int rectHeight = (std::abs(endY - startY) + 1) * CELL_SIZE;

			DrawRectangleLines(rectX, rectY, rectWidth, rectHeight, RED);
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

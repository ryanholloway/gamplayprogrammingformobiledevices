//Ryan Holloway
//C00283423

#define RAYGUI_SUPPORT_ICONS
#define RAYGUI_IMPLEMENTATION

#include "raygui.h"
#include "raylib.h"
#include <iostream>
#include <fstream>
#include <vector>

#define GRID_SIZE 64
#define CELL_SIZE 8
#define SIDEBAR_WIDTH 150
#define BUTTON_HEIGHT 30

const char* filename = "pixelart.rch";

typedef struct CopiedRect {
	int width, height;
	std::vector<std::vector<int>> data;
	bool isCopied = false;
};

// Expanded color palette
Color colors[] = { RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE, DARKBLUE, DARKGREEN, BROWN, PINK, MAROON, BLACK, SKYBLUE, LIME, VIOLET, WHITE };
Color lightGrey = Color{ 220, 220, 220, 255 }; // Light grey
Color darkGrey = Color{ 180, 180, 180, 255 }; // Dark grey

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
	int x = (mousePos.x - SIDEBAR_WIDTH) / CELL_SIZE;
	int y = mousePos.y / CELL_SIZE;
	return grid[x][y];
}

void copyRectangle(int grid[GRID_SIZE][GRID_SIZE], int startX, int startY, int endX, int endY, CopiedRect& copiedRect)
{
	int left = std::min(startX, endX);
	int top = std::min(startY, endY);
	int right = std::max(startX, endX);
	int bottom = std::max(startY, endY);

	copiedRect.width = right - left + 1;
	copiedRect.height = bottom - top + 1;
	copiedRect.data.assign(copiedRect.height, std::vector<int>(copiedRect.width));

	// Copy data from the original grid
	for (int row = 0; row < copiedRect.height; ++row) {
		for (int col = 0; col < copiedRect.width; ++col) {
			copiedRect.data[row][col] = grid[left + col][top + row];
		}
	}
	copiedRect.isCopied = true;

}

void pasteRectangle(int grid[GRID_SIZE][GRID_SIZE], int startX, int startY, CopiedRect& copiedRect) {
	if (!copiedRect.isCopied) return;

	// Paste data back to the grid
	for (int row = 0; row < copiedRect.height; ++row) {
		for (int col = 0; col < copiedRect.width; ++col) {
			int destX = startX + col;
			int destY = startY + row;

			if (destX >= 0 && destX < GRID_SIZE && destY >= 0 && destY < GRID_SIZE) {
				grid[destX][destY] = copiedRect.data[row][col];
			}
		}
	}

}

void drawGrid(int  grid[GRID_SIZE][GRID_SIZE])
{
	for (int x = 0; x < GRID_SIZE; x++) {
		for (int y = 0; y < GRID_SIZE; y++) {
			// Alternate between gray and white
			Color cellBackground = ((x + y) % 2 == 0) ? lightGrey : darkGrey;
			int indexColour = grid[x][y];
			if (indexColour < 0) //dont draw if there is something on the grid
				DrawRectangle(x * CELL_SIZE+SIDEBAR_WIDTH, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, cellBackground); // Background
			else  //error check on grid
				DrawRectangle(x * CELL_SIZE+ SIDEBAR_WIDTH, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, colors[indexColour]);
		}
	}
}


void drawSelectorTool(bool isDrawing, int& startX, int& startY, int& endX, int& endY)
{
	if (isDrawing || (startX != -1 && startY != -1 && endX != -1 && endY != -1)) {
		int left = std::min(startX, endX) * CELL_SIZE+SIDEBAR_WIDTH;
		int top = std::min(startY, endY) * CELL_SIZE;
		int width = (std::abs(endX - startX) + 1) * CELL_SIZE;
		int height = (std::abs(endY - startY) + 1) * CELL_SIZE;

		DrawRectangleLines(left, top, width, height, GRAY); //selector rectangle
	}
}


void drawColourSelector(int colourCount, int colorsPerColumn, int colorButtonSize, int selectedColour)
{
	for (int i = 0; i < colourCount; i++) {
		int col = i % colorsPerColumn;
		int row = i / colorsPerColumn;
		Rectangle colorRect = { 10 + col * (colorButtonSize + 10), 10 + row * (colorButtonSize + 10), colorButtonSize, colorButtonSize };

		DrawRectangleRec(colorRect, colors[i]);

		if (i == selectedColour)
			DrawRectangleLinesEx(colorRect, 3, BLACK);
	}
}

void guiButtons(int sidebarHeight, int& selectedColour, int  grid[GRID_SIZE][GRID_SIZE], const char* filename, bool& rectToolEnabled, bool& copying, bool& darkmode)
{
	// Handle Eraser Button
	if (GuiButton(Rectangle{ 10, (float)sidebarHeight + 20.0f, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B0, " Eraser"))) {
		selectedColour = -1;
	}
	// Handle Save, Load, and Reset
	if (GuiButton(Rectangle{  10, (float)sidebarHeight + 55, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B1, " Save"))) {
		saveFile(grid, filename);
	}
	if (GuiButton(Rectangle{  + 10, (float)sidebarHeight + 90, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B2, " Load"))) {
		loadFile(filename, grid);
	}
	if (GuiButton(Rectangle{  10, (float)sidebarHeight + 125, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B3, " Reset"))) {
		resetGrid(grid);
	}
	//Rectangle Tool
	if (GuiButton(Rectangle{  10, (float)sidebarHeight + 160, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B4, " Rectangle"))) {
		rectToolEnabled = !rectToolEnabled;
	}
	//Selector tool
	if (GuiButton(Rectangle{  10, (float)sidebarHeight + 195.0f, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B5, " Selector")))
	{
		copying = true;
	}
	//dark mode
	if (GuiButton(Rectangle{  10, (float)sidebarHeight + 230, SIDEBAR_WIDTH - 20, BUTTON_HEIGHT }, GuiIconText(0xE0B6, " Dark Mode")))
	{
		darkmode = !darkmode;
	}
}

int main() {


	SetConfigFlags(FLAG_WINDOW_HIGHDPI);

	InitWindow(GRID_SIZE * CELL_SIZE + SIDEBAR_WIDTH, GRID_SIZE * CELL_SIZE, "Pixel Art Program");

	GuiLoadStyleDefault();

	Color background = WHITE;
	int grid[GRID_SIZE][GRID_SIZE] = { 0 };

	int selectedColour = 0;

	int colourCount = sizeof(colors) / sizeof(colors[0]);
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
	int rowsNeeded = (colourCount + colorsPerColumn - 1) / colorsPerColumn; // Rounds up to next row if not a full row
	int sidebarHeight = rowsNeeded * (colorButtonSize + 10);

	resetGrid(grid);
	while (!WindowShouldClose()) {
		Vector2 mousePos = GetMousePosition();
		int mouseX = ((int)mousePos.x - SIDEBAR_WIDTH) / CELL_SIZE;
		int mouseY = (int)mousePos.y / CELL_SIZE;


		// Handle Color Picker
		for (int i = 0; i < colourCount; i++) {
			int col = i % colorsPerColumn;
			int row = i / colorsPerColumn;
			Rectangle colorRect = { 10 + col * (float)(colorButtonSize + 10), 10 + row * (float)(colorButtonSize + 10), colorButtonSize, colorButtonSize };

			if (CheckCollisionPointRec(mousePos, colorRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				selectedColour = i;
			}
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
		else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !rectToolEnabled && !copying) {
			if (mouseX >= 0 && mouseX < GRID_SIZE && mouseY >= 0 && mouseY < GRID_SIZE) {
				if (selectedColour >= 0) {
					grid[mouseX][mouseY] = selectedColour;
				}
				else {
					grid[mouseX][mouseY] = -1; // Erase
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
			selectedColour = colourSelector(grid, mousePos);
		}
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isDrawing) {
			isDrawing = false;
			for (int x = std::min(startX, endX); x <= std::max(startX, endX); x++) {
				for (int y = std::min(startY, endY); y <= std::max(startY, endY); y++) {
						grid[x][y] = selectedColour;
					
				}
			}			
			startX = startY = endX = endY = -1;
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

		drawGrid(grid);
		drawSelectorTool(isDrawing, startX, startY, endX, endY);

		DrawText("Pick a colour:", 8, sidebarHeight, 20, BLACK);
		drawColourSelector(colourCount, colorsPerColumn, colorButtonSize, selectedColour);
		guiButtons(sidebarHeight, selectedColour, grid, filename, rectToolEnabled, copying, darkmode);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}




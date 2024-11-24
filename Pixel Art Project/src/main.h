#pragma once

void saveFile(Color  grid[16][16], const char* filename);

void loadFile(const char* filename, Color  grid[16][16]);

void drawGrid(int  grid[32][32]);

void drawSelectorTool(bool isDrawing, int& startX, int& startY, int& endX, int& endY);

void drawColourSelector(int colourCount, int colorsPerColumn, int colorButtonSize, int selectedColour);

void guiButtons(int sidebarHeight, int& selectedColour, int  grid[32][32], const char* filename, bool& rectToolEnabled, bool& copying, bool& darkmode);

#pragma once
#include "pch.h"
#include "include.h"

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color);

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color);

void DrawEspBox2D(Vec2 top, Vec2 bot, int thickness, D3DCOLOR color);

bool DrawEspBox2DWithHpMp(Vec2 top, Vec2 bot, int thickness, D3DCOLOR color, float hpPer, float mpPer, int barThickness);

void DrawEspBox3D(Vec3 top, Vec3 bot, float a, float height3D, int width, int thickness, D3DCOLOR color);

void DrawText(const char* text, float x, float y, D3DCOLOR color);
void DrawText(const char* text, float x, float y, D3DCOLOR color, RECT rect, DWORD fmt = DT_LEFT | DT_NOCLIP);

int GetTextWidth(const char* szText, LPD3DXFONT pFont);

int GetTextHeight(const char* szText, LPD3DXFONT pFont);

void fillargb(int x, int y, int w, int h, D3DCOLOR color);

void DrawBox(IDirect3DDevice9* pDevice, int x, int y, int w, int h, D3DCOLOR col);

std::vector<std::string> split_string(const std::string& str,
	const std::string& delimiter);

bool mouseInBox(RECT rect);

void DrawDownArrow(Vec2 ptB, Vec2 ptT, D3DCOLOR color);
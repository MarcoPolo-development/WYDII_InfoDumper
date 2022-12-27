#include "pch.h"

extern Hack* hack;

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
	D3DRECT rect = { x,y,x + w,y + h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}


void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color) {

	if(!hack->LineL)
		D3DXCreateLine(pDevice, &hack->LineL);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	hack->LineL->SetWidth(thickness);
	hack->LineL->Draw(Line, 2, color);
}

void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color) {
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawDownArrow(Vec2 ptB, Vec2 ptT, D3DCOLOR color) {
	DrawLine(ptB, ptT, 2, color);
	DrawLine(ptB, Vec2{ ptB.x + ABS(ptT.y - ptB.y), ptB.y + ABS(ptT.y - ptB.y) }, 2, color);
	DrawLine(ptB, Vec2{ ptB.x - ABS(ptT.y - ptB.y), ptB.y + ABS(ptT.y - ptB.y) }, 2, color);
}

void DrawEspBox2D(Vec2 top, Vec2 bot, int thickness, D3DCOLOR color) {
	int height = ABS(top.y - bot.y);
	float heightToScreen = (float)height / (float)windowHeight;
	int width = (heightToScreen / 3) * windowWidth;
	Vec2 tl, tr;
	tl.x = top.x - width / 2;
	tr.x = top.x + width / 2;
	tl.y = tr.y = top.y;

	Vec2 bl, br;
	bl.x = bot.x - width / 2;
	br.x = bot.x + width / 2;
	bl.y = br.y = bot.y;


	DrawLine(tl, tr, thickness, color);
	DrawLine(tl, bl, thickness, color);
	DrawLine(bl, br, thickness, color);
	DrawLine(tr, br, thickness, color);
}


bool DrawEspBox2DWithHpMp(Vec2 top, Vec2 bot, int thickness, D3DCOLOR color, float hpPer, float mpPer, int barThickness) {
	RECT rect;
	

	int height = ABS(bot.y - top.y);
	//minimum height is set the same as the player height
	if (height < hack->localEntH) {
		height = hack->localEntH;
		top.y = bot.y - hack->localEntH;
	}
	float heightToScreen = (float)height / (float)windowHeight;
	int width = (heightToScreen / 3) * windowWidth;
	int dX = (bot.x - top.x);
	Vec2 botHp, topHp, botMp, topMp;
	Vec2 tl, tr;
	tl.x = bot.x - width / 2;
	tr.x = bot.x + width / 2;
	tl.y = tr.y = top.y;

	Vec2 bl, br;
	bl.x = bot.x - width / 2;
	br.x = bot.x + width / 2;
	bl.y = br.y = botHp.y = botMp.y = bot.y;
	int hpHeight = height * hpPer;
	int mpHeight = height * mpPer;

	//botHp.y = botMp.y = bot.y;

	botHp.x = br.x + (barThickness * 0.5);
	botMp.x = botHp.x + barThickness;

	topHp.y = br.y - hpHeight;
	topMp.y = br.y - mpHeight;

	topHp.x = tr.x + (barThickness * 0.5); // -(dX * hpPer);
	topMp.x = tr.x + (barThickness * 1.5); // -(dX * hpPer);
	/*topHp.x = top.x - (width / 2) - 2 - (dX * hpPer);
	topMp.x = top.x + (width / 2) + 2 - (dX * mpPer);*/

	rect.left = tl.x;
	rect.top = tl.y;
	rect.right = br.x;
	rect.bottom = br.y;
	
		

	DrawLine(tl, tr, thickness, color);
	DrawLine(tl, bl, thickness, color);
	DrawLine(bl, br, thickness, color);
	DrawLine(tr, br, thickness, color);
	if (hpPer > 0) {
		DrawLine(botHp, { topHp.x, topHp.y + hpHeight }, barThickness, D3DCOLOR_ARGB(255, 153, 153, 153));
		DrawLine(botHp, topHp, barThickness, D3DCOLOR_ARGB(255, 136, 0, 11));
	}
	if (mpPer > 0) {
		DrawLine(botMp, { topMp.x, topMp.y + mpHeight }, barThickness, D3DCOLOR_ARGB(255, 153, 153, 153));
		DrawLine(botMp, topMp, barThickness, D3DCOLOR_ARGB(255, 0, 35, 96));
	}
	if (hack->settings.statusBox) {
		if (mouseInBox(rect))
			return true;
		return false;
	}
	return false;

}


void DrawEspBox3D(Vec3 top, Vec3 bot, float a, float height3D, int width, int thickness, D3DCOLOR color) {
	//wchar_t text_buffer[64] = { 0 }; //temporary buffer
	//swprintf(text_buffer, _countof(text_buffer), L"top: %f %f %f\n", top.x, top.y, top.z); // convert
	//OutputDebugString(text_buffer); // print

	a = (a * 180) / M_PI;
	//height3D = ABS(top.y - bot.y);
	//swprintf(text_buffer, _countof(text_buffer), L"height3D: %d\n", height3D); // convert
	//OutputDebugString(text_buffer); // print
	//not working
	//if (height3D > 4)
	//	height3D = 4;
	//swprintf(text_buffer, _countof(text_buffer), L"hack->localEntH: %d\n", hack->localEntH); // convert
	//OutputDebugString(text_buffer); // print

	//swprintf(text_buffer, _countof(text_buffer), L"height3D: %f\n", height3D); // convert
	//OutputDebugString(text_buffer); // print
	height3D *= 2.0f;

	if (height3D > 4.0f)
		height3D /= 2.0f;

	Vec3 b1, b2, b3, b4, t1, t2, t3, t4;
	b1.y = b2.y = b3.y = b4.y = bot.y;

	b1.x = bot.x + (cos(TORAD(a + 45)) * width);
	b1.z = bot.z + (sin(TORAD(a + 45)) * width);

	b2.x = bot.x + (cos(TORAD(a + 135)) * width);
	b2.z = bot.z + (sin(TORAD(a + 135)) * width);

	b3.x = bot.x + (cos(TORAD(a + 225)) * width);
	b3.z = bot.z + (sin(TORAD(a + 225)) * width);

	b4.x = bot.x + (cos(TORAD(a + 315)) * width);
	b4.z = bot.z + (sin(TORAD(a + 315)) * width);

	t1.x = b1.x;
	t1.z = b1.z;
	t1.y = b1.y + height3D;

	t2.x = b2.x;
	t2.z = b2.z;
	t2.y = b2.y + height3D;

	t3.x = b3.x;
	t3.z = b3.z;
	t3.y = b3.y + height3D;

	t4.x = b4.x;
	t4.z = b4.z;
	t4.y = b4.y + height3D;



	Vec2 b1_2, b2_2, b3_2, b4_2, t1_2, t2_2, t3_2, t4_2;
	if (W2S(b1, b1_2) && W2S(b2, b2_2) && W2S(b3, b3_2) && W2S(b4, b4_2) && W2S(t1, t1_2) && W2S(t2, t2_2) && W2S(t3, t3_2) && W2S(t4, t4_2)){ //

		
		//columns
		DrawLine(t1_2, b1_2, thickness, color);
		DrawLine(t2_2, b2_2, thickness, color);
		DrawLine(t3_2, b3_2, thickness, color);
		DrawLine(t4_2, b4_2, thickness, color);

		//top base
		DrawLine(t1_2, t2_2, thickness, color);
		DrawLine(t2_2, t3_2, thickness, color);
		DrawLine(t3_2, t4_2, thickness, color);
		DrawLine(t4_2, t1_2, thickness, color);

		//bottom base
		DrawLine(b1_2, b2_2, thickness, color);
		DrawLine(b2_2, b3_2, thickness, color);
		DrawLine(b3_2, b4_2, thickness, color);
		DrawLine(b4_2, b1_2, thickness, color);
	}
}

void DrawText(const char* text, float x, float y, D3DCOLOR color) {
	RECT rect;

	if (!hack->FontF)
		D3DXCreateFont(pDevice, 12, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier", &hack->FontF);

	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	SetRect(&rect, x, y, x, y);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}

void DrawText(const char* text, float x, float y, D3DCOLOR color, RECT rect, DWORD fmt) {
	RECT bg;
	if (!hack->FontF)
		D3DXCreateFont(pDevice, 16, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier", &hack->FontF);
	SetRect(&bg, rect.left + 1, rect.top + 1, rect.right + 1, rect.bottom + 1);
	hack->FontF->DrawTextA(NULL, text, -1, &bg, fmt, D3DCOLOR_ARGB(255, 0, 0, 0));

	//SetRect(&rect, x, y, x, y);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, fmt, color);
}

int GetTextWidth(const char* szText, LPD3DXFONT pFont)
{
	RECT rcRect = { 0,0,0,0 };
	if (pFont)
	{
		// calculate required rect
		pFont->DrawText(NULL, (LPCSTR)szText, strlen(szText), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB(0, 0, 0));
	}

	// return width
	return rcRect.right - rcRect.left;
}

int GetTextHeight(const char* szText, LPD3DXFONT pFont)
{
	RECT rcRect = { 0,0,0,0 };
	if (pFont)
	{
		// calculate required rect
		pFont->DrawText(NULL, (LPCSTR)szText, strlen(szText), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB(0, 0, 0));
	}

	// return width
	return rcRect.bottom - rcRect.top;
}

void fillargb(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DXVECTOR2 vLine[2];
	D3DRECT rec = { x, y, x + w, y + h };

	if (!hack->LineP)
		D3DXCreateLine(pDevice, &hack->LineP);

	hack->LineP->SetWidth(w);
	hack->LineP->SetAntialias(false);
	//hack->LineL->SetGLLines(true);

	vLine[0].x = x + w / 2;
	vLine[0].y = y;
	vLine[1].x = x + w / 2;
	vLine[1].y = y + h;
	/*pDevice->SetPixelShader(NULL);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTexture(0, NULL);*/
	hack->LineP->Begin();
	hack->LineP->Draw(vLine, 2, color);
	hack->LineP->End();
}

DWORD _fvf;
void DrawBox(IDirect3DDevice9* pDevice, int x, int y, int w, int h, D3DCOLOR Color)
{
	struct {
		float x, y, z, rhw;
		DWORD dwColor;
	} qV[4] = { { (float)x , (float)(y + h), 0.0f, 0.0f, Color},
				{ (float)x , (float)y , 0.0f, 0.0f, Color},
				{ (float)(x + w), (float)(y + h), 0.0f, 0.0f, Color},
				{ (float)(x + w), (float)y , 0.0f, 0.0f, Color}
					};

	/*pDevice->SetPixelShader(NULL);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTexture(0, NULL);
	HRESULT ret;
	ret = pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, qV, sizeof(qV[0]));*/
	pDevice->GetFVF(&_fvf);
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pDevice->SetPixelShader(NULL);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, qV, sizeof(qV[0]));
	pDevice->SetFVF(_fvf);
}

std::vector<std::string> split_string(const std::string& str,
	const std::string& delimiter)
{
	std::vector<std::string> strings;

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = str.find(delimiter, prev)) != std::string::npos)
	{
		strings.push_back(str.substr(prev, pos - prev));
		prev = pos + 1;
	}

	// To get the last substring (or only, if delimiter is not found)
	strings.push_back(str.substr(prev));

	return strings;
}


bool mouseInBox(RECT rect) {
	POINT cursorPos;
	GetPhysicalCursorPos(&cursorPos);
	// GetCursorPos(&cursorPos);
	if (ScreenToClient(GetProcessWindow(), &cursorPos)) {
		if (!PtInRect(&rect, cursorPos))
			return false;
		return true;
	}
	return false;
}
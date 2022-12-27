// dllmain.cpp : Defines the entry point for the DLL application.

#include "pch.h"
#include "include.h"
#include "Vars.h"


using namespace std;




#pragma region Decl
DWORD __stdcall D3DThread(LPVOID lpParameter);
//DWORD __stdcall pigThread(LPVOID lpParameter);
DWORD __stdcall EjectThread(LPVOID lpParameter);

#pragma endregion

#pragma region Functions
//hot key info
void HotKeyInfo() {
    cout
        << dye::aqua_on_grey("[Hotkeys]") << "\n"
        << dye::aqua("[Ctrl][End]") << "-- " << "Eject Dll" << "\n"
        << dye::aqua("[Ctrl][Home]") << "- " << "Auto Pig farm" << "\n"
        << dye::aqua("[PgDn]") << "------- " << "Drop waypoint" << "\n"
        << dye::aqua("[Del]") << "-------- " << "Delete last waypoint from cache" << "\n"
        << dye::aqua("[Ins]") << "-------- " << "Save and clear waypoint cache" << "\n"
        << dye::aqua("[Num0]") << "------- " << "Change Targets" << "\n"
        << dye::aqua("[Num1]") << "------- " << "Dump itemlist to file" << "\n"
        << dye::aqua("[Num2]") << "------- " << "Save Current Map PF Data" << "\n"
        << dye::aqua("[Num3]") << "------- " << "Path to nearest target once" << "\n"
        << dye::aqua("[Num4]") << "------- " << "Path to nearest target loop" << "\n"
        << dye::aqua("[Num5]") << "------- " << "Toggle Packet Printing" << "\n"
        << dye::aqua("[Num6]") << "------- " << "Enter coords for bin2map" << "\n"
        << dye::aqua("[Num7]") << "------- " << "Test PF Data" << "\n"
        << dye::aqua("[Num8]") << "------- " << "Print Entity list" << "\n"
        << dye::aqua("[Num9]") << "------- " << "Goto Saved Target Coords" << "\n"
        << dye::aqua("[Num*]") << "------- " << "Test D3D" << "\n"
        << endl;
}
//hot keys
void HotKeys() {
#pragma region Numpad
    //Numpad 0
    if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000) {
        cout
            << "[Info]Current target(s): ";
        copy(targets.begin(), targets.end(), ostream_iterator<string>(cout, ","));
        cout
            << "\n[Prompt]Enter Target(s)(name[,name,name](...)) :" << endl;
        string s;
        getline(cin, s);
        if (!s.empty()) {
            boost::algorithm::split(targets, s, boost::is_any_of(","));
            cout << "Target list updated to: ";
            copy(targets.begin(), targets.end(), ostream_iterator<string>(cout, ","));
            cout << endl;
        }
        else
        {
            cout << "Target list unchanged" << endl;
        }
    }
    //Numpad 1
    if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) {
        dumpItemList((uintptr_t)(moduleBase + itemlistoffset), true);
    }    
    //Numpad 2
    if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) {
        manualDump = true;
        saveMap();
    }
    //Numpad 3
    if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000) {
        MoveToTargets(1);
    }
    //Numpad 4
    if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000) {
        MoveToTargets();
     }
    //Numpad 5
    if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000) {
        logPackets = !logPackets;
        if (logPackets)
            cout << "Packet Printing Enabled" << endl;
        else
            cout << "Packet Printing Disabled" << endl;
    }
    //Numpad 6
    if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000) {
        cout
            << "[Prompt]Enter Target Coords(XXXXYYYY) :" << endl;
        string s;
        getline(cin, s);
        if (!s.empty()) {
            boost::regex expr{ "^([0-4][0-9]{3}[0-4][0-9]{3})$" };
            boost::smatch what;
            if (!boost::regex_search(s, what, expr)) {
                cout << "Invalid Coordinates" << endl;
            }
            else
            {
                saveMap(s, false, loadBinToPathFs(s));
            }
        }
        else
        {
            cout << "Target coords unchanged" << endl;
        }

    }
    //Numpad 7
    if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000) {


        cout
            << "[Prompt]Enter PF Coords(XXXX,YYYY) :" << endl;
        string s;
        getline(cin, s);
        if (!s.empty()) {
            vector<string> t;
            boost::algorithm::split(t, s, boost::is_any_of(","));
            cout << "Target coords updated to: ";
            copy(t.begin(), t.end(), ostream_iterator<string>(cout, ","));
            cout << endl;   
            boost::regex expr{ "^([0-4][0-9]{3},[0-4][0-9]{3})$" };
            boost::smatch what;
            if (!boost::regex_search(s, what, expr)) {
                cout << "Invalid Coordinates" << endl;
            }
            else
            {
                Vec2Int c = Vec2Int{ stoi(t[0]),stoi(t[1]) };
                cNode* t = NodeFromWorldPos(c);
                char* b = GetPFdata(Vec2Int{t->x,t->x});
                cout << "Coords: " << c.x << " " << c.y << " PF: " << hex << int(b) << dec << endl;
            }
        }
        else
        {
            cout << "No coords provided" << endl;
        }
    }
    //Numpad 8
    if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000) {
        BuildEntList();
        cout << "EntList Size: " << entList->ents.size() << "\n";
        for (auto e : entList->ents) {
            cout
                << "Ent Name: [" << e->entName.name << "]" << "\n"
                << "Ent Level: " << e->Level << " Ent ID: " << e->entId << "\n"
                << "Ent Pos: X - " << e->vecOriginX << " Y - " << e->vecOriginZ << "\n"
                << "Ent Distance to Player: " << e->distance() << endl;
        }


        /*for (int i = 0; i < entList->ents.size(); i++) {
            cout
                << "Ent Name: [" << entList->ents[i]->entName.name << "]" << "\n"
                << "Ent Level: " << entList->ents[i]->Level << " Ent ID: " << entList->ents[i]->entId << "\n"
                << "Ent Pos: X - " << entList->ents[i]->vecOriginX << " Y - " << entList->ents[i]->vecOriginZ << "\n"
                << "Ent Distance to Player: " << entList->ents[i]->distance() << endl;
        }*/
    }
    //Numpad 9
    if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000) {
        walkPath(ParsePath(FindAstarPath(getXYVecInt(), Vec2Int{2113,2053})), false, Vec2Int{ 2113,2053 });
    }
    //Numpad *
    if (GetAsyncKeyState(VK_MULTIPLY) & 0x8000) {
        bD3dDisabled = !bD3dDisabled;
        std::cout << (bD3dDisabled ? "D3d Options Toggled Off" : "D3d Options Toggled On") << std::endl;

        if (bD3dDisabled) {
            //pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); // wireframe mode
            pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
            //pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
            //pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        }
        else {
            //pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
            pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
            //pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
            //pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        }
    }
#pragma endregion

#pragma region Other
    // Page Down
    if (GetAsyncKeyState(VK_NEXT) & 0x8000) {
        logXY();
        cout
            << "Waypoint Created: " << vWPBuffer.back().x << "," << vWPBuffer.back().y
            << " Cached: " << vWPBuffer.size() << endl;
    }
    // Delete
    if (GetAsyncKeyState(VK_DELETE) & 0x8000) { // Remove waypoint
        if (vWPBuffer.size() > 0) {

            cout << "Waypoint Deleted: " << vWPBuffer.back().x << "," << vWPBuffer.back().y << endl;
            vWPBuffer.pop_back();
        }

    }
    // Insert
    if (GetAsyncKeyState(VK_INSERT) & 0x8000) { // Save wp cache and clear
        saveWPVector();
    }
#pragma endregion

#pragma region Key Combos

    // Ctrl + Home
    if (GetAsyncKeyState(VK_HOME) & GetAsyncKeyState(VK_CONTROL) & 0x8000) {
        if (getEntIdByName((char*)"Farche") == 0) {
            cout << "Farche not Detected" << endl;
        }
        else {
            autopig = !autopig;
           // if (autopig) {
            //    CreateThread(0, 0, pigThread, 0, 0, 0);
            //    cout << "AutoPig Enabled" << endl;
            //}
           // else {
           //     cout << "AutoPig disabled" << endl;
           // }
        }

    }

#pragma endregion
}
//execute every loop
void EveryLoop() {

    if(savemapbin) {
        saveMapBin();
    }

    if (outputNear) {
        if (tgtList->ents.size() > 0) {
            cout
                << "Nearest Target: " << tgtList->ents.front()->entName.name << "\n"
                << "Target Distance: " << tgtList->ents.front()->distance() << "\n"
                << "Target Direction: " << GetDirection(getXYVec(), Vec2{ tgtList->ents.front()->vecOriginX, tgtList->ents.front()->vecOriginZ }) << endl;
        }
        else {
            cout << "No target in range" << endl;
        }
    }
}
//setup
void Setup() {
    AllocConsole();
    freopen_s(&fo, "CONOUT$", "w", stdout);
    freopen_s(&fi, "CONIN$", "r", stdin);
    freopen_s(&fe, "CONOUT$", "w", stderr);
    SigScan ss;
    entList = new EntList();
    entListSorted = new EntList();
    tgtList = new TgtList();
    moduleBase = (uintptr_t)GetModuleHandle("WYD.exe");
    itlist_addr = (DWORD)(moduleBase + itemlistoffset);
    base_addr1 = (DWORD)(moduleBase + playerinfo_offset);
    base_addr2 = (DWORD)(moduleBase + pObj_offset);
    sendpacket_addr = ss.FindPattern(exename, sendPacketSig, sendPacketMask);
    if (sendpacket_addr == NULL)
        cout << dye::red("Sendpacket addr not found.") << endl;
    buildregion_addr = ss.FindPattern(exename, buildRegionSig, buildRegionMask);
    if (buildregion_addr == NULL)
        cout << dye::red("Build region addr not found.") << endl;
    receivePacket_addr = ss.FindPattern(exename, receivePacketsSig, receivePacketsMask);
    if (receivePacket_addr == NULL)
        cout << dye::red("receivePacket addr not found.") << endl;
    TMHumanBase = GetTMBase();
    moveXaddr = mem::FindDMAAddy(moduleBase + playerinfo_offset, { moveX_offset });
    moveYaddr = mem::FindDMAAddy(moduleBase + playerinfo_offset, { moveY_offset });
    lastEntIdx = mem::FindDMAAddy(moduleBase + playerinfo_offset, { 0x5C, 0x4, 0x10 });
    pfSubGrid_addr = mem::FindDMAAddy(moduleBase + playerinfo_offset, { 0xB8 });
    classTMHuman = GetTMClass();
    pfAddr = (uintptr_t)(*(uintptr_t**)base_addr1) + 0xD4;
    pfAllAddr = (DWORD)(moduleBase + pfAll_offset);
    pfXInit_addr = (DWORD)(moduleBase + pfXInit_offset);
    pfYInit_addr = (DWORD)(moduleBase + pfYInit_offset);
    bMoving_addr = mem::FindDMAAddy(moduleBase + playerinfo_offset, { 0x5C, 0x20C });
    hack = new Hack();
    bool* bMoving = (bool*)bMoving_addr;
    current_region = GetPfRegion();
    BuildEntList();
    populateAStarNodes();
    CreateThread(0, 0, D3DThread, 0, 0, 0);

}
// Startup Info
void PrintStartup() {
    cout
        << "Initialized" << "\n" << hue::aqua 
        << "Itemlist Addr: 0x" << hex << (uintptr_t)(moduleBase + itemlistoffset) << dec << "\n"
        << "Sendpacket Addr: 0x" << hex << sendpacket_addr << dec << "\n"
        << "Build Region Addr: 0x" << hex << sendpacket_addr << dec << "\n"
        << "MoveX Addr: 0x" << hex << moveXaddr << dec << "\n"
        << "MoveY Addr: 0x" << hex << moveYaddr << dec << "\n"
        << "Client ID: " << hex << GetClientID() << dec << "\n"
        << "TMHumanBase: 0x" << hex << GetTMBase() << dec << "\n"
        << "Pathfinding Addr: 0x" << hex << pfAddr << dec << "\n"
        << "TMHumanClass: 0x" << hex << GetTMClass() << dec << '\n'
        << "lastEntIdx: 0x" << hex << lastEntIdx << dec << '\n'
        << "receivePacket_addr: 0x" << hex << receivePacket_addr << dec << '\n'        
        << "pfSubGrid_addr: 0x" << hex << pfSubGrid_addr << hue::reset << dec << '\n';


    cout << endl;

    if (entScan) {
        CreateThread(0, 0, EntThread, 0, 0, 0);
        cout << "EntScan Thread Started" << endl;
    }

    if (tgtScan) {
        // CreateThread(0, 0, TgtThread, 0, 0, 0);
        cout << "TgtScan Enabled" << endl;
    }
}
//PixelGameEngine
void PGE() {

    //Example demo;
    //if (demo.Construct(256, 240, 4, 4))
    //    demo.Start();
}


//non d3d hooks
void Hooks() {
    pSendPacket = (SendPacketT)DetourFunction((PBYTE)sendpacket_addr, (PBYTE)hSendPacket);
  //  pRecvPacket = (RecvPacketT)DetourFunction((PBYTE)receivePacket_addr, (PBYTE)hRecvPacket);
    pBuildRegion = (BuildRegionT)DetourFunction((PBYTE)buildregion_addr, (PBYTE)hBuildRegion);
}
//cleanup
void CleanUp() {
    
    entScan = false;
    bD3dloop = false;
    Sleep(100);
    if (d3dhooked) {
        hack->Init();
        while (!GetAsyncKeyState(VK_END)) {
            hack->Update();
        }
        //unhook
        try {
            Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);
            OutputDebugString("Unhooked\n");
        }
        catch (char* e) {
            printf("Exception Caught: %s\n", e);
        }

        Sleep(1000);
    }
    DetourRemove((PBYTE)pSendPacket, (PBYTE)hSendPacket);
    DetourRemove((PBYTE)pBuildRegion, (PBYTE)hBuildRegion);
    //DetourRemove((PBYTE)pRecvPacket, (PBYTE)hRecvPacket);
    fclose(fe);
    fclose(fi);
    fclose(fo);
    FreeConsole();
    CreateThread(0, 0, EjectThread, 0, 0, 0);
}

#pragma endregion

#pragma region Threads
// Util Threads
DWORD __stdcall EjectThread(LPVOID lpParameter) {
    Sleep(100);
    FreeLibraryAndExitThread(myhModule, 0);
}

//hooked endscene


//thread for d3d updater
DWORD __stdcall D3DThread(LPVOID lpParameter) {
    Sleep(100);
    hack->moduleBase = moduleBase;
    hack->TMHumanBase = GetTMBase();
    int attempts = 0;
    while (attempts < 5)
    {
        if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
            memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);
            cout << "Hooked D3D\n" << endl;
            oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
            d3dhooked = true;
            break;
        }
        ++attempts;
    }
    if (d3dhooked) {
        hack->Init();
        while (bD3dloop) {
            if (!bD3dloop)
                break;
            hack->Update();
            Sleep(5);
        }
    }
    if (!d3dhooked)
        cout << "Hook failed.\n" << endl;
    return 0;
}

//main
DWORD WINAPI MainThread() {
    Setup();
    if(printStartup)
        PrintStartup();
    HotKeyInfo();

    Hooks();

    //loop
    while (1)
    {
        if (GetAsyncKeyState(VK_END) & GetAsyncKeyState(VK_CONTROL) & 0x8000) break;

        HotKeys();

        EveryLoop();

        Sleep(100);
    }



    CleanUp();

    return 0;
}
#pragma endregion

#pragma region DllMain
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        myhModule = hModule;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#pragma endregion
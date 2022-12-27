#include "pch.h"
#include "include.h"



#pragma region Packets Working
void pktUseNPC(int npcid, bool test) {
	p28B sendData;
	ZeroMemory(&sendData, sizeof(sendData));
	sendData.Header.packetSize = sizeof(p28B);
	sendData.Header.packetID = 0x28B;
	sendData.Header.referID = GetClientID();
	sendData.npcid = npcid;
	if (!test) {
		hSendPacket((char*)&sendData, sizeof(p28B));
		std::cout << "Open NPC Sent" << std::endl;
		return;
	}

	BYTE b[sizeof(sendData)];
	memcpy(&b, &sendData, sizeof(sendData));
	std::cout << std::hex << "Size: " << sizeof(sendData) << std::dec << std::endl;

	for (int i = 0; i < sizeof(b) / sizeof(b[0]); i++) {
		std::cout << std::hex << "0x" << int(b[i]) << ' ';
	}
	std::cout << std::dec << std::endl;
}
void pktMoveItem(int srcSlot, int dstSlot, bool test) {
	p376 sendData;
	ZeroMemory(&sendData, sizeof(sendData));
	sendData.Header.packetSize = sizeof(p376);
	sendData.Header.packetID = 0x376;
	sendData.Header.referID = GetClientID();
	sendData.DstType = sendData.SrcType = 1;
	sendData.SrcSlot = srcSlot;
	sendData.DstSlot = dstSlot;
	if (!test) {
		hSendPacket((char*)&sendData, sizeof(p376));
		std::cout << "Move Item Sent" << std::endl;
		return;
	}
	BYTE b[sizeof(sendData)];
	memcpy(&b, &sendData, sizeof(sendData));
	std::cout << std::hex << "Size: " << sizeof(sendData) << std::dec << std::endl;

	for (int i = 0; i < sizeof(b) / sizeof(b[0]); i++) {
		std::cout << std::hex << "0x" << int(b[i]) << ' ';
	}
	std::cout << std::dec << std::endl;
}
void pktBuyItem(int npcid, int slotid, bool test) { // Compra algum item, em algum npc
	p379 sendData;
	ZeroMemory(&sendData, sizeof(sendData));
	sendData.Header.packetSize = sizeof(p379);
	sendData.Header.packetID = 0x379;
	sendData.Header.referID = GetClientID();
	for (int i = 0; i < GetLimitSlot(); i++) {
		if (GetInvSlot(i) == 0) {
			sendData.invSlot = i;
			break;
		}
	}
	sendData.mobID = npcid;
	sendData.sellSlot = slotid;
	if (!test) {
		hSendPacket((char*)&sendData, sizeof(p379));
		std::cout << "Buy Item Sent" << std::endl;
		return;
	}
	std::vector<BYTE> vecData(sizeof(sendData));
	memcpy(vecData.data(), &sendData, sizeof(sendData));
	std::cout << "Size: " << std::hex << sizeof(sendData) << " OP code: 0x" << sendData.Header.packetID << std::dec << std::endl;

	std::cout << "Header: " << std::hex;

	int c = 0;
	for (BYTE i : vecData) {
		if (c == sizeof(packetHeader))
			std::cout << std::endl << "Struct: ";
		char n[10];
		sprintf_s(n, "%02x", (unsigned char)i);
		std::cout << n << ' ';
		c++;
	}
	std::cout << std::dec << std::endl;
}
void pktSellItem(int npcid, int item, bool test, int slotid) { // Vende algum item, em algum npc
	p37A sendData;
	ZeroMemory(&sendData, sizeof(sendData));
	sendData.Header.packetSize = sizeof(p37A);
	sendData.Header.packetID = 0x37A;
	sendData.Header.referID = GetClientID();
	sendData.npcId = npcid;
	sendData.type = 1;
	sendData.sellSlot = slotid;
	if (slotid < 0) {
		for (int i = 0; i < GetLimitSlot(); i++) {
			if (GetInvSlot(i) == item) {
				sendData.sellSlot = i;
				break;
			}
		}
	}
	if (!test) {
		hSendPacket((char*)&sendData, sizeof(p37A));
		std::cout << "Sell Item Sent" << std::endl;
		return;
	}
	std::vector<BYTE> vecData(sizeof(sendData));
	memcpy(vecData.data(), &sendData, sizeof(sendData));
	std::cout << "Size: " << std::hex << sizeof(sendData) << " OP code: 0x" << sendData.Header.packetID << std::dec << std::endl;

	std::cout << "Header: " << std::hex;

	int c = 0;
	for (BYTE i : vecData) {
		if (c == sizeof(packetHeader))
			std::cout << std::endl << "Struct: ";
		char n[10];
		sprintf_s(n, "%02x", (unsigned char)i);
		std::cout << n << ' ';
		c++;
	}
	std::cout << std::dec << std::endl;
}
#pragma endregion

#pragma region Packets Testing
void pktDeleteItem(int slot) {
	p2E4 sendData;
	ZeroMemory(&sendData, sizeof(sendData));
	sendData.Header.packetSize = sizeof(p2E4);
	sendData.Header.packetID = 0x2E4;
	sendData.Header.referID = GetClientID();
	sendData.item = GetInvSlot(slot);
	sendData.slot = slot;
	//hSendPacket((char*)&sendData, sizeof(p2E4));
	BYTE b[sizeof(sendData)];
	memcpy(&b, &sendData, sizeof(sendData));
	std::cout << std::hex << "Size: " << sizeof(sendData) << std::dec << std::endl;

	for (int i = 0; i < sizeof(b) / sizeof(b[0]); i++) {
		std::cout << std::hex << "0x" << int(b[i]) << ' ';
	}
	std::cout << std::dec << std::endl;

}
void pktUseItem(int slot) { // Usa um item consumível
	p373 sendData;
	ZeroMemory(&sendData, sizeof(sendData));
	sendData.Header.packetSize = sizeof(p373);
	sendData.Header.packetID = 0x373;
	sendData.Header.referID = GetClientID();
	sendData.SrcType = sendData.DstType = 1;
	sendData.DstSlot = sendData.SrcSlot = slot;
	sendData.PosX = GetPosX();
	sendData.PosY = GetPosY();
	//hSendPacket((char*)&sendData, sizeof(p373));
	BYTE b[sizeof(sendData)];
	memcpy(&b, &sendData, sizeof(sendData));
	std::cout << std::hex << "Size: " << sizeof(sendData) << std::dec << std::endl;

	for (int i = 0; i < sizeof(b) / sizeof(b[0]); i++) {
		std::cout << std::hex << "0x" << int(b[i]) << ' ';
	}
	std::cout << std::dec << std::endl;
}
void pktSplitItem(int slot, int itemid, int qty, bool test) {
	p2E5 sendData;
	ZeroMemory(&sendData, sizeof(sendData));
	sendData.Header.packetSize = sizeof(p2E5);
	sendData.Header.packetID = 0x2E5;
	sendData.Header.referID = GetClientID();
	sendData.slot = slot;
	sendData.itemid = itemid;
	sendData.splitqt = qty;
	if (!test) {
		hSendPacket((char*)&sendData, sizeof(p2E5));
		std::cout << "Split Item Sent" << std::endl;
		return;
	}
	std::vector<BYTE> vecData(sizeof(sendData));
	memcpy(vecData.data(), &sendData, sizeof(sendData));
	std::cout << "Size: " << std::hex << sizeof(sendData) << " OP code: 0x" << sendData.Header.packetID << std::dec << std::endl;

	std::cout << "Header: " << std::hex;

	int c = 0;
	for (BYTE i : vecData) {
		if (c == sizeof(packetHeader))
			std::cout << std::endl << "Struct: ";
		char n[10];
		sprintf_s(n, "%02x", (unsigned char)i);
		std::cout << n << ' ';
		c++;
	}
	std::cout << std::dec << std::endl;
}
#pragma endregion
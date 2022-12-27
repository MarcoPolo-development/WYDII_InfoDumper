#pragma once


void pktMoveItem(int srcSlot, int dstSlot, bool test = true);
void pktBuyItem(int npcid, int slotid, bool test = true);
void pktSellItem(int npcid, int item, bool test = true, int slotid = -1);
void groupItems(int slot1, int slot2);
void pktDeleteItem(int slot);
void pktOpenNPC(int npcid, bool test = true);
void pktUseItem(int slot);
void pktSplitItem(int slot, int itemid, int qty, bool test = true);
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include "MemMan.h"

MemMan MemClass;

struct offset
{
	DWORD localPlayer = 0xC648AC;
	DWORD forceAttack = 0x3082DEC;
	DWORD entityList = 0x4C41704;
	DWORD crosshair = 0xB2DC;
	DWORD team = 0xF0;
	DWORD health = 0xFC;
}offset;

struct variables
{
	DWORD localPlayer;
	DWORD gameModule;
	int playersTeam;
}var;

void shoot()
{
	MemClass.writeMem<int>(var.gameModule + offset.forceAttack, 5);
	Sleep(20);
	MemClass.writeMem<int>(var.gameModule + offset.forceAttack, 4);
}

bool checkTrigger()
{
	int crosshair = MemClass.readMem<int>(var.localPlayer + offset.crosshair);
	if (crosshair > 0 && crosshair < 64)
	{
		DWORD enemy = MemClass.readMem<DWORD>(var.gameModule + offset.entityList + ((crosshair - 1) * 0x10));
		int enemyTeam = MemClass.readMem<int>(enemy + offset.team);
		int enemyHealth = MemClass.readMem<int>(enemy + offset.health);

		if (enemyTeam != var.playersTeam)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void handleTrigger()
{
	if (checkTrigger())
	{
		shoot();
	}
}

int main()
{
	bool triggerOn = false;
	int gameProc = MemClass.getProcess("csgo.exe");
	var.gameModule = MemClass.getModule(gameProc, "client_panorama.dll");
	var.localPlayer = MemClass.readMem<DWORD>(var.gameModule + offset.localPlayer);

	//store localplayer adress if found
	while (var.localPlayer == NULL)
	{
		var.localPlayer = MemClass.readMem<DWORD>(var.gameModule + offset.localPlayer);
		Sleep(10);
	}

	while (true)
	{
		if (GetAsyncKeyState(VK_F2) & 1)
		{
			var.playersTeam = MemClass.readMem<int>(var.localPlayer + offset.team);
			triggerOn = !triggerOn;
		}

		if (triggerOn)
		{
			handleTrigger();
		}
	}

}

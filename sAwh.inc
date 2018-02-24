/*

* Anti Weapons Hack
* By Sunehildeep
* V1.0

*/

#if defined ACWEAPS_Included
	#endinput
#endif

#define ACWEAPS_Included
#include <a_samp>

#if !defined FILTERSCRIPT
/*======================================================
  * INCLUDE FUNCTIONS
========================================================
Only one function:
- OnPlayerWeaponsHack(playerid, weaponid, ammo, type)
 
Types:

1 - Weapons Hack
2 - Ammo Hack
3 - Different Weapon In Another Slot (I highly recommend to timeout player for this type as this can also be caused for being desycned too

*/


/*======================================================
  * DEFINES
========================================================*/
#define wIsPlayerPaused(%1) GetTickCount() > (weaponsInfo[%1][wTick]+2000)

/*======================================================
  * VARIABLES
========================================================*/
enum wInfo
{
	Weapons[53],
	Slot[13],
	Ammo[53],
	bool:wImmune,
	wTick,
	bool:wisSpawned
};

new weaponsInfo[MAX_PLAYERS][wInfo];
new wtimer[MAX_PLAYERS];

/*======================================================
  * FORWARDING HOOKS
========================================================*/
#if defined OnPlayerConnectWeapons
forward OnPlayerConnectWeapons(playerid);
#endif

#if defined OnPlayerDeathWeapons
forward OnPlayerDeathWeapons(playerid, killerid, reason);
#endif

#if defined OnPlayerDisconnectWeapons
forward OnPlayerDisconnectWeapons(playerid, reason);
#endif

#if defined OnPlayerWeaponsHack
forward OnPlayerWeaponsHack(playerid, weaponid, ammo, type);
#endif

#if defined OnPlayerUpdateWeapons
forward OnPlayerUpdateWeapons(playerid);
#endif

#if defined OnPlayerSpawnWeapons
forward OnPlayerSpawnWeapons(playerid);
#endif

#if defined OnPlayerWeaponShotWeapons
forward OnPlayerWeaponShotWeapons(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

#if defined OnPlayerEnterVehicleWeapons
forward OnPlayerEnterVehicleWeapons(playerid, vehicleid, ispassenger);
#endif

#if defined OnPlayerExitVehicleWeapons
forward OnPlayerExitVehicleWeapons(playerid, vehicleid);
#endif

/*======================================================
  * TIMERS
========================================================*/
forward wResetImmune(playerid);
public wResetImmune(playerid)
{
    weaponsInfo[playerid][wImmune] = false;
    return 1;
}

forward CheckWeapons(playerid);
public CheckWeapons(playerid)
{
	if(wIsPlayerPaused(playerid) || !weaponsInfo[playerid][wisSpawned] || GetPlayerState(playerid) != 1) return 1;
	if(!weaponsInfo[playerid][wImmune])
	{
	    new id = GetPlayerWeapon(playerid);
	    if(id == 46 || id == 40 || id == 0) return 1;
	    new slot = GetWeaponSlot(id);
	    new ammo = GetPlayerAmmo(playerid);
	    
		if(weaponsInfo[playerid][Weapons][id] == 0 )
		{
		    #if defined OnPlayerWeaponsHack
			OnPlayerWeaponsHack(playerid, id, ammo, 1);
			#endif
		}
		else if(weaponsInfo[playerid][Ammo][id] < ammo)
		{
		    #if defined OnPlayerWeaponsHack
			OnPlayerWeaponsHack(playerid, id, ammo, 2);
			#endif
		}
		else if(weaponsInfo[playerid][Slot][slot] != id)
		{
            #if defined OnPlayerWeaponsHack
			OnPlayerWeaponsHack(playerid, id, ammo, 3);
			#endif
		}
	}
	return 1;
}

/*======================================================
  * CALLBACKS
========================================================*/
public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if(weaponsInfo[playerid][Ammo] > 0) weaponsInfo[playerid][Ammo][weaponid]--;
	else if(weaponsInfo[playerid][Ammo] <= 0)
	{
	    weaponsInfo[playerid][Ammo] = 0;
	    weaponsInfo[playerid][Slot][GetWeaponSlot(weaponid)] = 0;
	    weaponsInfo[playerid][Weapons][weaponid] = 0;
	}
	#if defined OnPlayerWeaponShotWeapons
        return OnPlayerWeaponShotWeapons(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
    #else
    	return 1;
    #endif
}

public OnPlayerDisconnect(playerid, reason)
{
	KillTimer(wtimer[playerid]);
	#if defined OnPlayerDisconnectWeapons
        return OnPlayerDisconnectWeapons(playerid, reason);
    #else
    	return 1;
    #endif
}

public OnPlayerUpdate(playerid)
{
    weaponsInfo[playerid][wTick] = GetTickCount();
    #if defined OnPlayerUpdateWeapons
        return OnPlayerUpdateWeapons(playerid);
	#else
	    return 1;
	#endif
}

public OnPlayerDeath(playerid, killerid, reason)
{
	wResetVars(playerid);
	#if defined OnPlayerDeathWeapons
        return OnPlayerDeathWeapons(playerid, killerid, reason);
	#else
	    return 1;
	#endif
}

public OnPlayerConnect(playerid)
{
	wResetVars(playerid);
	wtimer[playerid] = SetTimerEx("CheckWeapons", 1000, true, "i", playerid);
	#if defined OnPlayerConnectWeapons
        return OnPlayerConnectWeapons(playerid);
    #else
    	return 1;
    #endif
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if(!weaponsInfo[playerid][wImmune]) SetTimerEx("wResetImmune", 3000, false, "i", playerid);
	weaponsInfo[playerid][wImmune] = true;
	#if defined OnPlayerEnterVehicleWeapons
	    return OnPlayerEnterVehicleWeapons(playerid, vehicleid, ispassenger);
	#else
	    return 1;
	#endif
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
    if(!weaponsInfo[playerid][wImmune]) SetTimerEx("wResetImmune", 3000, false, "i", playerid);
	weaponsInfo[playerid][wImmune] = true;
    #if defined OnPlayerExitVehicleWeapons
	    return OnPlayerExitVehicleWeapons(playerid, vehicleid);
	#else
	    return 1;
	#endif
}

public OnPlayerSpawn(playerid)
{
	weaponsInfo[playerid][wisSpawned] = true;
	#if defined OnPlayerSpawnWeapons
	    return OnPlayerSpawnWeapons(playerid);
	#else
	    return 1;
	#endif
}

/*======================================================
  * STOCKS
========================================================*/
stock wResetVars(playerid)
{
    for(new i = 0; i < 53; i++) weaponsInfo[playerid][Weapons][i] = 0, weaponsInfo[playerid][Ammo][i] = 0;
	for(new i = 0; i < 13; i++) weaponsInfo[playerid][Slot][i] = 0;
	weaponsInfo[playerid][wImmune] = false;
	weaponsInfo[playerid][wisSpawned] = false;
}

stock wGivePlayerWeapon(playerid, weapon, ammo)
{
	weaponsInfo[playerid][Weapons][weapon] = 1;
	weaponsInfo[playerid][Slot][GetWeaponSlot(weapon)] = weapon;
	weaponsInfo[playerid][Ammo][weapon] = ammo;
	if(!weaponsInfo[playerid][wImmune]) SetTimerEx("wResetImmune", 3000, false, "i", playerid);
	weaponsInfo[playerid][wImmune] = true;
	return GivePlayerWeapon(playerid, weapon, ammo);
}

stock wResetPlayerWeapons(playerid)
{
	for(new i = 0; i < 53; i++) weaponsInfo[playerid][Weapons][i] = 0, weaponsInfo[playerid][Ammo][i] = 0;
	for(new i = 0; i < 13; i++) weaponsInfo[playerid][Slot][i] = 0;
	if(!weaponsInfo[playerid][wImmune]) SetTimerEx("wResetImmune", 3000, false, "i", playerid);
	weaponsInfo[playerid][wImmune] = true;
	return ResetPlayerWeapons(playerid);
}

stock GetWeaponSlot(weaponid)
{
	new slot;
	switch(weaponid){
		case 0, 1: slot = 0; // No weapon
		case 2 .. 9: slot = 1; // Melee
		case 22 .. 24: slot = 2; // Handguns
		case 25 .. 27: slot = 3; // Shotguns
		case 28, 29, 32: slot = 4; // Sub-Machineguns
		case 30, 31: slot = 5; // Machineguns
		case 33, 34: slot = 6; // Rifles
		case 35 .. 38: slot = 7; // Heavy Weapons
		case 16, 18, 39: slot = 8; // Projectiles
		case 42, 43: slot = 9; // Special 1
		case 14: slot = 10; // Gifts
		case 44 .. 46: slot = 11; // Special 2
		case 40: slot = 12; // Detonators
		default: slot = -1; // No slot
	}
	return slot;
}

stock wSetPlayerAmmo(playerid, weapon, ammo)
{
   	weaponsInfo[playerid][Ammo][weapon] = ammo;
	if(!weaponsInfo[playerid][wImmune]) SetTimerEx("wResetImmune", 3000, false, "i", playerid);
	weaponsInfo[playerid][wImmune] = true;
	return SetPlayerAmmo(playerid, weapon, ammo);
}

/*======================================================
  * HOOKS
========================================================*/
#if defined _ALS_OnPlayerExitVehicle
	#undef OnPlayerExitVehicle
#else
	#define _ALS_OnPlayerExitVehicle
#endif

#define OnPlayerExitVehicle OnPlayerExitVehicleWeapons

#if defined _ALS_OnPlayerEnterVehicle
	#undef OnPlayerEnterVehicle
#else
	#define _ALS_OnPlayerEnterVehicle
#endif

#define OnPlayerEnterVehicle OnPlayerEnterVehicleWeapons

#if defined _ALS_OnPlayerWeaponShot
	#undef OnPlayerWeaponShot
#else
	#define _ALS_OnPlayerWeaponShot
#endif

#define OnPlayerWeaponShot OnPlayerWeaponShotWeapons

#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn OnPlayerSpawnWeapons

#if defined _ALS_OnPlayerUpdate
  #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate OnPlayerUpdateWeapons

#if defined _ALS_SetPlayerAmmo
	#undef SetPlayerAmmo
#else
	#define _ALS_SetPlayerAmmo
#endif

#define SetPlayerAmmo    wSetPlayerAmmo

#if defined _ALS_ResetPlayerWeapons
	#undef ResetPlayerWeapons
#else
	#define _ALS_ResetPlayerWeapons
#endif

#define ResetPlayerWeapons wResetPlayerWeapons

#if defined _ALS_GivePlayerWeapon
	#undef GivePlayerWeapon
#else
	#define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon    wGivePlayerWeapon

#if defined _ALS_OnPlayerConnect
  #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect OnPlayerConnectWeapons

#if defined _ALS_OnPlayerDeath
  #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif

#define OnPlayerDeath OnPlayerDeathWeapons

#if defined _ALS_OnPlayerDisconnect
  #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect OnPlayerDisconnectWeapons

#else

stock wResetPlayerWeaponsFS(playerid)
	return CallLocalFunction("wResetPlayerWeapons", "i", playerid);
	
stock wGivePlayerWeaponFS(playerid, weapon, ammo)
	return CallLocalFunction("wGivePlayerWeapon","iii", playerid, weapon, ammo);
	
stock wSetPlayerAmmoFS(playerid, weapon, ammo)
	return CallLocalFunction("wSetPlayerAmmo", "iii", playerid, weapon, ammo);


#if defined _ALS_SetPlayerAmmo
	#undef SetPlayerAmmo
#else
	#define _ALS_SetPlayerAmmo
#endif

#define SetPlayerAmmo    wSetPlayerAmmoFS

#if defined _ALS_ResetPlayerWeapons
	#undef ResetPlayerWeapons
#else
	#define _ALS_ResetPlayerWeapons
#endif

#define ResetPlayerWeapons wResetPlayerWeaponsFS

#if defined _ALS_GivePlayerWeapon
	#undef GivePlayerWeapon
#else
	#define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon    wGivePlayerWeaponFS


#endif

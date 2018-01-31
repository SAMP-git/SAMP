/****************************************************************************************************
 *                                                                                                  *
 *                                   )(   Visible NPC HP Bar   )(                                   *
 *                                                                                                  *
 * Copyright © 2018 Abyss Morgan. All rights reserved.                                              *
 *                                                                                                  *
 * Website:  http://8.ct8.pl                                                                        *
 * Download: http://8.ct8.pl/r/download                                                             *
 *                                                                                                  *
 * Plugins: YSF                                                                                     *
 * Modules: None                                                                                    *
 *                                                                                                  *
 * File Version: 1.0.0                                                                              *
 *                                                                                                  *
 * Functions:                                                                                       *
 * ShowNPCInTabList(npcid);                                                                         *
 * HideNPCInTabList(npcid);                                                                         *
 *                                                                                                  *
 ****************************************************************************************************/

#if defined _visible_npc_inc
	#endinput
#endif
#define _visible_npc_inc

#if defined _samp_included
	#if (!defined GetPlayerPoolSize || !defined GetSVarInt)
		#error [ADM] This include requires SA:MP version 0.3.7 (8.ct8.pl/r/download)
	#endif
#elseif defined _rwmp_included
	#error [ADM] This game currently is not supported
#else
	#error [ADM] Not found any general game includes
#endif

#if ((!defined _YSF_included) || (!defined RemovePlayerForPlayer))
	#error [ADM] You need YSF R19 (github.com/kurta999/YSF/releases)
#endif

#define VNPC::				VNPC_

new bool:VNPC::IsNPCVisible[MAX_PLAYERS],
	bool:VNPC::FirstSpawn[MAX_PLAYERS];
	
stock VNPC::ShowNPCInTabListForPlayer(playerid,npcid){
	if(!IsPlayerNPC(npcid)) return 0;
	TogglePlayerFakePing(npcid,true);
	SetPlayerFakePing(npcid,0);
	RemovePlayerForPlayer(playerid,npcid);
	AddPlayerForPlayer(playerid,npcid,0);
	return 1;
}

stock VNPC::HideNPCInTabListForPlayer(playerid,npcid){
	if(!IsPlayerNPC(npcid)) return 0;
	TogglePlayerFakePing(npcid,true);
	SetPlayerFakePing(npcid,0);
	RemovePlayerForPlayer(playerid,npcid);
	AddPlayerForPlayer(playerid,npcid,1);
	return 1;
}

stock ShowNPCInTabList(npcid){
	if(!IsPlayerNPC(npcid)) return 0;
	VNPC::IsNPCVisible[npcid] = true;
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++){
		if(IsPlayerConnected(i) && !IsPlayerNPC(i)){
			VNPC::ShowNPCInTabListForPlayer(i,npcid);
		}
	}
	return 1;
}

stock HideNPCInTabList(npcid){
	if(!IsPlayerNPC(npcid)) return 0;
	VNPC::IsNPCVisible[npcid] = false;
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++){
		if(IsPlayerConnected(i) && !IsPlayerNPC(i)){
			VNPC::HideNPCInTabListForPlayer(i,npcid);
		}
	}
	return 1;
}

//Hook: OnPlayerConnect
public OnPlayerConnect(playerid){
	if(IsPlayerNPC(playerid)){
		VNPC::IsNPCVisible[playerid] = false;
	}
	VNPC::FirstSpawn[playerid] = false;
	#if defined VNPC_OnPlayerConnect
		VNPC_OnPlayerConnect(playerid);
	#endif
	return 1;
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect VNPC_OnPlayerConnect
#if defined VNPC_OnPlayerConnect
	forward VNPC_OnPlayerConnect(playerid);
#endif

//Hook: OnPlayerSpawn
public OnPlayerSpawn(playerid){
	if(!VNPC::FirstSpawn[playerid]){
		VNPC::FirstSpawn[playerid] = true;
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++){
			if(IsPlayerConnected(i) && IsPlayerNPC(i)){
				if(VNPC::IsNPCVisible[i]){
					VNPC::ShowNPCInTabListForPlayer(playerid,i);
				}
			}
		}
	}
	#if defined VNPC_OnPlayerSpawn
		VNPC_OnPlayerSpawn(playerid);
	#endif
	return 1;
}

#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn VNPC_OnPlayerSpawn
#if defined VNPC_OnPlayerSpawn
	forward VNPC_OnPlayerSpawn(playerid);
#endif 

//EOF
/**********************************************************************************************************************************
 *                                                                                                                                *
 *                                                 Military Dynamic Mines System                                                  *
 *                                                                                                                                *
 * Copyright © 2018 Abyss Morgan. All rights reserved.                                                                            *
 *                                                                                                                                *
 * Website:  http://8.ct8.pl                                                                                                      *
 * Download: http://8.ct8.pl/r/download                                                                                           *
 *                                                                                                                                *
 * Plugins: Streamer                                                                                                              *
 * Modules: 3DTryg                                                                                                                *
 *                                                                                                                                *
 * File Version: 2.1.0                                                                                                            *
 * Streamer Version: 2.9.1                                                                                                        *
 * 3DTryg Version: 4.6.0                                                                                                          *
 *                                                                                                                                *
 * Functions:                                                                                                                     *
 * CreateDynamicMine(type,Float:detection_range,Float:explode_radius,Float:health,respawntime,Float:x,Float:y,Float:z,            *
 *                   worldid,interiorid,playerid,Float:streamdistance,mine_object = MINE_OBJECT_STANDARD,teamid = ANY_TEAM,       *
 *                   detect_type = MINE_DETECT_TYPE_ALL,byplayerid = INVALID_PLAYER_ID,Float:damage=-1.0,                         *
 *                   Float:vehicle_damage=-1.0);                                                                                  *
 * CreateDynamicMineEx(objectid,type,Float:detection_range,Float:explode_radius,Float:health,respawntime,Float:x,Float:y,Float:z, *
 *                     Float:rx,Float:ry,Float:rz,worldid,interiorid,playerid,Float:streamdistance,teamid = ANY_TEAM,             *
 *                     detect_type = MINE_DETECT_TYPE_ALL,byplayerid = INVALID_PLAYER_ID,Float:damage=-1.0,                       *
 *                     Float:vehicle_damage=-1.0);                                                                                *
 * DestroyDynamicMine(mobid);                                                                                                     *
 * Float:GetMineExplodeVisibility();                                                                                              *
 * SetMineExplodeVisibility(Float:explode_stream);                                                                                *
 *                                                                                                                                *
 * Extended Functions:                                                                                                            *
 * ForceRespawnDynamicMine(mobid);                                                                                                *
 * CountDynamicMines();                                                                                                           *
 * GetMineObjectID(mobid);                                                                                                        *
 * GetMineAreaID(mobid);                                                                                                          *
 * GetDynamicMinePos(mobid,&Float:x,&Float:y,&Float:z);                                                                           *
 * SetDynamicMinePos(mobid,Float:x,Float:y,Float:z);                                                                              *
 * GetDynamicMineRot(mobid,&Float:rx,&Float:ry,&Float:rz);                                                                        *
 * SetDynamicMineRot(mobid,Float:rx,Float:ry,Float:rz);                                                                           *
 * GetDynamicMineDetectionRange(mobid,&Float:detection_range);                                                                    *
 * SetDynamicMineDetectionRange(mobid,Float:detection_range);                                                                     *
 * GetDynamicMineExplodeRadius(mobid,&Float:explode_radius);                                                                      *
 * SetDynamicMineExplodeRadius(mobid,Float:explode_radius);                                                                       *
 * GetDynamicMineHealth(mobid,&Float:health);                                                                                     *
 * SetDynamicMineHealth(mobid,Float:health);                                                                                      *
 * GetDynamicMineMaxHealth(mobid,&Float:health);                                                                                  *
 * SetDynamicMineMaxHealth(mobid,Float:health);                                                                                   *
 * GetDynamicMineVW(mobid);                                                                                                       *
 * SetDynamicMineVW(mobid,worldid);                                                                                               *
 * GetDynamicMineINT(mobid);                                                                                                      *
 * SetDynamicMineINT(mobid,interiorid);                                                                                           *
 * GetDynamicMineSD(mobid,&Float:streamdistance);                                                                                 *
 * SetDynamicMineSD(mobid,Float:streamdistance);                                                                                  *
 * GetDynamicMineTeam(mobid);                                                                                                     *
 * SetDynamicMineTeam(mobid,teamid);                                                                                              *
 * GetDynamicMineType(mobid);                                                                                                     *
 * SetDynamicMineType(mobid,type);                                                                                                *
 * GetDynamicMineRespawntime(mobid);                                                                                              *
 * SetDynamicMineRespawntime(mobid,respawntime);                                                                                  *
 * Float:GetDynamicMineDamage(mobid);                                                                                             *
 * SetDynamicMineDamage(mobid,Float:damage);                                                                                      *
 * Float:GetDynamicMineVehicleDamage(mobid);                                                                                      *
 * SetDynamicMineVehicleDamage(mobid,Float:damage);                                                                               *
 * Mine_Update(variable);                                                                                                         *
 *                                                                                                                                *
 * Callbacks:                                                                                                                     *
 * OnMineDestroy(mobid,Float:x,Float:y,Float:z,type,killerid,Float:radius,damagerid);                                             *
 *                                                                                                                                *
 **********************************************************************************************************************************/

#if ((!defined Streamer_AppendArrayData) || (!defined INVALID_STREAMER_ID) || (!defined STREAMER_TYPE_ACTOR))
	#error [ADM] You need Streamer v2.9.1 (github.com/samp-incognito/samp-streamer-plugin/releases)
#endif

//Check Version 3DTryg.inc
#if !defined _3D_Tryg
	#error [ADM] You need 3DTryg.inc v4.6.0 (8.ct8.pl/d/3DTryg.inc)
#elseif !defined Tryg3D_Version
	#error [ADM] Update you 3DTryg.inc to v4.6.0 (8.ct8.pl/d/3DTryg.inc)
#elseif (Tryg3D_Version < 40600)
	#error [ADM] Update you 3DTryg.inc to v4.6.0 (8.ct8.pl/d/3DTryg.inc)
#endif

#if defined _dynamic_mine_system
	#endinput
#endif
#define _dynamic_mine_system
#define DYNAMIC_FILTERSCRIPT					"ALS:HOOK:METHOD"

#define MINE_OBJECT_STANDARD					(0)
#define MINE_OBJECT_UNDERWATER					(1)
#define MINE_OBJECT_LASER						(3)
#define MINE_OBJECT_PIZZA						(4)

#define MAX_MINES								(2048)
#define MINE_EXTRA_ID_OFFSET					(200001)
#define MINE_INFINITY_HEALTH					(1000000.0)

#define MINE_STATUS_UNACTIVE					(0)
#define MINE_STATUS_ACTIVE						(1)
#define MINE_STATUS_DAMAGED						(2)

#define MINE_TYPE_EXPLODE_SMALL					(12)
#define MINE_TYPE_EXPLODE_NORMAL				(2)
#define MINE_TYPE_EXPLODE_LARGE					(6)
#define MINE_TYPE_EXPLODE_HUGE					(7)

#define INVALID_MINE_ID							(0)

#define MINE_DESTROY_TYPE_DETECT				(0)	//When mine has been destroyed by detection range
#define MINE_DESTROY_TYPE_KILL					(1)	//When mine has been destroyed by player shoot

#define MINE_DETECT_TYPE_ALL					(0)
#define MINE_DETECT_TYPE_PLAYER					(1)
#define MINE_DETECT_TYPE_VEHICLE				(2)

#if !defined ANY_TEAM
	#define ANY_TEAM							(0xFFFF)
#endif

#define Mine_Update(%0)							for(new %0 = 1; %0 <= MineUpperBound; %0++) if(MineComponent[(%0)][mine_status] != MINE_STATUS_UNACTIVE)

enum e_mine {
	mine_status,
	STREAMER_TAG_OBJECT mine_objectid,
	STREAMER_TAG_AREA mine_areaid,
	mine_tickcount,
	mine_type,
	mine_respawntime,
	Float:mine_health,
	Float:mine_maxhealth,
	Float:mine_explode,
	mine_teamid,
	mine_dtype,
	mine_playerid,
	Float:mine_damage,
	Float:mine_vehicle_damage
}

new MineComponent[MAX_MINES][e_mine],
	MineUpperBound = 0,
	MineTimer,
	Float:MineExplosionVisibility = 200.0;

#define GetMineObjectID(%0)						MineComponent[(%0)][mine_objectid]
#define GetMineAreaID(%0)						MineComponent[(%0)][mine_areaid]

#define GetMineExplodeVisibility()				MineExplosionVisibility
#define SetMineExplodeVisibility(%0)			MineExplosionVisibility = (%0)

#define GetDynamicMinePos(%0,%1,%2,%3)			GetDynamicObjectPos(MineComponent[(%0)][mine_objectid],%1,%2,%3)
#define SetDynamicMinePos(%0,%1,%2,%3)			SetDynamicObjectPos(MineComponent[(%0)][mine_objectid],%1,%2,%3)

#define GetDynamicMineRot(%0,%1,%2,%3)			GetDynamicObjectRot(MineComponent[(%0)][mine_objectid],%1,%2,%3)
#define SetDynamicMineRot(%0,%1,%2,%3)			SetDynamicObjectRot(MineComponent[(%0)][mine_objectid],%1,%2,%3)

#define GetDynamicMineDetectionRange(%0,%1)		Streamer::GetFloatData(STREAMER_TYPE_AREA,MineComponent[(%0)][mine_areaid],E_STREAMER_SIZE,(%1))
#define SetDynamicMineDetectionRange(%0,%1)		Streamer::SetFloatData(STREAMER_TYPE_AREA,MineComponent[(%0)][mine_areaid],E_STREAMER_SIZE,(%1))

#define GetDynamicMineExplodeRadius(%0,%1)		(%1) = MineComponent[(%0)][mine_explode]
#define SetDynamicMineExplodeRadius(%0,%1)		MineComponent[(%0)][mine_explode] = (%1)

#define GetDynamicMineHealth(%0,%1)				(%1) = MineComponent[(%0)][mine_health]
#define SetDynamicMineHealth(%0,%1)				MineComponent[(%0)][mine_health] = (%1)

#define GetDynamicMineMaxHealth(%0,%1)			(%1) = MineComponent[(%0)][mine_maxhealth]
#define SetDynamicMineMaxHealth(%0,%1)			MineComponent[(%0)][mine_maxhealth] = (%1)

#define GetDynamicMineTeam(%0)					MineComponent[(%0)][mine_teamid]
#define SetDynamicMineTeam(%0,%1)				MineComponent[(%0)][mine_teamid] = (%1)

#define GetDynamicMineVW(%0)					Streamer::GetIntData(STREAMER_TYPE_OBJECT,MineComponent[(%0)][mine_objectid],E_STREAMER_WORLD_ID,(%1))
#define GetDynamicMineINT(%0)					Streamer::GetIntData(STREAMER_TYPE_OBJECT,MineComponent[(%0)][mine_objectid],E_STREAMER_INTERIOR_ID,(%1))

#define GetDynamicMineType(%0)					MineComponent[(%0)][mine_type]
#define SetDynamicMineType(%0,%1)				MineComponent[(%0)][mine_type] = (%1)

#define GetDynamicMineRespawntime(%0)			MineComponent[(%0)][mine_respawntime]
#define SetDynamicMineRespawntime(%0,%1)		MineComponent[(%0)][mine_respawntime] = (%1)

#define GetDynamicMineDamage(%0)				MineComponent[(%0)][mine_damage]
#define SetDynamicMineDamage(%0,%1)				MineComponent[(%0)][mine_damage] = (%1)

#define GetDynamicMineVehicleDamage(%0)			MineComponent[(%0)][mine_vehicle_damage]
#define SetDynamicMineVehicleDamage(%0,%1)		MineComponent[(%0)][mine_vehicle_damage] = (%1)

#define GetDynamicMineSD(%0,%1)					Streamer::GetFloatData(STREAMER_TYPE_OBJECT,MineComponent[(%0)][mine_objectid],E_STREAMER_STREAM_DISTANCE,(%1))

stock SetDynamicMineVW(mobid,worldid){
	Streamer::SetIntData(STREAMER_TYPE_OBJECT,MineComponent[mobid][mine_objectid],E_STREAMER_WORLD_ID,worldid);
	Streamer::SetIntData(STREAMER_TYPE_AREA,MineComponent[mobid][mine_areaid],E_STREAMER_WORLD_ID,worldid);
}

stock SetDynamicMineINT(mobid,interiorid){
	Streamer::SetIntData(STREAMER_TYPE_OBJECT,MineComponent[mobid][mine_objectid],E_STREAMER_INTERIOR_ID,interiorid);
	Streamer::SetIntData(STREAMER_TYPE_AREA,MineComponent[mobid][mine_areaid],E_STREAMER_INTERIOR_ID,interiorid);
}

stock SetDynamicMineSD(mobid,Float:streamdistance){
	Streamer::SetFloatData(STREAMER_TYPE_OBJECT,MineComponent[mobid][mine_objectid],E_STREAMER_STREAM_DISTANCE,streamdistance);
	Streamer::SetFloatData(STREAMER_TYPE_AREA,MineComponent[mobid][mine_areaid],E_STREAMER_STREAM_DISTANCE,streamdistance);
}

stock CountDynamicMines(){
	new cnt = 0;
	for(new mobid = 1; mobid <= MineUpperBound; mobid++){
		if(MineComponent[mobid][mine_status] != MINE_STATUS_UNACTIVE) cnt++;
	}
	return cnt;
}

stock FindFreeMineID(){
	for(new mobid = 1; mobid < MAX_MINES; mobid++){
		if(MineComponent[mobid][mine_status] == MINE_STATUS_UNACTIVE) return mobid;
	}
	return INVALID_MINE_ID;
}

forward OnDynamicMineUpdate(tec);
forward ForceRespawnDynamicMine(mobid);
forward DestroyDynamicMine(mobid);
forward ExplodeDynamicMine(mobid,type,playerid);
forward RespawnDynamicMine(mobid);
forward OnMineDestroy(mobid,Float:x,Float:y,Float:z,type,killerid,Float:radius,damagerid);

new bool:CRC_MinesInit = true,
	bool:CRC_MinesExit = true;

//Hook: OnFilterScriptInit
public OnFilterScriptInit(){
	if(CRC_MinesInit){
		CRC_MinesInit = false;
		MineTimer = SetTimerEx("OnDynamicMineUpdate",1000,true,"d",1);
	}
	#if defined MDM_OnFilterScriptInit
		MDM_OnFilterScriptInit();
	#endif
	return 1;
}

#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit MDM_OnFilterScriptInit
#if defined MDM_OnFilterScriptInit
	forward MDM_OnFilterScriptInit();
#endif

//Hook: OnGameModeInit
public OnGameModeInit(){
	if(CRC_MinesInit){
		CRC_MinesInit = false;
		MineTimer = SetTimerEx("OnDynamicMineUpdate",1000,true,"d",1);
	}
	#if defined MDM_OnGameModeInit
		MDM_OnGameModeInit();
	#endif
	return 1;
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit MDM_OnGameModeInit
#if defined MDM_OnGameModeInit
	forward MDM_OnGameModeInit();
#endif

//Hook: OnFilterScriptExit
public OnFilterScriptExit(){
	if(CRC_MinesExit){
		CRC_MinesExit = false;
		KillTimer(MineTimer);
	}
	#if defined MDM_OnFilterScriptExit
		MDM_OnFilterScriptExit();
	#endif
	return 1;
}

#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif
#define OnFilterScriptExit MDM_OnFilterScriptExit
#if defined MDM_OnFilterScriptExit
	forward MDM_OnFilterScriptExit();
#endif

//Hook: OnGameModeExit
public OnGameModeExit(){
	if(CRC_MinesExit){
		CRC_MinesExit = false;
		KillTimer(MineTimer);
	}
	#if defined MDM_OnGameModeExit
		MDM_OnGameModeExit();
	#endif
	return 1;
}

#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit MDM_OnGameModeExit
#if defined MDM_OnGameModeExit
	forward MDM_OnGameModeExit();
#endif

public OnDynamicMineUpdate(tec){
	#pragma unused tec
	new tick = GetTickCount();
	for(new mobid = 1; mobid <= MineUpperBound; mobid++){
		if(MineComponent[mobid][mine_status] == MINE_STATUS_DAMAGED){
			if((tick >= MineComponent[mobid][mine_tickcount]) && (MineComponent[mobid][mine_tickcount] != 0)){
				MineComponent[mobid][mine_tickcount] = 0;
				RespawnDynamicMine(mobid);
			}
		}
	}
	return 1;
}

public ForceRespawnDynamicMine(mobid){
	if(mobid <= 0 || mobid >= MAX_MINES) return 0;
	if(MineComponent[mobid][mine_status] == MINE_STATUS_UNACTIVE) return 0;
	if(MineComponent[mobid][mine_status] == MINE_STATUS_DAMAGED && MineComponent[mobid][mine_tickcount] != 0){
		MineComponent[mobid][mine_tickcount] = 0;
		RespawnDynamicMine(mobid);
	}
	return 1;
}

stock CreateDynamicMine(type,Float:detection_range,Float:explode_radius,Float:health,respawntime,Float:x,Float:y,Float:z,worldid,interiorid,playerid,Float:streamdistance,mine_object=MINE_OBJECT_STANDARD,teamid=ANY_TEAM,detect_type=MINE_DETECT_TYPE_ALL,byplayerid=INVALID_PLAYER_ID,Float:damage=-1.0,Float:vehicle_damage=-1.0){
	new mobid = FindFreeMineID();
	if(mobid == INVALID_MINE_ID){
		printf("[ADM] Error: Failed to CreateDynamicMine limit %d exceeded, add #define MAX_MINES before Mines.inc",MAX_MINES);
		return INVALID_MINE_ID;
	}
	
	MineComponent[mobid][mine_health] = health;
	MineComponent[mobid][mine_maxhealth] = health;
	MineComponent[mobid][mine_explode] = explode_radius;
	MineComponent[mobid][mine_tickcount] = 0;
	MineComponent[mobid][mine_type] = type;
	MineComponent[mobid][mine_respawntime] = respawntime;
	MineComponent[mobid][mine_teamid] = teamid;
	MineComponent[mobid][mine_dtype] = detect_type;
	MineComponent[mobid][mine_playerid] = byplayerid;
	MineComponent[mobid][mine_damage] = damage;
	MineComponent[mobid][mine_vehicle_damage] = vehicle_damage;
	
	if(MineComponent[mobid][mine_damage] != -1.0 && MineComponent[mobid][mine_vehicle_damage] == -1.0) MineComponent[mobid][mine_vehicle_damage] = 750.0;
	
	switch(mine_object){
		case MINE_OBJECT_STANDARD:		MineComponent[mobid][mine_objectid] = CreateDynamicObject(19602,x,y,z-0.93,0.0,0.0,0.0,worldid,interiorid,playerid,streamdistance);
		case MINE_OBJECT_UNDERWATER:	MineComponent[mobid][mine_objectid] = CreateDynamicObject(2918,x,y,z-0.10,0.0,0.0,0.0,worldid,interiorid,playerid,streamdistance);
		case MINE_OBJECT_LASER:			MineComponent[mobid][mine_objectid] = CreateDynamicObject(18643,x,y,z-0.09,180.0,90.0,0.0,worldid,interiorid,playerid,streamdistance);
		case MINE_OBJECT_PIZZA:			MineComponent[mobid][mine_objectid] = CreateDynamicObject(19580,x,y,z+0.05,0.0,0.0,0.0,worldid,interiorid,playerid,streamdistance);
		
		default:						MineComponent[mobid][mine_objectid] = CreateDynamicObject(19602,x,y,z-0.93,0.0,0.0,0.0,worldid,interiorid,playerid,streamdistance);
	}
	MineComponent[mobid][mine_areaid] = CreateDynamicSphere(x,y,z,detection_range,worldid,interiorid,playerid);
	AttachDynamicAreaToObject(MineComponent[mobid][mine_areaid],MineComponent[mobid][mine_objectid]);
	
	Streamer::SetIntData(STREAMER_TYPE_OBJECT,MineComponent[mobid][mine_objectid],E_STREAMER_EXTRA_ID,(mobid+MINE_EXTRA_ID_OFFSET));
	Streamer::SetIntData(STREAMER_TYPE_AREA,MineComponent[mobid][mine_areaid],E_STREAMER_EXTRA_ID,(mobid+MINE_EXTRA_ID_OFFSET));
	MineComponent[mobid][mine_status] = MINE_STATUS_ACTIVE;
	if(mobid > MineUpperBound) MineUpperBound = mobid;
	return mobid;
}

stock CreateDynamicMineEx(objectid,type,Float:detection_range,Float:explode_radius,Float:health,respawntime,Float:x,Float:y,Float:z,Float:rx,Float:ry,Float:rz,worldid,interiorid,playerid,Float:streamdistance,teamid=ANY_TEAM,detect_type=MINE_DETECT_TYPE_ALL,byplayerid=INVALID_PLAYER_ID,Float:damage=-1.0,Float:vehicle_damage=-1.0){
	new mobid = FindFreeMineID();
	if(mobid == INVALID_MINE_ID){
		printf("[ADM] Error: Failed to CreateDynamicMine limit %d exceeded, add #define MAX_MINES before Mines.inc",MAX_MINES);
		return mobid;
	}
	MineComponent[mobid][mine_health] = health;
	MineComponent[mobid][mine_maxhealth] = health;
	MineComponent[mobid][mine_explode] = explode_radius;
	MineComponent[mobid][mine_tickcount] = 0;
	MineComponent[mobid][mine_type] = type;
	MineComponent[mobid][mine_respawntime] = respawntime;
	MineComponent[mobid][mine_teamid] = teamid;
	MineComponent[mobid][mine_dtype] = detect_type;
	MineComponent[mobid][mine_playerid] = byplayerid;
	MineComponent[mobid][mine_damage] = damage;
	MineComponent[mobid][mine_vehicle_damage] = vehicle_damage;
	
	if(MineComponent[mobid][mine_damage] != -1.0 && MineComponent[mobid][mine_vehicle_damage] == -1.0) MineComponent[mobid][mine_vehicle_damage] = 750.0;

	MineComponent[mobid][mine_objectid] = CreateDynamicObject(objectid,x,y,z,rx,ry,rz,worldid,interiorid,playerid,streamdistance);
	MineComponent[mobid][mine_areaid] = CreateDynamicSphere(x,y,z,detection_range,worldid,interiorid,playerid);
	AttachDynamicAreaToObject(MineComponent[mobid][mine_areaid],MineComponent[mobid][mine_objectid]);
	
	Streamer::SetIntData(STREAMER_TYPE_OBJECT,MineComponent[mobid][mine_objectid],E_STREAMER_EXTRA_ID,(mobid+MINE_EXTRA_ID_OFFSET));
	Streamer::SetIntData(STREAMER_TYPE_AREA,MineComponent[mobid][mine_areaid],E_STREAMER_EXTRA_ID,(mobid+MINE_EXTRA_ID_OFFSET));
	MineComponent[mobid][mine_status] = MINE_STATUS_ACTIVE;
	if(mobid > MineUpperBound) MineUpperBound = mobid;
	return mobid;
}

public DestroyDynamicMine(mobid){
	if(mobid <= 0 || mobid >= MAX_MINES) return 0;
	if(MineComponent[mobid][mine_status] == MINE_STATUS_UNACTIVE) return 0;
	if(IsValidDynamicObject(MineComponent[mobid][mine_objectid])) DestroyDynamicObject(MineComponent[mobid][mine_objectid]);
	if(IsValidDynamicArea(MineComponent[mobid][mine_areaid])) DestroyDynamicArea(MineComponent[mobid][mine_areaid]);
	MineComponent[mobid][mine_objectid] = STREAMER_TAG_OBJECT INVALID_STREAMER_ID;
	MineComponent[mobid][mine_areaid] = STREAMER_TAG_AREA INVALID_STREAMER_ID;
	MineComponent[mobid][mine_health] = 0.0;
	MineComponent[mobid][mine_maxhealth] = 0.0;
	MineComponent[mobid][mine_explode] = 0.0;
	MineComponent[mobid][mine_tickcount] = 0;
	MineComponent[mobid][mine_type] = 0;
	MineComponent[mobid][mine_respawntime] = 0;
	MineComponent[mobid][mine_teamid] = ANY_TEAM;
	MineComponent[mobid][mine_dtype] = MINE_DETECT_TYPE_ALL;
	MineComponent[mobid][mine_damage] = -1;
	MineComponent[mobid][mine_vehicle_damage] = -1;
	MineComponent[mobid][mine_status] = MINE_STATUS_UNACTIVE;
	if(mobid == MineUpperBound) MineUpperBound--;
	return 1;
}

public ExplodeDynamicMine(mobid,type,playerid){
	if(mobid <= 0 || mobid >= MAX_MINES) return 0;
	if(MineComponent[mobid][mine_status] != MINE_STATUS_ACTIVE) return 0;
	new Float:minex, Float:miney, Float:minez,
		worldid = Streamer::GetIntData(STREAMER_TYPE_OBJECT,MineComponent[mobid][mine_objectid],E_STREAMER_WORLD_ID),
		interiorid = Streamer::GetIntData(STREAMER_TYPE_OBJECT,MineComponent[mobid][mine_objectid],E_STREAMER_INTERIOR_ID),
		Float:radius;
	
	Streamer::GetFloatData(STREAMER_TYPE_AREA,MineComponent[mobid][mine_areaid],E_STREAMER_SIZE,radius);
	GetDynamicObjectPos(MineComponent[mobid][mine_objectid],minex,miney,minez);
	
	if(GetDynamicMineDamage(mobid) == -1.0){
		CreateDynamicExplosion(minex,miney,minez,MineComponent[mobid][mine_type],MineComponent[mobid][mine_explode],worldid,interiorid,-1,GetMineExplodeVisibility());
	} else {
		if(GetDynamicMineVehicleDamage(mobid) == -1.0) MineComponent[mobid][mine_vehicle_damage] = 750.0;
		CreateDynamicExplosionDMG(minex,miney,minez,MineComponent[mobid][mine_type],MineComponent[mobid][mine_explode],worldid,interiorid,-1,GetMineExplodeVisibility(),GetDynamicMineDamage(mobid),GetDynamicMineVehicleDamage(mobid),MineComponent[mobid][mine_playerid]);
	}
	MineComponent[mobid][mine_tickcount] = GetTickCount()+(MineComponent[mobid][mine_respawntime]*1000);
	MineComponent[mobid][mine_status] = MINE_STATUS_DAMAGED;
	Streamer::SetIntData(STREAMER_TYPE_OBJECT,MineComponent[mobid][mine_objectid],E_STREAMER_WORLD_ID,worldid+10000); //hide mine
	CallRemoteFunction("OnMineDestroy","dfffddfd",mobid,minex,miney,minez,type,playerid,radius,MineComponent[mobid][mine_playerid]);
	return 1;
}

public RespawnDynamicMine(mobid){
	if(mobid <= 0 || mobid >= MAX_MINES) return 0;
	if(MineComponent[mobid][mine_status] == MINE_STATUS_ACTIVE) return 0;
	new worldid = Streamer::GetIntData(STREAMER_TYPE_OBJECT,MineComponent[mobid][mine_objectid],E_STREAMER_WORLD_ID);
	Streamer::SetIntData(STREAMER_TYPE_OBJECT,MineComponent[mobid][mine_objectid],E_STREAMER_WORLD_ID,worldid-10000); //show mine
	MineComponent[mobid][mine_tickcount] = 0;
	MineComponent[mobid][mine_health] = MineComponent[mobid][mine_maxhealth];
	MineComponent[mobid][mine_status] = MINE_STATUS_ACTIVE;
	return 1;
}

//Hook: OnPlayerShootDynamicObject
public OnPlayerShootDynamicObject(playerid, weaponid, STREAMER_TAG_OBJECT objectid, Float:x, Float:y, Float:z){
	new mobid = (Streamer::GetIntData(STREAMER_TYPE_OBJECT,objectid,E_STREAMER_EXTRA_ID)-MINE_EXTRA_ID_OFFSET);
	if(mobid > 0 && mobid < MAX_MINES){
		if(MineComponent[mobid][mine_status] == MINE_STATUS_ACTIVE && MineComponent[mobid][mine_objectid] == objectid && MineComponent[mobid][mine_playerid] != playerid){
			if((MineComponent[mobid][mine_teamid] != GetPlayerTeam(playerid)) || (MineComponent[mobid][mine_teamid] == ANY_TEAM)){
				if(MineComponent[mobid][mine_health] != MINE_INFINITY_HEALTH){
					MineComponent[mobid][mine_health] -= Tryg3D::GetWeaponDamage(weaponid);
					new buffer[128];
					if(MineComponent[mobid][mine_health] > 0){
						format(buffer,sizeof buffer,"Mine~n~~w~%.0f HP~n~~r~~h~~h~-%.0f HP",MineComponent[mobid][mine_health],Tryg3D::GetWeaponDamage(weaponid));
					} else {
						format(buffer,sizeof buffer,"Mine~n~~w~0 HP~n~~r~~h~~h~-%.0f HP",Tryg3D::GetWeaponDamage(weaponid));
					}
					GameTextForPlayer(playerid,buffer,500,4);
					if(MineComponent[mobid][mine_health] <= 0.0){
						ExplodeDynamicMine(mobid,MINE_DESTROY_TYPE_KILL,playerid);
					}
				}
			}
		}
	}
	#if defined MDM_OnPlayerShootDynamicObject
		MDM_OnPlayerShootDynamicObject(playerid, weaponid, objectid, x, y, z);
	#endif
	return 1;
}

#if defined _ALS_OnPlayerShootDynamicObject
	#undef OnPlayerShootDynamicObject
#else
	#define _ALS_OnPlayerShootDynamicObject
#endif
#define OnPlayerShootDynamicObject MDM_OnPlayerShootDynamicObject
#if defined MDM_OnPlayerShootDynamicObject
	forward MDM_OnPlayerShootDynamicObject(playerid, weaponid, STREAMER_TAG_OBJECT objectid, Float:x, Float:y, Float:z);
#endif

//Hook: OnPlayerEnterDynamicArea
public OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA areaid){
	new mobid = (Streamer::GetIntData(STREAMER_TYPE_AREA,areaid,E_STREAMER_EXTRA_ID)-MINE_EXTRA_ID_OFFSET);
	if(mobid > 0 && mobid < MAX_MINES){
		if(MineComponent[mobid][mine_status] == MINE_STATUS_ACTIVE && MineComponent[mobid][mine_areaid] == areaid && MineComponent[mobid][mine_playerid] != playerid){
			new bool:tmp_det = false;
			switch(MineComponent[mobid][mine_dtype]){
				case MINE_DETECT_TYPE_ALL:		tmp_det = true;
				case MINE_DETECT_TYPE_PLAYER: 	if(!IsPlayerInAnyVehicle(playerid)) tmp_det = true;
				case MINE_DETECT_TYPE_VEHICLE:	if(IsPlayerInAnyVehicle(playerid)) tmp_det = true;
			}
			if((MineComponent[mobid][mine_teamid] != GetPlayerTeam(playerid)) || (MineComponent[mobid][mine_teamid] == ANY_TEAM) && tmp_det){
				MineComponent[mobid][mine_health] = 0.0;
				ExplodeDynamicMine(mobid,MINE_DESTROY_TYPE_DETECT,playerid);
			}
		}
	}	
	#if defined MDM_OnPlayerEnterDynamicArea
		MDM_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA areaid);
	#endif
	return 1;
}

#if defined _ALS_OnPlayerEnterDynamicArea
	#undef OnPlayerEnterDynamicArea
#else
	#define _ALS_OnPlayerEnterDynamicArea
#endif
#define OnPlayerEnterDynamicArea MDM_OnPlayerEnterDynamicArea
#if defined MDM_OnPlayerEnterDynamicArea
	forward MDM_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA areaid);
#endif

//EOF
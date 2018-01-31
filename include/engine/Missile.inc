/**********************************************************************************************************************************
 *                                                                                                                                *
 *                                                        Missile Launcher                                                        *
 *                                                                                                                                *
 * Copyright © 2018 Abyss Morgan. All rights reserved.                                                                            *
 *                                                                                                                                *
 * Website:  http://8.ct8.pl                                                                                                      *
 * Download: http://8.ct8.pl/r/download                                                                                           *
 *                                                                                                                                *
 * Plugins: Streamer                                                                                                              *
 * Modules: 3DTryg                                                                                                                *
 *                                                                                                                                *
 * File Version: 2.2.0                                                                                                            *
 * Streamer Version: 2.9.1                                                                                                        *
 * 3DTryg Version: 4.6.0                                                                                                          *
 *                                                                                                                                *
 * Functions:                                                                                                                     *
 * MissileLaunch(type,Float:detection_range,Float:explode_radius,Float:speed,Float:x,Float:y,Float:z,worldid,interiorid,playerid, *
 *               Float:streamdistance,missile_object,Float:tx,Float:ty,Float:tz,byplayerid=INVALID_PLAYER_ID,teamid=ANY_TEAM,     *
 *               bool:remote=false,remote_count=MAX_MISSILE_REMOTE_TARGET,remote_height=1.0,Float:arc_roll=0.0,Float:damage=-1.0, *
 *               Float:vehicle_damage=-1.0);                                                                                      *
 * MissileLaunchAimPlayer(targetid,type,Float:detection_range,Float:explode_radius,Float:speed,Float:x,Float:y,Float:z,           *
 *                        worldid,interiorid,playerid,Float:streamdistance,missile_object,Float:tx,Float:ty,Float:tz,             *
 *                        byplayerid=INVALID_PLAYER_ID,teamid=ANY_TEAM,Float:damage=-1.0,Float:vehicle_damage=-1.0);              *
 * bool:MissileStopAimPlayer(mobid);                                                                                              *
 * MissileDestroy(mobid,type,playerid);                                                                                           *
 * MissileRemoteTarget(mobid,Float:tx,Float:ty,Float:tz,Float:speed = 0.0);                                                       *
 * Float:GetMissileExplodeVisibility();                                                                                           *
 * SetMissileExplodeVisibility(Float:explode_stream);                                                                             *
 * DisableAllPlayerMissileAim(playerid);                                                                                          *
 *                                                                                                                                *
 * Callbacks:                                                                                                                     *
 * OnMissileDestroy(mobid,Float:x,Float:y,Float:z,type,killerid,byplayerid);                                                      *
 * OnMissileDetectPlayer(playerid,killerid);                                                                                      *
 * OnMissileRequestLaunch(Float:x,Float:y,Float:z,playerid,targetid);                                                             *
 *                                                                                                                                *
 **********************************************************************************************************************************/

/*
//Check Version Missile.inc
#if !defined _missile_launcher
	#error [ADM] You need Missile.inc v2.2.0 (8.ct8.pl/d/Missile.inc)
#elseif !defined Missile_Version
	#error [ADM] Update you Missile.inc to v2.2.0 (8.ct8.pl/d/Missile.inc)
#elseif (Missile_Version < 20200)
	#error [ADM] Update you Missile.inc to v2.2.0 (8.ct8.pl/d/Missile.inc)
#endif
*/

#if defined _missile_launcher
	#endinput
#endif
#define _missile_launcher
#define DYNAMIC_FILTERSCRIPT				"ALS:HOOK:METHOD"

#if defined _samp_included
	#if (!defined GetPlayerPoolSize || !defined GetSVarInt)
		#error [ADM] This include requires SA:MP version 0.3.7 (8.ct8.pl/r/download)
	#endif
#elseif defined _rwmp_included
	#error [ADM] This game currently is not supported
#else
	#error [ADM] Not found any general game includes
#endif

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

#define Missile_Version						(20200)

#define MISSILE_OBJECT_NORMAL				(0)
#define MISSILE_OBJECT_BIG					(1)
#define MISSILE_OBJECT_HYDRA				(2)
#define MISSILE_OBJECT_BOMB					(3)
#define MISSILE_OBJECT_BOMB_STATIC			(4)

#if !defined MAX_MISSILE
	#define MAX_MISSILE						(2048)
#endif

#if !defined MAX_MISSILE_REMOTE_TARGET
	#define MAX_MISSILE_REMOTE_TARGET		(34)
#endif

#define MISSILE_EXTRA_ID_OFFSET				(300001)
#define MISSILE_REMOTE_PLAYER				(999)

#define MISSILE_TYPE_EXPLODE_SMALL			(12)
#define MISSILE_TYPE_EXPLODE_NORMAL			(2)
#define MISSILE_TYPE_EXPLODE_LARGE			(6)
#define MISSILE_TYPE_EXPLODE_HUGE			(7)

#define MISSILE_DESTROY_TYPE_TARGET			(0)	//When rocket reaches its destination (always killerid=INVALID_PLAYER_ID)
#define MISSILE_DESTROY_TYPE_DETECT			(1)	//When rocket has been destroyed by detection range
#define MISSILE_DESTROY_TYPE_KILL			(2)	//When rocket has been destroyed by player

#define INVALID_MISSILE_ID					(0)

#if !defined ANY_TEAM
	#define ANY_TEAM						(0xFFFF)
#endif

enum e_missile {
	bool:missile_active,
	STREAMER_TAG_OBJECT missile_objectid,
	STREAMER_TAG_AREA missile_areaid,
	missile_type,
	Float:missile_explode,
	missile_playerid,
	missile_teamid,
	missile_vehicleid,
	missile_remote_count,
	missile_remote_id,
	Float:missile_speed,
	player_target,
	Float:missile_dmg,
	Float:missile_veh_dmg
}

new MissileComponent[MAX_MISSILE][e_missile],
	Float:MissilePoints[MAX_MISSILE][MAX_MISSILE_REMOTE_TARGET][3],
	bool:PlayerAimByMissile[MAX_PLAYERS],
	MissileUsedUpperBound = -1,
	Float:MissileExplosionVisibility = 200.0;

#define GetMissileExplodeVisibility()		MissileExplosionVisibility
#define SetMissileExplodeVisibility(%0)		MissileExplosionVisibility = (%0)

stock FindFreeMissileID(){
	for(new i = 1; i < MAX_MISSILE; i++){
		if(!MissileComponent[i][missile_active]) return i;
	}
	return INVALID_MISSILE_ID;
}

forward OnMissileDestroy(mobid,Float:x,Float:y,Float:z,type,killerid,byplayerid);
forward OnMissileDetectPlayer(playerid,killerid);
#if defined OnMissileRequestLaunch
	forward OnMissileRequestLaunch(Float:x,Float:y,Float:z,playerid,targetid);
#endif

stock MissileLaunch(type,Float:detection_range,Float:explode_radius,Float:speed,Float:x,Float:y,Float:z,worldid,interiorid,playerid, \
	Float:streamdistance,missile_object,Float:tx,Float:ty,Float:tz,byplayerid=INVALID_PLAYER_ID,teamid=ANY_TEAM, \
	bool:remote=false,remote_count=MAX_MISSILE_REMOTE_TARGET,Float:remote_height=1.0,Float:arc_roll=0.0,Float:damage=-1.0,Float:vehicle_damage=-1.0){
	
	#if defined OnMissileRequestLaunch
		if(!OnMissileRequestLaunch(tx,ty,tz,byplayerid,INVALID_PLAYER_ID)) return INVALID_MISSILE_ID;
	#endif
	
	new mobid = FindFreeMissileID();
	if(mobid == INVALID_MISSILE_ID){
		printf("[ADM] Error: Failed to MissileLaunch limit %d exceeded, add #define MAX_MISSILE before Missile.inc",MAX_MISSILE);
		return INVALID_MISSILE_ID;
	}
	if(mobid > MissileUsedUpperBound) MissileUsedUpperBound = mobid;
	
	new Float:rx, Float:rz;
	GetRotationFor2Point3D(x,y,z,tx,ty,tz,rx,rz);
	switch(missile_object){
		case MISSILE_OBJECT_NORMAL: {
			CompRotationFloat(rz-90.0,rz);
			MissileComponent[mobid][missile_objectid] = CreateDynamicObject(3790,x,y,z,0.0,rx,rz,worldid,interiorid,playerid,streamdistance,streamdistance);
		}
		case MISSILE_OBJECT_BIG: {
			CompRotationFloat(rz-90.0,rz);
			MissileComponent[mobid][missile_objectid] = CreateDynamicObject(3786,x,y,z,0.0,rx,rz,worldid,interiorid,playerid,streamdistance,streamdistance);
		}
		case MISSILE_OBJECT_HYDRA: {
			MissileComponent[mobid][missile_objectid] = CreateDynamicObject(345,x,y,z,rx,0.0,rz,worldid,interiorid,playerid,streamdistance,streamdistance);
		}
		case MISSILE_OBJECT_BOMB: {
			MissileComponent[mobid][missile_objectid] = CreateDynamicObject(1636,x,y,z,rx,0.0,rz,worldid,interiorid,playerid,streamdistance,streamdistance);
		}
		case MISSILE_OBJECT_BOMB_STATIC: {
			MissileComponent[mobid][missile_objectid] = CreateDynamicObject(1636,x,y,z,0.0,0.0,rz,worldid,interiorid,playerid,streamdistance,streamdistance);
		}
	}
	MissileComponent[mobid][missile_explode] = explode_radius;
	MissileComponent[mobid][missile_playerid] = byplayerid;
	MissileComponent[mobid][missile_teamid] = teamid;
	MissileComponent[mobid][missile_speed] = speed;
	MissileComponent[mobid][player_target] = INVALID_PLAYER_ID;
	MissileComponent[mobid][missile_dmg] = damage;
	MissileComponent[mobid][missile_veh_dmg] = vehicle_damage;
	
	if(MissileComponent[mobid][missile_dmg] != -1 && MissileComponent[mobid][missile_veh_dmg] == -1) MissileComponent[mobid][missile_veh_dmg] = 750.0;
	
	if(remote && remote_count > 0){
		if(remote_height <= 0.0) remote_height = 1.0;
		
		if(remote_count > MAX_MISSILE_REMOTE_TARGET) remote_count = MAX_MISSILE_REMOTE_TARGET;
		MissileComponent[mobid][missile_remote_count] = remote_count-2;
		MissileComponent[mobid][missile_remote_id] = 2;
		
		GetArcPoints3D(x,y,z,tx,ty,tz,arc_roll,remote_height,MissilePoints[mobid],remote_count);
		
		#if defined Tryg3D_ColAndreas
			new rid = remote_count-1;
			tx = MissilePoints[mobid][rid][0];
			ty = MissilePoints[mobid][rid][1];
			tz = MissilePoints[mobid][rid][2];
			
			MovePointCol(tx,ty,tz,tx,ty,tz-10000.0,MissilePoints[mobid][rid][0],MissilePoints[mobid][rid][1],MissilePoints[mobid][rid][2]);
		#endif
		
		tx = MissilePoints[mobid][1][0];
		ty = MissilePoints[mobid][1][1];
		tz = MissilePoints[mobid][1][2];
		
		GetRotationFor2Point3D(x,y,z,tx,ty,tz,rx,rz);
		switch(missile_object){
			case MISSILE_OBJECT_NORMAL: {
				CompRotationFloat(rz-90.0,rz);
				SetDynamicObjectRot(MissileComponent[mobid][missile_objectid],0.0,rx,rz);
			}
			case MISSILE_OBJECT_BIG: {
				CompRotationFloat(rz-90.0,rz);
				SetDynamicObjectRot(MissileComponent[mobid][missile_objectid],0.0,rx,rz);
			}
			case MISSILE_OBJECT_HYDRA: {
				SetDynamicObjectRot(MissileComponent[mobid][missile_objectid],rx,0.0,rz);
			}
			case MISSILE_OBJECT_BOMB: {
				SetDynamicObjectRot(MissileComponent[mobid][missile_objectid],rx,0.0,rz);
			}
			case MISSILE_OBJECT_BOMB_STATIC: {
				SetDynamicObjectRot(MissileComponent[mobid][missile_objectid],0.0,0.0,rz);
			}
		}
	} else {
		MissileComponent[mobid][missile_remote_count] = 0;
	}
	MissileComponent[mobid][missile_vehicleid] = GetPlayerVehicleID(byplayerid);
	
	MissileComponent[mobid][missile_areaid] = CreateDynamicSphere(x,y,z,detection_range,worldid,interiorid,playerid);
	AttachDynamicAreaToObject(MissileComponent[mobid][missile_areaid],MissileComponent[mobid][missile_objectid]);
	
	Streamer::SetIntData(STREAMER_TYPE_OBJECT,MissileComponent[mobid][missile_objectid],E_STREAMER_EXTRA_ID,(mobid+MISSILE_EXTRA_ID_OFFSET));
	Streamer::SetIntData(STREAMER_TYPE_AREA,MissileComponent[mobid][missile_areaid],E_STREAMER_EXTRA_ID,(mobid+MISSILE_EXTRA_ID_OFFSET));
	MissileComponent[mobid][missile_type] = type;
	
	switch(missile_object){
		case MISSILE_OBJECT_HYDRA: {
			MoveDynamicObject(MissileComponent[mobid][missile_objectid],tx,ty,tz,speed,rx,0.0,rz);
		}
		case MISSILE_OBJECT_BOMB: {
			MoveDynamicObject(MissileComponent[mobid][missile_objectid],tx,ty,tz,speed,rx,0.0,rz);
		}
		case MISSILE_OBJECT_BOMB_STATIC: {
			if(remote && remote_count > 0){
				MoveDynamicObject(MissileComponent[mobid][missile_objectid],tx,ty,tz,speed,rx,0.0,rz);
			} else {
				MoveDynamicObject(MissileComponent[mobid][missile_objectid],tx,ty,tz,speed,0.0,0.0,rz);
			}
		}
		default: {
			MoveDynamicObject(MissileComponent[mobid][missile_objectid],tx,ty,tz,speed,0.0,rx,rz);
		}
	}
	MissileComponent[mobid][missile_active] = true;
	return mobid;
}

stock MissileLaunchAimPlayer(targetid,type,Float:detection_range,Float:explode_radius,Float:speed,Float:x,Float:y,Float:z,worldid,interiorid,playerid, \
	Float:streamdistance,missile_object,Float:tx,Float:ty,Float:tz,byplayerid=INVALID_PLAYER_ID,teamid=ANY_TEAM,Float:damage=-1.0,Float:vehicle_damage=-1.0){
	
	#if defined OnMissileRequestLaunch
		if(!OnMissileRequestLaunch(tx,ty,tz,byplayerid,targetid)) return INVALID_MISSILE_ID;
	#endif
	
	new mobid = FindFreeMissileID();
	if(mobid == INVALID_MISSILE_ID){
		printf("[ADM] Error: Failed to MissileLaunch limit %d exceeded, add #define MAX_MISSILE before Missile.inc",MAX_MISSILE);
		return INVALID_MISSILE_ID;
	}
	if(mobid > MissileUsedUpperBound) MissileUsedUpperBound = mobid;
	
	new Float:rx, Float:rz;
	GetRotationFor2Point3D(x,y,z,tx,ty,tz,rx,rz);
	switch(missile_object){
		case MISSILE_OBJECT_NORMAL: {
			CompRotationFloat(rz-90.0,rz);
			MissileComponent[mobid][missile_objectid] = CreateDynamicObject(3790,x,y,z,0.0,rx,rz,worldid,interiorid,playerid,streamdistance,streamdistance);
		}
		case MISSILE_OBJECT_BIG: {
			CompRotationFloat(rz-90.0,rz);
			MissileComponent[mobid][missile_objectid] = CreateDynamicObject(3786,x,y,z,0.0,rx,rz,worldid,interiorid,playerid,streamdistance,streamdistance);
		}
		case MISSILE_OBJECT_HYDRA: {
			MissileComponent[mobid][missile_objectid] = CreateDynamicObject(345,x,y,z,rx,0.0,rz,worldid,interiorid,playerid,streamdistance,streamdistance);
		}
		case MISSILE_OBJECT_BOMB, MISSILE_OBJECT_BOMB_STATIC: {
			MissileComponent[mobid][missile_objectid] = CreateDynamicObject(1636,x,y,z,rx,0.0,rz,worldid,interiorid,playerid,streamdistance,streamdistance);
		}
	}
	MissileComponent[mobid][missile_explode] = explode_radius;
	MissileComponent[mobid][missile_playerid] = byplayerid;
	MissileComponent[mobid][missile_teamid] = teamid;
	MissileComponent[mobid][missile_speed] = speed;
	MissileComponent[mobid][missile_remote_count] = MISSILE_REMOTE_PLAYER;
	MissileComponent[mobid][missile_vehicleid] = GetPlayerVehicleID(byplayerid);
	MissileComponent[mobid][player_target] = targetid;
	
	MissileComponent[mobid][missile_dmg] = damage;
	MissileComponent[mobid][missile_veh_dmg] = vehicle_damage;
	
	if(MissileComponent[mobid][missile_dmg] != -1 && MissileComponent[mobid][missile_veh_dmg] == -1) MissileComponent[mobid][missile_veh_dmg] = 750.0;
	
	PlayerAimByMissile[targetid] = true;
	MissileComponent[mobid][missile_areaid] = CreateDynamicSphere(x,y,z,detection_range,worldid,interiorid,playerid);
	AttachDynamicAreaToObject(MissileComponent[mobid][missile_areaid],MissileComponent[mobid][missile_objectid]);
	
	Streamer::SetIntData(STREAMER_TYPE_OBJECT,MissileComponent[mobid][missile_objectid],E_STREAMER_EXTRA_ID,(mobid+MISSILE_EXTRA_ID_OFFSET));
	Streamer::SetIntData(STREAMER_TYPE_AREA,MissileComponent[mobid][missile_areaid],E_STREAMER_EXTRA_ID,(mobid+MISSILE_EXTRA_ID_OFFSET));
	MissileComponent[mobid][missile_type] = type;
	
	switch(missile_object){
		case MISSILE_OBJECT_HYDRA: {
			MoveDynamicObject(MissileComponent[mobid][missile_objectid],tx,ty,tz,speed,rx,0.0,rz);
		}
		case MISSILE_OBJECT_BOMB: {
			MoveDynamicObject(MissileComponent[mobid][missile_objectid],tx,ty,tz,speed,rx,0.0,rz);
		}
		case MISSILE_OBJECT_BOMB_STATIC: {
			MoveDynamicObject(MissileComponent[mobid][missile_objectid],tx,ty,tz,speed,rx,0.0,rz);
		}
		default: {
			MoveDynamicObject(MissileComponent[mobid][missile_objectid],tx,ty,tz,speed,0.0,rx,rz);
		}
	}
	MissileComponent[mobid][missile_active] = true;
	return mobid;
}

stock bool:MissileStopAimPlayer(mobid){
	if(MissileComponent[mobid][missile_active]){
		if(MissileComponent[mobid][missile_remote_count] == MISSILE_REMOTE_PLAYER){
			if(!IsPlayerConnected(MissileComponent[mobid][player_target])){
				MissileComponent[mobid][player_target] = INVALID_PLAYER_ID;
				MissileComponent[mobid][missile_remote_count] = 0;
				return true;
			}
		}
	}
	return false;
}

stock DisableAllPlayerMissileAim(playerid){
	PlayerAimByMissile[playerid] = false;
	new cnt = 0;
	for(new mobid = 0; mobid <= MissileUsedUpperBound; mobid++){
		if(MissileComponent[mobid][player_target] == playerid){
			MissileStopAimPlayer(mobid);
			cnt++;
		}
	}
	return cnt;
}

stock MissileRemoteTarget(mobid,Float:tx,Float:ty,Float:tz,Float:speed = 0.0){
	if(mobid <= 0 || mobid >= MAX_MISSILE) return 0;
	if(!MissileComponent[mobid][missile_active]) return 0;
	if(speed == 0.0) speed = MissileComponent[mobid][missile_speed];
	new missile_modelid = Streamer::GetIntData(STREAMER_TYPE_OBJECT,MissileComponent[mobid][missile_objectid],E_STREAMER_MODEL_ID),
		Float:x ,Float:y, Float:z, Float:rx, Float:rz;
	GetDynamicObjectPos(MissileComponent[mobid][missile_objectid],x,y,z);
	GetRotationFor2Point3D(x,y,z,tx,ty,tz,rx,rz);
	switch(missile_modelid){
		case 345, 1636: {
			SetDynamicObjectRot(MissileComponent[mobid][missile_objectid],rx,0.0,rz);
			MoveDynamicObject(MissileComponent[mobid][missile_objectid],tx,ty,tz,speed,rx,0.0,rz);
		}
		
		case 3790, 3786: {
			CompRotationFloat(rz-90.0,rz);
			SetDynamicObjectRot(MissileComponent[mobid][missile_objectid],0.0,rx,rz);
			MoveDynamicObject(MissileComponent[mobid][missile_objectid],tx,ty,tz,speed,0.0,rx,rz);
		}
	}
	return 1;
}

stock MissileDestroy(mobid,type,playerid){
	if(mobid <= 0 || mobid >= MAX_MISSILE) return 0;
	if(!MissileComponent[mobid][missile_active]) return 0;

	new Float:x, Float:y, Float:z,
		worldid = Streamer::GetIntData(STREAMER_TYPE_OBJECT,MissileComponent[mobid][missile_objectid],E_STREAMER_WORLD_ID),
		interiorid = Streamer::GetIntData(STREAMER_TYPE_OBJECT,MissileComponent[mobid][missile_objectid],E_STREAMER_INTERIOR_ID);
	
	GetDynamicObjectPos(MissileComponent[mobid][missile_objectid],x,y,z);
	
	if(MissileComponent[mobid][missile_dmg] == -1.0){
		CreateDynamicExplosion(x,y,z,MissileComponent[mobid][missile_type],MissileComponent[mobid][missile_explode],worldid,interiorid,-1,GetMissileExplodeVisibility());
	} else {
		CreateDynamicExplosionDMG(x,y,z,MissileComponent[mobid][missile_type],MissileComponent[mobid][missile_explode],worldid,interiorid,-1,GetMissileExplodeVisibility(),MissileComponent[mobid][missile_dmg],MissileComponent[mobid][missile_veh_dmg],MissileComponent[mobid][missile_playerid]);
	}
	if(IsAnyPlayerInDynamicArea(MissileComponent[mobid][missile_areaid],1)){
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++){
			if(IsPlayerConnected(i)){
				if(IsPlayerInDynamicArea(i,MissileComponent[mobid][missile_areaid])){
					CallRemoteFunction("OnMissileDetectPlayer","dd",i,MissileComponent[mobid][missile_playerid]);
				}
			}
		}
	}
	if(IsValidDynamicObject(MissileComponent[mobid][missile_objectid])) DestroyDynamicObject(MissileComponent[mobid][missile_objectid]);
	if(IsValidDynamicArea(MissileComponent[mobid][missile_areaid])) DestroyDynamicArea(MissileComponent[mobid][missile_areaid]);
	new byplayerid = MissileComponent[mobid][missile_playerid];
	MissileComponent[mobid][missile_objectid] = STREAMER_TAG_OBJECT INVALID_STREAMER_ID;
	MissileComponent[mobid][missile_areaid] = STREAMER_TAG_AREA	INVALID_STREAMER_ID;
	MissileComponent[mobid][missile_type] = 0;
	MissileComponent[mobid][missile_explode] = 0.0;
	MissileComponent[mobid][missile_playerid] = INVALID_PLAYER_ID;
	MissileComponent[mobid][missile_teamid] = ANY_TEAM;
	MissileComponent[mobid][missile_vehicleid] = 0;
	MissileComponent[mobid][missile_speed] = 0.0;
	MissileComponent[mobid][missile_remote_count] = 0;
	MissileComponent[mobid][missile_remote_id] = 0;
	MissileComponent[mobid][missile_dmg] = -1.0;
	MissileComponent[mobid][missile_veh_dmg] = -1.0;
	MissileComponent[mobid][player_target] = INVALID_PLAYER_ID;
	MissileComponent[mobid][missile_active] = false;
	CallLocalFunction("OnMissileDestroy","dfffddd",mobid,x,y,z,type,playerid,byplayerid);
	if(mobid == MissileUsedUpperBound) MissileUsedUpperBound--;
	return 1;
}

//Hook: OnPlayerShootDynamicObject
public OnPlayerShootDynamicObject(playerid,weaponid,STREAMER_TAG_OBJECT objectid,Float:x,Float:y,Float:z){
	if(IsValidDynamicObject(objectid)){
		new mobid = (Streamer::GetIntData(STREAMER_TYPE_OBJECT,objectid,E_STREAMER_EXTRA_ID)-MISSILE_EXTRA_ID_OFFSET);
		if(mobid > 0 && mobid < MAX_MISSILE){
			if(MissileComponent[mobid][missile_active] && MissileComponent[mobid][missile_objectid] == objectid){
				if((MissileComponent[mobid][missile_teamid] != GetPlayerTeam(playerid)) || (MissileComponent[mobid][missile_teamid] == ANY_TEAM)){
					MissileDestroy(mobid,MISSILE_DESTROY_TYPE_KILL,playerid);
				}
			}
		}
	}
	#if defined MISOnPlayerShootDynamicObject
		MISOnPlayerShootDynamicObject(playerid,weaponid,objectid,x,y,z);
	#endif
	return 1;
}

#if defined _ALS_OnPlayerShootDynamicObject
	#undef OnPlayerShootDynamicObject
#else
	#define _ALS_OnPlayerShootDynamicObject
#endif
#define OnPlayerShootDynamicObject MISOnPlayerShootDynamicObject
#if defined MISOnPlayerShootDynamicObject
	forward MISOnPlayerShootDynamicObject(playerid,weaponid,STREAMER_TAG_OBJECT objectid,Float:x,Float:y,Float:z);
#endif

//Hook: OnPlayerEnterDynamicArea
public OnPlayerEnterDynamicArea(playerid,STREAMER_TAG_AREA areaid){
	new pstate = GetPlayerState(playerid);
	if(pstate != 1 && pstate != 2 && pstate != 3) return 1;
	
	new mobid = (Streamer::GetIntData(STREAMER_TYPE_AREA,areaid,E_STREAMER_EXTRA_ID)-MISSILE_EXTRA_ID_OFFSET);
	if(mobid > 0 && mobid < MAX_MISSILE){
		if(MissileComponent[mobid][missile_active] && MissileComponent[mobid][missile_playerid] != playerid && MissileComponent[mobid][missile_areaid] == areaid){
			if((MissileComponent[mobid][missile_teamid] != GetPlayerTeam(playerid)) || (MissileComponent[mobid][missile_teamid] == ANY_TEAM)){
				if((MissileComponent[mobid][missile_vehicleid] != GetPlayerVehicleID(playerid)) || (MissileComponent[mobid][missile_vehicleid] == 0)){
					MissileDestroy(mobid,MISSILE_DESTROY_TYPE_DETECT,playerid);
				}
			}
		}
	}	
	#if defined MIS_OnPlayerEnterDynamicArea
		MIS_OnPlayerEnterDynamicArea(playerid,areaid);
	#endif
	return 1;
}

#if defined _ALS_OnPlayerEnterDynamicArea
	#undef OnPlayerEnterDynamicArea
#else
	#define _ALS_OnPlayerEnterDynamicArea
#endif
#define OnPlayerEnterDynamicArea MIS_OnPlayerEnterDynamicArea
#if defined MIS_OnPlayerEnterDynamicArea
	forward MIS_OnPlayerEnterDynamicArea(playerid,STREAMER_TAG_AREA areaid);
#endif

//Hook: OnDynamicObjectMoved
public OnDynamicObjectMoved(STREAMER_TAG_OBJECT objectid){
	if(IsValidDynamicObject(objectid)){
		new mobid = (Streamer::GetIntData(STREAMER_TYPE_OBJECT,objectid,E_STREAMER_EXTRA_ID)-MISSILE_EXTRA_ID_OFFSET);
		if(mobid > 0 && mobid < MAX_MISSILE){
			if(MissileComponent[mobid][missile_active] && MissileComponent[mobid][missile_objectid] == objectid){
				if(MissileComponent[mobid][missile_remote_count] == MISSILE_REMOTE_PLAYER){
					if(!IsPlayerConnected(MissileComponent[mobid][player_target])){
						MissileComponent[mobid][player_target] = INVALID_PLAYER_ID;
						MissileComponent[mobid][missile_remote_count] = 0;
					}
				}
				if(MissileComponent[mobid][missile_remote_count] <= 0){
					MissileDestroy(mobid,MISSILE_DESTROY_TYPE_TARGET,INVALID_PLAYER_ID);
				} else if(MissileComponent[mobid][missile_remote_count] == MISSILE_REMOTE_PLAYER){
					new Float:px,Float:py,Float:pz;
					GetPlayerPos(MissileComponent[mobid][player_target],px,py,pz);
					#if defined COLANDREAS
						new Float:x, Float:y, Float:z, Float:tx, Float:ty, Float:tz;
						GetDynamicObjectPos(objectid,x,y,z);
						MovePointCol(x,y,z,px,py,pz,tx,ty,tz);
						if(tx != px || ty != py || tz != pz){
							//collision
							MissileComponent[mobid][missile_remote_count] = 0;
							MissileRemoteTarget(mobid,tx,ty,tz);
						} else {
							MissileRemoteTarget(mobid,tx,ty,tz);
						}
					#else
						MissileRemoteTarget(mobid,px,py,pz);
					#endif
				} else {
					MissileComponent[mobid][missile_remote_count]--;
					new remote_id = MissileComponent[mobid][missile_remote_id]++;
					#if defined COLANDREAS
						new Float:x, Float:y, Float:z, Float:tx, Float:ty, Float:tz;
						GetDynamicObjectPos(objectid,x,y,z);
						MovePointCol(x,y,z,MissilePoints[mobid][remote_id][0],MissilePoints[mobid][remote_id][1],MissilePoints[mobid][remote_id][2],tx,ty,tz);
						if(tx != MissilePoints[mobid][remote_id][0] || ty != MissilePoints[mobid][remote_id][1] || tz != MissilePoints[mobid][remote_id][2]){
							//collision
							MissileComponent[mobid][missile_remote_count] = 0;
							MissileRemoteTarget(mobid,tx,ty,tz);
						} else {
							MissileRemoteTarget(mobid,tx,ty,tz);
						}
					#else
						MissileRemoteTarget(mobid,MissilePoints[mobid][remote_id][0],MissilePoints[mobid][remote_id][1],MissilePoints[mobid][remote_id][2]);
					#endif
				}
			}
		}
	}
	#if defined MIS_OnDynamicObjectMoved
		MIS_OnDynamicObjectMoved(objectid);
	#endif
	return 1;
}

#if defined _ALS_OnDynamicObjectMoved
	#undef OnDynamicObjectMoved
#else
	#define _ALS_OnDynamicObjectMoved
#endif
#define OnDynamicObjectMoved MIS_OnDynamicObjectMoved
#if defined MIS_OnDynamicObjectMoved
	forward MIS_OnDynamicObjectMoved(STREAMER_TAG_OBJECT objectid);
#endif

//Hook: OnPlayerDeath
public OnPlayerDeath(playerid, killerid, reason){
	if(PlayerAimByMissile[playerid]){
		DisableAllPlayerMissileAim(playerid);
	}
	#if defined MIS_OnPlayerDeath
		MIS_OnPlayerDeath(playerid, killerid, reason);
	#endif
	return 1;
}

#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath MIS_OnPlayerDeath
#if defined MIS_OnPlayerDeath
	forward MIS_OnPlayerDeath(playerid, killerid, reason);
#endif

//Hook: OnPlayerConnect
public OnPlayerConnect(playerid){
	
	Streamer::ToggleIdleUpdate(playerid,1);
	
	#if defined MIS_OnPlayerConnect
		MIS_OnPlayerConnect(playerid);
	#endif
	return 1;
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect MIS_OnPlayerConnect
#if defined MIS_OnPlayerConnect
	forward MIS_OnPlayerConnect(playerid);
#endif

//EOF
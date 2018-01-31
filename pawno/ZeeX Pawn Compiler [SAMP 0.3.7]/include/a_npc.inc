/*  SA-MP NPC Functions
 *
 *  (c) Copyright 2009, SA-MP Team
 *
 */

#if defined _samp_included
	#endinput
#endif
#define _samp_included
#pragma library samp

#pragma tabsize 4

#include <core>
#include <float>
#include <string>
#include <file>
#include <time>
#include <datagram>

// --------------------------------------------------
// Natives
// --------------------------------------------------

#if !defined gpci
	native gpci(playerid, serial[], len);
#endif

// Util
native print(const string[]);
native printf(const format[], {Float,_}:...);
native format(output[], len, const format[], {Float,_}:...);
native SetTimer(funcname[], interval, repeating);
native KillTimer(timerid);
native GetTickCount();
native Float:asin(Float:value);
native Float:acos(Float:value);
native Float:atan(Float:value);
native Float:atan2(Float:x, Float:y);

native SendChat(msg[]);
native SendCommand(commandtext[]);

native GetPlayerState(playerid);
native GetPlayerPos(playerid, &Float:x, &Float:y, &Float:z);
native GetPlayerVehicleID(playerid);
native GetPlayerArmedWeapon(playerid);
native GetPlayerHealth(playerid);
native GetPlayerArmour(playerid);
native GetPlayerSpecialAction(playerid);
native IsPlayerStreamedIn(playerid);
native IsVehicleStreamedIn(vehicleid);
native GetPlayerKeys(playerid, &keys, &updown, &leftright);
native GetPlayerFacingAngle(playerid, &Float:ang);
native GetMyPos(&Float:x, &Float:y, &Float:z);
native SetMyPos(Float:x, Float:y, Float:z);
native GetMyFacingAngle(&Float:ang);
native SetMyFacingAngle(Float:ang);

native GetDistanceFromMeToPoint(Float:X, Float:Y, Float:Z, &Float:Distance);
native IsPlayerInRangeOfPoint(playerid, Float:range, Float:X, Float:Y, Float:Z);

native GetPlayerName(playerid, const name[], len);
native IsPlayerConnected(playerid);

#define PLAYER_RECORDING_TYPE_NONE		0
#define PLAYER_RECORDING_TYPE_DRIVER	1
#define PLAYER_RECORDING_TYPE_ONFOOT	2

native StartRecordingPlayback(playbacktype, recordname[]);
native StopRecordingPlayback();
native PauseRecordingPlayback();
native ResumeRecordingPlayback();

// --------------------------------------------------
// Defines
// --------------------------------------------------

// States
#define PLAYER_STATE_NONE						(0)
#define PLAYER_STATE_ONFOOT						(1)
#define PLAYER_STATE_DRIVER						(2)
#define PLAYER_STATE_PASSENGER					(3)
#define PLAYER_STATE_WASTED						(7)
#define PLAYER_STATE_SPAWNED					(8)
#define PLAYER_STATE_SPECTATING					(9)

// Misc
#define MAX_PLAYER_NAME							(24)
#define MAX_PLAYERS								(1000)
#define MAX_VEHICLES							(2000)
#define MAX_ACTORS								(1000)
#define INVALID_PLAYER_ID						(0xFFFF)
#define INVALID_VEHICLE_ID						(0xFFFF)
#define INVALID_ACTOR_ID						(0xFFFF)
#define NO_TEAM									(255)
#define MAX_OBJECTS								(1000)
#define INVALID_OBJECT_ID						(0xFFFF)
#define MAX_GANG_ZONES							(1024)
#define MAX_TEXT_DRAWS							(2048)
#define MAX_PLAYER_TEXT_DRAWS					(256)
#define MAX_MENUS								(128)
#define MAX_3DTEXT_GLOBAL						(1024)
#define MAX_3DTEXT_PLAYER						(1024)
#define MAX_PICKUPS								(4096)
#define INVALID_MENU							(0xFF)
#define INVALID_TEXT_DRAW						(0xFFFF)
#define INVALID_GANG_ZONE						(-1)
#define INVALID_3DTEXT_ID						(0xFFFF)
#define INVALID_PICKUP_ID						(-1)

// Weapons
#define WEAPON_FIST						(0)
#define WEAPON_BRASSKNUCKLE				(1)
#define WEAPON_GOLFCLUB					(2)
#define WEAPON_NITESTICK				(3)
#define WEAPON_KNIFE					(4)
#define WEAPON_BAT						(5)
#define WEAPON_SHOVEL					(6)
#define WEAPON_POOLSTICK				(7)
#define WEAPON_KATANA					(8)
#define WEAPON_CHAINSAW					(9)
#define WEAPON_DILDO					(10)
#define WEAPON_DILDO2					(11)
#define WEAPON_VIBRATOR					(12)
#define WEAPON_VIBRATOR2				(13)
#define WEAPON_FLOWER					(14)
#define WEAPON_CANE						(15)
#define WEAPON_GRENADE					(16)
#define WEAPON_TEARGAS					(17)
#define WEAPON_MOLTOV					(18)
//Unknown								(19)
//Unknown								(20)
//Unknown								(21)
#define WEAPON_COLT45					(22)
#define WEAPON_SILENCED					(23)
#define WEAPON_DEAGLE					(24)
#define WEAPON_SHOTGUN					(25)
#define WEAPON_SAWEDOFF					(26)
#define WEAPON_SHOTGSPA					(27)
#define WEAPON_UZI						(28)
#define WEAPON_MP5						(29)
#define WEAPON_AK47						(30)
#define WEAPON_M4						(31)
#define WEAPON_TEC9						(32)
#define WEAPON_RIFLE					(33)
#define WEAPON_SNIPER					(34)
#define WEAPON_ROCKETLAUNCHER			(35)
#define WEAPON_HEATSEEKER				(36)
#define WEAPON_FLAMETHROWER				(37)
#define WEAPON_MINIGUN					(38)
#define WEAPON_SATCHEL					(39)
#define WEAPON_BOMB						(40)
#define WEAPON_SPRAYCAN					(41)
#define WEAPON_FIREEXTINGUISHER			(42)
#define WEAPON_CAMERA					(43)
#define WEAPON_NIGHTGOGGLES				(44)
#define WEAPON_THERMALGOGGLES 			(45)
#define WEAPON_PARACHUTE				(46)
#define WEAPON_FAKEPISTOL				(47)
//Unknown								(48)
#define WEAPON_VEHICLE					(49)
#define WEAPON_HELICOPTERBLADES			(50)
#define WEAPON_EXPLOSION				(51)
//Unknown								(52)
#define WEAPON_DROWN					(53)
#define WEAPON_COLLISION				(54)
#define WEAPON_SUICIDE					(255)

// Keys
#define KEY_ACTION						(1)
#define KEY_CROUCH						(2)
#define KEY_FIRE						(4)
#define KEY_SPRINT						(8)
#define KEY_SECONDARY_ATTACK			(16)
#define KEY_JUMP						(32)
#define KEY_LOOK_RIGHT					(64)
#define KEY_HANDBRAKE					(128)
#define KEY_LOOK_LEFT					(256)
#define KEY_SUBMISSION					(512)
#define KEY_LOOK_BEHIND					(512)
#define KEY_WALK						(1024)
#define KEY_ANALOG_UP					(2048)
#define KEY_ANALOG_DOWN					(4096)
#define KEY_ANALOG_LEFT					(8192)
#define KEY_ANALOG_RIGHT				(16384)
#define KEY_YES							(65536)
#define KEY_NO							(131072)
#define KEY_CTRL_BACK					(262144)

#define KEY_UP							(-128)
#define KEY_DOWN						(128)
#define KEY_LEFT						(-128)
#define KEY_RIGHT						(128)
// --------------------------------------------------
// Forwards (Callback declarations)
// --------------------------------------------------

forward OnNPCModeInit();
forward OnNPCModeExit();
forward OnNPCConnect(myplayerid);
forward OnNPCDisconnect(reason[]);
forward OnNPCSpawn();
forward OnNPCEnterVehicle(vehicleid, seatid);
forward OnNPCExitVehicle();
forward OnClientMessage(color, text[]);
forward OnPlayerDeath(playerid);
forward OnPlayerText(playerid, text[]);
forward OnPlayerStreamIn(playerid);
forward OnPlayerStreamOut(playerid);
forward OnVehicleStreamIn(vehicleid);
forward OnVehicleStreamOut(vehicleid);
forward OnRecordingPlaybackEnd();

// --------------------------------------------------
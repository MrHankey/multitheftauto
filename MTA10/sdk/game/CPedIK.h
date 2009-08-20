/*****************************************************************************
*
*  PROJECT:		Multi Theft Auto v1.0
*  LICENSE:		See LICENSE in the top level directory
*  FILE:		sdk/game/CPedIK.h
*  PURPOSE:		Ped inverse kinematics interface
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAME_PEDIK
#define __CGAME_PEDIK

#include <windows.h>
#include <CVector.h>

// Ped IK flags
enum {
	PEDIK_GUN_REACHED_TARGET	= 0x1,
	PEDIK_TORSO_USED			= 0x2,
	PEDIK_USE_ARM				= 0x4,
	PEDIK_SLOPE_PITCH 			= 0x08,
	PEDIK_EVERYTHING_USED		= 0x10
};	

// Return flags from MoveLimb() function
enum MoveLimbResult {
	CANT_REACH_TARGET,
	HAVENT_REACHED_TARGET,
	REACHED_TARGET
};

// Structure containing info about a limb
typedef struct {
	float maxYaw, minYaw;
	float yawD;
	float maxPitch, minPitch;
	float pitchD;
} LimbMovementInfo;

// Structure containing current state of limb
struct LimbOrientation {
	float yaw, pitch;
};

class CPedIK
{
public:
	virtual void SetFlag(DWORD flag)=0;
	virtual void ClearFlag(DWORD flag)=0;
	virtual bool IsFlagSet(DWORD flag)=0;
};

#endif
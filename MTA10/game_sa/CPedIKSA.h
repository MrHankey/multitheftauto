/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        game_sa/CPedIKSA.h
*  PURPOSE:     Header file for ped entity inverse kinematics class
*  DEVELOPERS:  Ed Lyons <eai@opencoding.net>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAMESA_PEDIK
#define __CGAMESA_PEDIK

#include <game/CPedIK.h>
#include "Common.h"
#include <CVector.h>

class CPedSAInterface;

/********* END ***********/

class CPedIKSAInterface
{
public:
	CPedSAInterface	* ThisPed;			// 528
	LimbOrientation m_torsoOrien;
	float m_fSlopePitch;
	float m_fSlopePitchLimitMult;
	float m_fSlopeRoll;
	float m_fBodyRoll;
	DWORD m_flags;
};

class CPedIKSA : public CPedIK
{
private:
	CPedIKSAInterface		* internalInterface;
public:
	// constructor
	CPedIKSA(CPedIKSAInterface * ikInterface) { internalInterface = ikInterface; };

	// r*'s functions
	void SetFlag(DWORD flag)		{this->internalInterface->m_flags |= flag;}
	void ClearFlag(DWORD flag)		{this->internalInterface->m_flags &= ~flag;}
    bool IsFlagSet(DWORD flag)		{return ((this->internalInterface->m_flags & flag) > 0 ? 1 : 0);}	
};

#endif
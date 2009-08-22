/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/deathmatch/logic/CDeathmatchVehicle.h
*  PURPOSE:     Header for deathmatch vehicle class
*  DEVELOPERS:  Christian Myhre Lundheim <>
*               Jax <>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CDEATHMATCHVEHICLE_H
#define __CDEATHMATCHVEHICLE_H

#include "CClientVehicle.h"

class CDeathmatchVehicle : public CClientVehicle
{
public:
                                        CDeathmatchVehicle              ( CClientManager* pManager, class CUnoccupiedVehicleSync* pUnoccupiedVehicleSync, ElementID ID, unsigned short usVehicleModel  );
                                        ~CDeathmatchVehicle             ( void );

    inline bool                         IsSyncing                       ( void )                        { return m_bIsSyncing; };
    void                                SetIsSyncing                    ( bool bIsSyncing );

    bool                                SyncDamageModel                 ( void );
    void                                ResetDamageModelSync            ( void );

    void                                UpdateSyncTimes                 ( void );

    // Overloaded
    void                                SetAdjustablePropertyValue      ( unsigned short usValue );
    void                                SetTurretRotation               ( float fHorizontal, float fVertical );

protected:
    bool                                UseInterpolation                ( void );
    void                                StreamedInPulse                 ( void );

private:
    class CUnoccupiedVehicleSync*       m_pUnoccupiedVehicleSync;
    bool                                m_bIsSyncing;

    unsigned char                       m_ucLastDoorStates [MAX_DOORS];
    unsigned char                       m_ucLastWheelStates [MAX_WHEELS];
    unsigned char                       m_ucLastPanelStates [MAX_PANELS];
    unsigned char                       m_ucLastLightStates [MAX_LIGHTS];

public:
    unsigned long                       m_ulLastSyncTick;
    unsigned long                       m_ulSyncFrequency;

private:
    CInterpolatedVar < unsigned short > m_adjustableProperty;
    CInterpolatedVar < float >          m_turretX, m_turretY;
};

#endif

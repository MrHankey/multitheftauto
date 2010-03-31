/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/deathmatch/logic/CHandlingManager.h
*  PURPOSE:     Header file for vehicle handling manager class
*  DEVELOPERS:  Christian Myhre Lundheim <>
*               Florian Busse <flobu@gmx.net>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

class CHandlingManager;

#ifndef __CHANDLINGMANAGER_H
#define __CHANDLINGMANAGER_H

#include "CHandlingEntry.h"

class CHandlingManager
{
public:
                                CHandlingManager              ( void );
                                ~CHandlingManager             ( void );

    void                        LoadDefaultHandlings            ( void );

    CHandlingEntry*             CreateHandlingData              ( void );
    bool                        ApplyHandlingData               ( enum eVehicleTypes eModel, CHandlingEntry* pEntry );
    void                        RemoveFromVeh                   ( CVehicle* pVeh );

    CHandlingEntry*             GetHandlingData                 ( eVehicleTypes eModel );
    const CHandlingEntry*       GetOriginalHandlingData         ( eVehicleTypes eModel );
    CHandlingEntry*             GetOriginalHandlingTable        ( eHandlingTypes eHandling );
    CHandlingEntry*             GetPreviousHandlingTable        ( eHandlingTypes eHandling );
    float                       GetDragMultiplier               ( void );
    float                       GetBasicDragCoeff               ( void );
    eHandlingTypes              GetHandlingID                   ( eVehicleTypes eModel );

private:
    void                        InitializeDefaultHandlings      ( void );

    static DWORD                m_dwStore_LoadHandlingCfg;

    static void                 LoadHandlingCfg                 ( void );
    static void                 Hook_LoadHandlingCfg            ( void );

    // Original handling data unaffected by handling.cfg changes
    static tHandlingData        m_OriginalHandlingData [HT_MAX];

    // Our wrapper classes for the classes GTA use and the original data
    static CHandlingEntry*      m_pEntries [HT_MAX];
    static CHandlingEntry*      m_pOriginalEntries [HT_MAX];

    // These are the entries GTA use
    static tHandlingData        m_RealHandlingData [HT_MAX];

    // Additional entries are saved here
    std::list < CHandlingEntry* > m_HandlingList;
};

#endif

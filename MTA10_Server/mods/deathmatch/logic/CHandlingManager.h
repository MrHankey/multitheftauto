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

    CHandlingEntry*             CreateHandlingData              ( void );

    const CHandlingEntry*       GetOriginalHandlingData         ( eVehicleTypes eModel );

    eHandlingTypes              GetHandlingID                   ( eVehicleTypes eModel );

private:
    void                        InitializeDefaultHandlings      ( void );

    // Original handling data unaffected by handling.cfg changes
    static tHandlingData        m_OriginalHandlingData [HT_MAX];

    // Our wrapper classes for the classes GTA use and the original data
    static CHandlingEntry*      m_pOriginalEntries [HT_MAX];
};

#endif

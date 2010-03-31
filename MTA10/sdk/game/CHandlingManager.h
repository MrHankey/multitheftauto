/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        sdk/game/CHandlingManager.h
*  PURPOSE:     Vehicle handling manager interface
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAME_CHandlingManager
#define __CGAME_CHandlingManager

#include "CHandlingEntry.h"

class CHandlingManager
{
public:
    virtual void                    LoadDefaultHandlings    ( void ) = 0;

    virtual CHandlingEntry*         CreateHandlingData      ( void ) = 0;
    virtual bool                    ApplyHandlingData       ( enum eVehicleTypes eModel, CHandlingEntry* pEntry ) = 0;

    virtual float                   GetDragMultiplier       ( void ) = 0;
    virtual CHandlingEntry*         GetHandlingData         ( enum eVehicleTypes eModel ) = 0;
    virtual const CHandlingEntry*   GetOriginalHandlingData ( enum eVehicleTypes eModel ) = 0;
};

#endif

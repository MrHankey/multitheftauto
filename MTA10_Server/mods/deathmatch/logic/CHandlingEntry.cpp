/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/deathmatch/logic/CHandlingEntry.cpp
*  PURPOSE:     Vehicle handling data entry
*  DEVELOPERS:  Christian Myhre Lundheim <>
*               The_GTA <>
*               Florian Busse <flobu@gmx.net>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include "StdInc.h"

CHandlingEntry::CHandlingEntry ( void )
{
    // Create a new interface and zero it
    m_pHandling = new tHandlingData;
    memset ( m_pHandling, 0, sizeof ( tHandlingData ) );
    m_bDeleteInterface = true;

    // We have no original data
    m_pOriginalData = NULL;
}


CHandlingEntry::CHandlingEntry ( tHandlingData* pData, tHandlingData* pOriginalUncalculatedData )
{
    // Store gta's pointer
    m_pHandling = pData;
    m_bDeleteInterface = false;
    m_pOriginalData = pOriginalUncalculatedData;
}


CHandlingEntry::CHandlingEntry ( tHandlingData* pOriginal )
{
    // Store gta's pointer
    m_pHandling = NULL;
    m_pOriginalData = NULL;
    m_bDeleteInterface = false;
    memcpy ( &m_Handling, pOriginal, sizeof ( tHandlingData ) );
}


CHandlingEntry::~CHandlingEntry ( void )
{
    if ( m_bDeleteInterface )
    {
        delete m_pHandling;
    }
}

// Does vehicle entry already exist?
bool CHandlingEntry::IsVehicleAdded ( CVehicle* pVeh )
{
//  std::list < CVehicle* > ::iterator iter = m_VehicleList.begin ();
//  for ( ; iter != m_VehicleList.begin (); iter++ )
//  {
//      if (*iter == pVeh)
//          return true;
//  }
    return false;
}

// Adds a vehicle to list
void CHandlingEntry::AddVehicle ( CVehicle* pVeh )
{
//  if ( IsVehicleAdded ( pVeh ) )
//      return;
//  m_VehicleList.push_front ( dynamic_cast < CVehicle* > ( pVeh ) );
}


// Apply the handlingdata from another data
void CHandlingEntry::ApplyHandlingData ( CHandlingEntry* pData )
{
    // Copy the data
    CHandlingEntry* pEntry = static_cast < CHandlingEntry* > ( pData );
    m_Handling = pEntry->m_Handling;
}


// Remove a vehicle from list
void CHandlingEntry::RemoveVehicle ( CVehicle* pVeh )
{
//  m_VehicleList.remove ( dynamic_cast < CVehicle* > ( pVeh ) );
}


void CHandlingEntry::Recalculate ( void )
{
    // Real GTA class?
    if ( m_pHandling )
    {
        // Copy our stored field to GTA's
        memcpy ( m_pHandling, &m_Handling, sizeof ( m_Handling ) );

        // Call GTA's function that calculates the final values from the read values
        DWORD dwFunc = 0x6F5080;
        DWORD dwHandling = reinterpret_cast < DWORD > ( m_pHandling );
        _asm
        {
            push        dwHandling
            call        dwFunc
        }
    }
}


void CHandlingEntry::Restore ( void )
{
    // Real GTA class?
    if ( m_pOriginalData )
    {
        // Copy default stuff over gta's data
        memcpy ( &m_Handling, m_pOriginalData, sizeof ( tHandlingData ) );
    }
}

/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/deathmatch/logic/CDeathmatchObject.cpp
*  PURPOSE:     Object element interface
*  DEVELOPERS:  Christian Myhre Lundheim <>
*               Jax <>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include "StdInc.h"

using std::list;

extern CClientGame * g_pClientGame;

CDeathmatchObject::CDeathmatchObject ( CClientManager* pManager, CMovingObjectsManager* pMovingObjectsManager, ElementID ID, unsigned short usModel ) : CClientObject ( pManager, ID, usModel )
{
    m_pMovingObjectsManager = pMovingObjectsManager;
}


CDeathmatchObject::~CDeathmatchObject ( void )
{
    m_pMovingObjectsManager->Remove ( this );
}


void CDeathmatchObject::StartMovement ( const CVector& vecTargetPosition, const CVector& vecTargetRotation, unsigned long ulTime )
{
    GetPosition ( m_movePosition );
    GetRotationRadians ( m_moveRotation );
    NormalizeRadian ( m_moveRotation );
    m_movePosition.lerp ( vecTargetPosition, ulTime );
    m_moveRotation.lerp ( m_moveRotation + vecTargetRotation, ulTime );

    // Add us to the moving object's manager
    m_pMovingObjectsManager->Add ( this );
}


void CDeathmatchObject::StopMovement ( void )
{
    m_movePosition.finish ();
    m_moveRotation.finish ();
}


void CDeathmatchObject::UpdateMovement ( void )
{
    if ( !m_movePosition.finished () || !m_moveRotation.finished () )
    {
        CVector vecPreviousPosition = m_vecPosition;
        CVector vecPreviousRotation = m_vecRotation;

        CClientObject::SetOrientation ( m_movePosition.update (), m_moveRotation.update () );

        UpdateContactingBegin ( vecPreviousPosition, vecPreviousRotation );        
    }
}


//
// SetPosition with update of contacting peds
//
void CDeathmatchObject::SetPosition ( const CVector& vecPosition )
{
    // Don't allow if we're still moving
    if ( !m_movePosition.finished () ) return;

    CVector vecPreviousPosition = m_vecPosition;
    CVector vecPreviousRotation = m_vecRotation;

    // Call original
    CClientObject::SetPosition( vecPosition );

    // Update our contact players
    UpdateContactingBegin( vecPreviousPosition, vecPreviousRotation );
}


//
// SetRotationRadians with update of contacting peds
//
void CDeathmatchObject::SetRotationRadians ( const CVector& vecRotationRadians )
{
    // Don't allow if we're still moving
    if ( !m_moveRotation.finished () ) return;

    CVector vecPreviousPosition = m_vecPosition;
    CVector vecPreviousRotation = m_vecRotation;

    // Call original
    CClientObject::SetRotationRadians( vecRotationRadians );

    // Update our contact players
    UpdateContactingBegin( vecPreviousPosition, vecPreviousRotation );
}


//
// SetOrientation with update of contacting peds
//
void CDeathmatchObject::SetOrientation ( const CVector& vecPosition, const CVector& vecRotationRadians )
{
    // Don't allow if we're still moving
    if ( !m_movePosition.finished () || !m_moveRotation.finished () ) return;

    CVector vecPreviousPosition = m_vecPosition;
    CVector vecPreviousRotation = m_vecRotation;

    // Call original
    CClientObject::SetOrientation( vecPosition, vecRotationRadians );

    // Update our contact players
    UpdateContactingBegin( vecPreviousPosition, vecPreviousRotation );
}


//
// Prepare vars for UpdateContacting
//
void CDeathmatchObject::UpdateContactingBegin ( const CVector& vecPreviousPosition, const CVector& vecPreviousRotation )
{
    // Calc frame delta
    CVector vecFrameRotation    = m_vecRotation - vecPreviousRotation;
    CVector vecFrameTranslation = m_vecPosition - vecPreviousPosition;
    CVector vecCenterOfRotation = m_vecPosition;

    // Rotate frame rotation x/y by absolute rotation z, for some reason
    RotateVector ( vecFrameRotation, CVector(0, 0, -vecPreviousRotation.fZ ) );

    UpdateContacting ( vecCenterOfRotation, vecFrameTranslation, vecFrameRotation );
}


//
// Apply object movement to contacting peds
//
void CDeathmatchObject::UpdateContacting ( const CVector& vecCenterOfRotation, const CVector& vecFrameTranslation, const CVector& vecFrameRotation )
{
    bool bHasRotation    = ( vecFrameRotation.fX != 0 || vecFrameRotation.fY != 0 || vecFrameRotation.fZ != 0 );
    bool bHasTranslation = ( vecFrameTranslation.fX != 0 || vecFrameTranslation.fY != 0 || vecFrameTranslation.fZ != 0 );

    // Early out if no orientation change here
    if ( !bHasRotation && !bHasTranslation )
        return;

    // Step through each contacting ped
    list < CClientPed * > ::iterator iter = m_Contacts.begin ();
    for ( ; iter != m_Contacts.end () ; iter++ )
    {
        CClientPed* pPed = *iter;

        // Get ped start position
        CVector vecPlayerPosition;
        pPed->GetPosition ( vecPlayerPosition );

        // Apply rotation effect on position
        if ( bHasRotation )
        {
            vecPlayerPosition -= vecCenterOfRotation;
            RotateVector ( vecPlayerPosition, CVector(-vecFrameRotation.fX, -vecFrameRotation.fY, -vecFrameRotation.fZ ) );
            vecPlayerPosition += vecCenterOfRotation;
        }

        // Apply translation
        vecPlayerPosition += vecFrameTranslation;
        pPed->SetPosition ( vecPlayerPosition );

        // Also change ped facing direction
        if ( vecFrameRotation.fZ != 0 )
        {
            float fRotationZ = pPed->GetCurrentRotation ();
            pPed->SetCurrentRotation ( fRotationZ + vecFrameRotation.fZ );
        }
    }

    // Look in attached objects for more ped contacts
    list < CClientEntity * > ::iterator itera = m_AttachedEntities.begin ();
    for ( ; itera != m_AttachedEntities.end () ; itera++ )
    {
        CClientEntity* pEntity = *itera;
        if ( IS_OBJECT ( pEntity ) )
        {
            CDeathmatchObject* pObject = static_cast < CDeathmatchObject* > ( pEntity );
            pObject->UpdateContacting( vecCenterOfRotation, vecFrameTranslation, vecFrameRotation );
        }
    }
}









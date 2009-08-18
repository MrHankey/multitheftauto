/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/deathmatch/logic/CDeathmatchObject.h
*  PURPOSE:     Header for deathmatch object class
*  DEVELOPERS:  Christian Myhre Lundheim <>
*               Jax <>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CDEATHMATCHOBJECT_H
#define __CDEATHMATCHOBJECT_H

#include "CClientObject.h"

class CDeathmatchObject : public CClientObject
{
public:
                                        CDeathmatchObject               ( CClientManager* pManager, class CMovingObjectsManager* pMovingObjectsManager, ElementID ID, unsigned short usModel );
                                        ~CDeathmatchObject              ( void );

    // CClientEntity interface
    void                                SetPosition                     ( const CVector& vecPosition );

    // CClientObject interface
    void                                SetRotationRadians              ( const CVector& vecRotation );
    void                                SetOrientation                  ( const CVector& vecPosition, const CVector& vecRotationRadians );

    // CDeathmatchObject functions
    inline const CVector&               GetStartPosition                ( void )        { return m_movePosition.begin; };
    inline const CVector&               GetStartRotation                ( void )        { return m_moveRotation.begin; };
    inline const CVector&               GetTargetPosition               ( void )        { return m_movePosition.end; };
    inline const CVector&               GetTargetRotation               ( void )        { return m_moveRotation.end; };
    inline void                         SetTargetRotation               ( const CVector& vecRotation ) { m_moveRotation.end = vecRotation; }
    inline unsigned long                GetStartTime                    ( void )        { return m_movePosition.beginTime; };
    inline unsigned long                GetTargetTime                   ( void )        { return m_movePosition.endTime; };

    void                                StartMovement                   ( const CVector& vecTargetPosition, const CVector& vecTargetRotation, unsigned long ulTime );
    void                                StopMovement                    ( void );
    void                                UpdateMovement                  ( void );
    
    void                                UpdateContactingBegin           ( const CVector& vecPreviousPosition, const CVector& vecPreviousRotation );
    void                                UpdateContacting                ( const CVector& vecCenterOfRotation, const CVector& vecFrameTranslation, const CVector& vecFrameRotation );

    inline bool                         IsMoving                        ( void )        { return !m_movePosition.finished (); }

protected:
    class CMovingObjectsManager*        m_pMovingObjectsManager;

    CInterpolatedVar < CVector >        m_movePosition;
    CInterpolatedVar < CVector >        m_moveRotation;
};

#endif

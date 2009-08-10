/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/deathmatch/logic/CInterpolator.h
*  PURPOSE:     Header for interpolator class
*  DEVELOPERS:  Kent Simon <>
*               Christian Myhre Lundheim <>
*               Jax <>
*               
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#pragma once

#include <list>
#include "Utils.h"


// the logic may seem backwards in some places, because i'm inserting at the front, so the list is 
// reversed to what you'd think, like a stack
template < class T >
class CInterpolator
{
private:
    struct VecMap
    {
        unsigned long   m_ulTime;
        T               data;
    };

public:
    CInterpolator       ( )
    {
    }

    ~CInterpolator      ( )
    {
        m_listNodes.clear ();
    }

    void                Push                ( const T& newData, unsigned long ulTime )
    {
        m_listNodes.push_front ( VecMap () );
        VecMap& map = m_listNodes.front ();
        map.data = newData;
        map.m_ulTime = ulTime;
    }

    bool                Evaluate            ( unsigned long ulTime, T* output )
    {
        if ( m_listNodes.empty () )
            return false;

	    // Time later than newest point, so use the newest point
        VecMap& front = m_listNodes.front ();
        if ( ulTime > front.m_ulTime )
        {
            *output = front.data;
            return true;
        }

    	// Find the two points either side and interpolate
        std::list < VecMap >::const_iterator iterPrev = m_listNodes.begin ();
        std::list < VecMap >::const_iterator iter = m_listNodes.begin ();
        for ( ++iter; iter != m_listNodes.end (); ++iter )
        {
            const VecMap& currentMap = *iter;
            if ( ulTime > currentMap.m_ulTime )
                return Eval ( *iterPrev, currentMap, ulTime, output );
            iterPrev = iter;
        }

	    // Time earlier than oldest point, so use the oldest point
        VecMap& back = m_listNodes.back ();
        *output = back.data;
        return true;
    }

    void                Pop                 ( )
    {
        if ( !m_listNodes.empty () )
            m_listNodes.pop_back ();
    }

    unsigned long       GetOldestEntry      ( T* output )
    {
        VecMap& map = m_listNodes.back ();
        *output = map.data;
        return map.m_ulTime;
    }

private:
    bool                Eval                ( const VecMap& Left,
                                              const VecMap& Right,
                                              unsigned long ulTimeEval,
                                              T* output )
    {
        // Check for being the same or maybe wrap around
        if ( Right.m_ulTime >= Left.m_ulTime )
        {
            *output = Left.data;
            return true;
        }

        // Find the relative position of ulTimeEval between R.m_ulTimeStamp and L.m_ulTimeStamp
        float fAlpha = Unlerp ( Right.m_ulTime, ulTimeEval, Left.m_ulTime );

        // Lerp between Right.pos and Left.pos
        *output = Lerp ( Right.data, fAlpha, Left.data );
        return true;
    }

private:
    std::list < VecMap >  m_listNodes;
};

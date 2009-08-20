/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/shared_logic/CClientSpecialCharacters.h
*  PURPOSE:     Special characters class
*  DEVELOPERS:  Jax <>
*
*****************************************************************************/

#ifndef __CCLIENTSPECIALCHARACTERS_H
#define __CCLIENTSPECIALCHARACTERS_H

#define NUM_SPECIAL_CHARS 37

#include <vector>

class CClientSpecialCharacters
{
public:
                                    ~CClientSpecialCharacters       ( void );

    static bool                     IsSpecialChar                   ( const char * szName );
    static unsigned char            GetID                           ( const char * szName );
    static const char *             GetName                         ( unsigned char ID );

    bool                            ReplaceModel                    ( unsigned short usModel, const char * szName );
    bool                            IsModelReplaced                 ( unsigned short usModel );
    bool                            RestoreModel                    ( unsigned short usModel );
    void                            RestoreAllModels                ( void );

private:
    static const char *             m_szSpecialChars [ NUM_SPECIAL_CHARS ];
    std::vector < unsigned short >  m_replacedModels;
};

#endif
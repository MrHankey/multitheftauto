/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/shared_logic/CClientSpecialCharacters.cpp
*  PURPOSE:     Special characters class
*  DEVELOPERS:  Jax <>
*
*****************************************************************************/

#include <StdInc.h>
using namespace std;

const char * CClientSpecialCharacters::m_szSpecialChars [ NUM_SPECIAL_CHARS ] =
{ "TENPEN", "PULASKI", "HERN", "SMOKE", "SWEET", "RYDER", "RYDER2", "RYDER3",
  "EMMET", "ANDRE", "OGLOC", "CESAR", "KENDL", "WUZIMU", "CAT", "TRUTH", "JETHRO",
  "DWAYNE", "ZERO", "TBONE", "JIZZY", "TORINO", "SINDACO", "MACCER", "PAUL", "ROSE",
  "FORELLI", "MADDOGG", "CROGRL2", "JANITOR", "BBTHIN", "SMOKEV", "GANGRL2", "MECGRL2",
  "GUNGRL2", "COPGRL2", "NURGRL2" };

CClientSpecialCharacters::~CClientSpecialCharacters ( void )
{
    RestoreAllModels ();
}


bool CClientSpecialCharacters::IsSpecialChar ( const char * szName )
{
    return ( GetID ( szName ) != 0xFF );
}


unsigned char CClientSpecialCharacters::GetID ( const char * szName )
{
    for ( unsigned char i = 0 ; i < NUM_SPECIAL_CHARS ; i++ )
    {
        if ( !stricmp ( m_szSpecialChars [ i ], szName ) )
        {
            return i;
        }
    }
    return 0xFF;
}


const char * CClientSpecialCharacters::GetName ( unsigned char ID )
{
    if ( ID < NUM_SPECIAL_CHARS )
    {
        return m_szSpecialChars [ ID ];
    }
    return NULL;
}


bool CClientSpecialCharacters::ReplaceModel ( unsigned short usModel, const char * szName )
{
    if ( CClientPlayerManager::IsValidModel ( usModel ) )
    {
        if ( IsSpecialChar ( szName ) )
        {
            CModelInfo * pInfo = g_pGame->GetModelInfo ( usModel );
            if ( pInfo )
            {
                pInfo->ReplaceTexture ( szName );
                if ( !IsModelReplaced ( usModel ) ) m_replacedModels.push_back ( usModel );

                return true;
            }
        }
    }
    return false;
}


bool CClientSpecialCharacters::IsModelReplaced ( unsigned short usModel )
{
    vector < unsigned short > ::iterator iter = m_replacedModels.begin ();
    for ( ; iter != m_replacedModels.end () ; iter++ )
    {
        if ( *iter == usModel ) return true;
    }
    return false;
}


bool CClientSpecialCharacters::RestoreModel ( unsigned short usModel )
{
    if ( IsModelReplaced ( usModel ) )
    {
        CModelInfo * pInfo = g_pGame->GetModelInfo ( usModel );
        pInfo->RestoreTexture ();

        vector < unsigned short > ::iterator iter = m_replacedModels.begin ();
        for ( ; iter != m_replacedModels.end () ; iter++ )
        {
            if ( *iter == usModel )
            {
                m_replacedModels.erase ( iter );
                break;
            }
        }

        return true;
    }
    return false;
}


void CClientSpecialCharacters::RestoreAllModels ( void )
{
    CModelInfo * pInfo;
    vector < unsigned short > ::iterator iter = m_replacedModels.begin ();
    for ( ; iter != m_replacedModels.end () ; iter++ )
    {
        pInfo = g_pGame->GetModelInfo ( *iter );
        pInfo->RestoreTexture ();
    }
    m_replacedModels.clear ();
}
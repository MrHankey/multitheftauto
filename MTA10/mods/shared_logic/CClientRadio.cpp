/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/shared_logic/CClientRadio.cpp
*  PURPOSE:     Radio class
*  DEVELOPERS:  Jax <>
*
*****************************************************************************/

#include "StdInc.h"

#define RADIO_COLOR 0xFF106290
#define RADIO_TUNE_COLOR 0xFF666666
#define RADIO_DROP_COLOR 0xFF000000
#define RADIO_POS_X 0.5f
#define RADIO_POS_Y 0.05f
#define RADIO_SCALE_X 0.8f
#define RADIO_SCALE_Y 1.2f
#define CHANNEL_TUNE_TIME 1500
#define CHANNEL_DISPLAY_TIME 2500
#define RADIO_SOUND_TUNE 34
#define RADIO_SOUND_STOP_TUNE 35
#define RADIO_SOUND_SELECT 37

char * CClientRadio::m_ChannelNames [ NUM_RADIO_CHANNELS ] =
{
    "Police", "Playback FM",
    "K Rose", "K-DST",
    "Bounce FM", "SF-UR",
    "Radio Los Santos", "Radio X",
    "CSR 103.9", "K-Jah West",
    "Master Sounds 98.3", "WCTR",
    "User Track Player", "Radio Off"
};

CClientRadio::CClientRadio ( CClientManager * pManager )
{
    m_pManager = pManager;
    m_PreviousChannel = m_Channel = CHANNEL_PLAYBACK_FM;
    m_bPlaying = false;
    m_bTuning = false;
    m_bTuneForThisChannel = false;
    m_ulChangeTick = 0;
    m_bPoliceRadio = false;
}


CClientRadio::~CClientRadio ( void )
{
    if ( m_bPlaying ) Stop ();
}


void CClientRadio::DoPulse ( void )
{
    // Can we hear the radio? do we use the police radio?
    bool bCanHearRadio = false;
    CClientPlayer * pLocalPlayer = m_pManager->GetPlayerManager ()->GetLocalPlayer ();
    if ( pLocalPlayer )
    {
        CClientVehicle * pVehicle = pLocalPlayer->GetRealOccupiedVehicle ();
        if ( pVehicle && !pVehicle->IsBlown () && !pLocalPlayer->IsLeavingVehicle () &&
             pVehicle->HasRadio () )
        {
            bCanHearRadio = true;
            m_bPoliceRadio = pVehicle->HasPoliceRadio ();
        }            
    }

    // Start the radio after tuning
    if ( m_bPlaying && m_bTuning )
    {
        // Is it time to start playing?
        unsigned long ulTick = GetTickCount ();
        if ( m_ulChangeTick && ulTick >= ( m_ulChangeTick + CHANNEL_TUNE_TIME ) )
        {
            // Stop the tuning in
            m_bTuning = false;
            g_pGame->GetAudio ()->PlayFrontEndSound ( RADIO_SOUND_STOP_TUNE );

            // Are we still turned on?
            g_pGame->GetAudio ()->StartRadio ( m_Channel );
        }
    }

    // If we should be playing, start the radio and vice versa
    if ( bCanHearRadio && !IsPlaying () ) Start ();
    else if ( !bCanHearRadio && IsPlaying () ) Stop ();
}


void CClientRadio::Render ( void )
{    
    // Are we live n kickin?
    if ( IsPlaying () )
    {
        // Tuning takes 2.5secs and we display for another 2.5secs afterward
        unsigned long ulTick = GetTickCount ();
        unsigned long ulDisplayTime = CHANNEL_DISPLAY_TIME;
        if ( m_bTuneForThisChannel ) ulDisplayTime += CHANNEL_TUNE_TIME;

        // Are we still displaying?
        if ( m_ulChangeTick && ulTick <= ( m_ulChangeTick + ulDisplayTime ) )
        {    
            // We dont display the police channel
            if ( m_Channel != CHANNEL_POLICE )
            {
                const char * channelName = GetChannelName ();
                if ( channelName )
                {
                    unsigned long fontColor = ( m_bTuning ) ? RADIO_TUNE_COLOR : RADIO_COLOR;

                    // Position and scale correction for resolution
                    CVector2D vecRes = g_pCore->GetGUI ()->GetResolution ();
                    float fX = RADIO_POS_X * vecRes.fX, fY = RADIO_POS_Y * vecRes.fY;
                    float fScaleX = RADIO_SCALE_X * ( vecRes.fX / 800.0f ), fScaleY = RADIO_SCALE_Y * ( vecRes.fY / 600.0f );

                    // Draw the radio channel name
                    CFont * pFont = g_pGame->GetFont ();
                    pFont->SetFontStyle ( 2 );
                    pFont->SetJustify ( 0 );
                    pFont->SetBackground ( 0, 0 );
                    pFont->SetScale ( fScaleX, fScaleY );
                    pFont->SetProportional ( 1 );
                    pFont->SetOrientation ( 0 );
                    pFont->SetRightJustifyWrap ( 0 );
                    pFont->SetEdge ( 1 );
                    pFont->SetDropColor ( RADIO_DROP_COLOR );
                    pFont->SetColor ( fontColor );
                    pFont->PrintString ( fX, fY, const_cast < char * > ( channelName ) );
                    pFont->DrawFonts ();
                }
            }
        }
    }
}


bool CClientRadio::IsPlaying ( void )
{
    return m_bPlaying;
}


void CClientRadio::Start ( void )
{    
    // If we have a police channel, set us to it! ..and vice versa
    if ( m_bPoliceRadio && m_Channel != CHANNEL_POLICE )
    {
        // Save our current channel
        m_PreviousChannel = m_Channel;
        m_Channel = CHANNEL_POLICE;
    }
    // Restore our previous channel?
    else if ( !m_bPoliceRadio && m_Channel == CHANNEL_POLICE ) m_Channel = m_PreviousChannel;

    // Start the radio on our current channel
    g_pGame->GetAudio ()->StartRadio ( m_Channel );
    m_bPlaying = true;
    m_bTuning = false;
    m_bTuneForThisChannel = false;
    m_ulChangeTick = GetTickCount ();
}


void CClientRadio::Stop ( void )
{   
    // Stop the radio
    g_pGame->GetAudio ()->StopRadio ();
    if ( m_bTuning ) g_pGame->GetAudio ()->PlayFrontEndSound ( RADIO_SOUND_STOP_TUNE );
    m_bPlaying = false;
    m_bTuning = false;
    m_bTuneForThisChannel = false;
}


eRadioChannel CClientRadio::GetFirstChannel ( void )
{
    if ( m_bPoliceRadio ) return CHANNEL_POLICE;    
    return CHANNEL_PLAYBACK_FM;
}


eRadioChannel CClientRadio::GetLastChannel ( void )
{
    return CHANNEL_RADIO_OFF;
}


void CClientRadio::SetChannel ( eRadioChannel channel )
{
    // Different channel?
    if ( channel != m_Channel )
    {
        CClientPlayer * pLocalPlayer = m_pManager->GetPlayerManager ()->GetLocalPlayer ();
        if ( pLocalPlayer )
        {
            // Call the radio switch event on the local player
            CLuaArguments Arguments;
            Arguments.PushNumber ( ( unsigned char ) channel );
            if ( pLocalPlayer->CallEvent ( "onClientPlayerRadioSwitch", Arguments, true ) )
            {                    
                m_bTuning = m_bTuneForThisChannel = DoTuneForChannel ( channel );
                m_ulChangeTick = GetTickCount ();
                m_Channel = channel;

                // Save our current radio status by calling StopRadio
                g_pGame->GetAudio ()->StopRadio ();

                // Does this channel need to be tuned in first?
                if ( m_bTuneForThisChannel )
                {              
                    // Sound fx
                    g_pGame->GetAudio ()->PlayFrontEndSound ( RADIO_SOUND_TUNE );
                }
                // If not select it right now
                else
                {
                    // Sound fx
                    g_pGame->GetAudio ()->PlayFrontEndSound ( RADIO_SOUND_STOP_TUNE );
                    g_pGame->GetAudio ()->PlayFrontEndSound ( RADIO_SOUND_SELECT );

                    // Save our current radio status by calling StopRadio
                    g_pGame->GetAudio ()->StopRadio ();
                    g_pGame->GetAudio ()->StartRadio ( m_Channel );                    
                }
            }
        }
    }
}


bool CClientRadio::CanChangeChannel ( void )
{
    // We must be playing and not have a police a radio
    if ( m_bPlaying && !m_bPoliceRadio ) return true;
    return false;
}


const char * CClientRadio::GetChannelName ( void )
{
    return GetChannelName ( m_Channel );
}


const char * CClientRadio::GetChannelName ( eRadioChannel channel )
{
    if ( channel < NUM_RADIO_CHANNELS )
    {
        return m_ChannelNames [ channel ];
    }
    return NULL;
}


bool CClientRadio::DoTuneForChannel ( eRadioChannel channel )
{
    if ( channel == CHANNEL_RADIO_OFF || channel == CHANNEL_USER_TRACK_PLAYER ) return false;
    return true;
}


void CClientRadio::NextChannel ( void )
{
    // Can we change channel now?
    if ( CanChangeChannel () )
    {
        eRadioChannel channel = m_Channel;
        if ( channel == GetLastChannel () ) channel = GetFirstChannel ();
        else channel = eRadioChannel ( channel + 1 );
        SetChannel ( channel );
    }
}


void CClientRadio::PreviousChannel ( void )
{
    // Can we change channel now?
    if ( CanChangeChannel () )
    {
        eRadioChannel channel = m_Channel;
        if ( channel == GetFirstChannel () ) channel = GetLastChannel ();
        else channel = eRadioChannel ( channel - 1 );
        SetChannel ( channel );
    }
}
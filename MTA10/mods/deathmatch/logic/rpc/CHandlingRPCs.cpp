/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/deathmatch/logic/rpc/CHandlingRPCs.cpp
*  PURPOSE:     Handling remote procedure calls
*  DEVELOPERS:  Christian Myhre Lundheim <>
*               Jax <>
*               Florian Busse <flobu@gmx.net>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include <StdInc.h>
#include "CHandlingRPCs.h"

void CHandlingRPCs::LoadFunctions ( void )
{
    AddHandler ( SET_VEHICLE_HANDLING_PROPERTY, SetVehicleHandlingProperty, "SetVehicleHandlingProperty" );
    AddHandler ( RESET_VEHICLE_HANDLING_PROPERTY, RestoreVehicleHandlingProperty, "RestoreVehicleHandlingProperty" );
    AddHandler ( RESET_VEHICLE_HANDLING, RestoreVehicleHandling, "RestoreVehicleHandling" );
}


// Enum with property id's for handling
enum eHandlingProperty
{
    PROPERTY_MASS,
    PROPERTY_TURNMASS,
    PROPERTY_DRAGCOEFF,
    PROPERTY_CENTEROFMASS,
    PROPERTY_PERCENTSUBMERGED,
    PROPERTY_TRACTIONMULTIPLIER,
    PROPERTY_DRIVETYPE,
    PROPERTY_ENGINETYPE,
    PROPERTY_NUMOFGEARS,
    PROPERTY_ENGINEACCELLERATION,
    PROPERTY_ENGINEINERTIA,
    PROPERTY_MAXVELOCITY,
    PROPERTY_BRAKEDECELLERATION,
    PROPERTY_BRAKEBIAS,
    PROPERTY_ABS,
    PROPERTY_STEERINGLOCK,
    PROPERTY_TRACTIONLOSS,
    PROPERTY_TRACTIONBIAS,
    PROPERTY_SUSPENSION_FORCELEVEL,
    PROPERTY_SUSPENSION_DAMPING,
    PROPERTY_SUSPENSION_HIGHSPEEDDAMPING,
    PROPERTY_SUSPENSION_UPPER_LIMIT,
    PROPERTY_SUSPENSION_LOWER_LIMIT,
    PROPERTY_SUSPENSION_FRONTREARBIAS,
    PROPERTY_SUSPENSION_ANTIDIVEMULTIPLIER,
    PROPERTY_COLLISIONDAMAGEMULTIPLIER,
    PROPERTY_SEATOFFSETDISTANCE,
    PROPERTY_HANDLINGFLAGS,
    PROPERTY_MODELFLAGS,
    PROPERTY_HEADLIGHT,
    PROPERTY_TAILLIGHT,
    PROPERTY_ANIMGROUP,
};


void CHandlingRPCs::SetVehicleHandlingProperty ( NetBitStreamInterface& bitStream )
{
    // Read out the handling id and property id
    ElementID ID;
    unsigned char ucProperty;
    if ( bitStream.Read ( ID ) &&
         bitStream.Read ( ucProperty ) )
    {
        // Grab it and check its type
        CClientEntity* pEntity = CElementIDs::GetElement ( ID );
        if ( pEntity && pEntity->GetType () == CCLIENTVEHICLE )
        {
            // Grab the vehicle handling entry
            CClientVehicle& Vehicle = static_cast < CClientVehicle& > ( *pEntity );
            CHandlingEntry* pHandlingEntry = Vehicle.GetHandlingData();

            // Temporary storage for reading out data
            union
            {
                unsigned char ucChar;
                unsigned int uiInt;
                float fFloat;
            };

            // Depending on what property
            switch ( ucProperty )
            {
                case PROPERTY_MASS:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetMass ( fFloat );
                    break;
                    
                case PROPERTY_TURNMASS:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetTurnMass ( fFloat );
                    break;

                case PROPERTY_DRAGCOEFF:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetDragCoeff ( fFloat );
                    break;

                case PROPERTY_CENTEROFMASS:
                {
                    CVector vecVector;
                    bitStream.Read ( vecVector.fX );
                    bitStream.Read ( vecVector.fY );
                    bitStream.Read ( vecVector.fZ );
                    pHandlingEntry->SetCenterOfMass ( vecVector );
                    break;
                }

                case PROPERTY_PERCENTSUBMERGED:
                    bitStream.Read ( uiInt );
                    pHandlingEntry->SetPercentSubmerged ( uiInt );
                    break;

                case PROPERTY_TRACTIONMULTIPLIER:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetTractionMultiplier ( fFloat );
                    break;

                case PROPERTY_DRIVETYPE:
                {
                    bitStream.Read ( ucChar );
                    if ( ucChar != CHandlingEntry::FOURWHEEL &&
                         ucChar != CHandlingEntry::RWD &&
                         ucChar != CHandlingEntry::FWD )
                    {
                        ucChar = CHandlingEntry::RWD;
                    }

                    pHandlingEntry->SetCarDriveType ( static_cast < CHandlingEntry::eDriveType > ( ucChar ) );
                    break;
                }

                case PROPERTY_ENGINETYPE:
                {
                    bitStream.Read ( ucChar );
                    if ( ucChar != CHandlingEntry::DIESEL &&
                         ucChar != CHandlingEntry::ELECTRIC &&
                         ucChar != CHandlingEntry::PETROL )
                    {
                        ucChar = CHandlingEntry::PETROL;
                    }

                    pHandlingEntry->SetCarEngineType ( static_cast < CHandlingEntry::eEngineType > ( ucChar ) );
                    break;
                }

                case PROPERTY_NUMOFGEARS:
                    bitStream.Read ( ucChar );
                    pHandlingEntry->SetNumberOfGears ( ucChar );
                    break;

                case PROPERTY_ENGINEACCELLERATION:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetEngineAccelleration ( fFloat );
                    break;

                case PROPERTY_ENGINEINERTIA:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetEngineInertia ( fFloat );
                    break;

                case PROPERTY_MAXVELOCITY:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetMaxVelocity ( fFloat );
                    break;

                case PROPERTY_BRAKEDECELLERATION:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetBrakeDecelleration ( fFloat );
                    break;

                case PROPERTY_BRAKEBIAS:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetBrakeBias ( fFloat );
                    break;

                case PROPERTY_ABS:
                    bitStream.Read ( ucChar );
                    pHandlingEntry->SetABS ( ucChar != 0 );
                    break;

                case PROPERTY_STEERINGLOCK:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetSteeringLock ( fFloat );
                    break;

                case PROPERTY_TRACTIONLOSS:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetTractionLoss ( fFloat );
                    break;

                case PROPERTY_TRACTIONBIAS:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetTractionBias ( fFloat );
                    break;

                case PROPERTY_SUSPENSION_FORCELEVEL:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetSuspensionForceLevel ( fFloat );
                    break;

                case PROPERTY_SUSPENSION_DAMPING:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetSuspensionDamping ( fFloat );
                    break;

                case PROPERTY_SUSPENSION_HIGHSPEEDDAMPING:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetSuspensionHighSpeedDamping ( fFloat );
                    break;

                case PROPERTY_SUSPENSION_UPPER_LIMIT:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetSuspensionUpperLimit ( fFloat );
                    break;

                case PROPERTY_SUSPENSION_LOWER_LIMIT:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetSuspensionLowerLimit ( fFloat );
                    break;

                case PROPERTY_SUSPENSION_FRONTREARBIAS:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetSuspensionFrontRearBias ( fFloat );
                    break;

                case PROPERTY_SUSPENSION_ANTIDIVEMULTIPLIER:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetSuspensionAntidiveMultiplier ( fFloat );
                    break;

                case PROPERTY_COLLISIONDAMAGEMULTIPLIER:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetCollisionDamageMultiplier ( fFloat );
                    break;

                case PROPERTY_SEATOFFSETDISTANCE:
                    bitStream.Read ( fFloat );
                    pHandlingEntry->SetSeatOffsetDistance ( fFloat );
                    break;

                case PROPERTY_HANDLINGFLAGS:
                    bitStream.Read ( uiInt );
                    pHandlingEntry->SetHandlingFlags ( uiInt );
                    break;

                case PROPERTY_MODELFLAGS:
                    bitStream.Read ( uiInt );
                    pHandlingEntry->SetModelFlags ( uiInt );
                    break;

                case PROPERTY_HEADLIGHT:
                    bitStream.Read ( ucChar );
                    if ( ucChar > CHandlingEntry::TALL )
                        ucChar = CHandlingEntry::TALL;

                    pHandlingEntry->SetHeadLight ( static_cast < CHandlingEntry::eLightType > ( ucChar ) );
                    break;

                case PROPERTY_TAILLIGHT:
                    bitStream.Read ( ucChar );
                    if ( ucChar > CHandlingEntry::TALL )
                        ucChar = CHandlingEntry::TALL;

                    pHandlingEntry->SetTailLight ( static_cast < CHandlingEntry::eLightType > ( ucChar ) );
                    break;

                case PROPERTY_ANIMGROUP:
                    bitStream.Read ( ucChar );
                    pHandlingEntry->SetAnimGroup ( ucChar );
                    break;
            }

            Vehicle.ApplyHandling();
        }
    }
}


void CHandlingRPCs::RestoreVehicleHandlingProperty ( NetBitStreamInterface& bitStream )
{
    // Read out the handling id and property id
    ElementID ID;
    unsigned char ucProperty;
    if ( bitStream.Read ( ID ) &&
         bitStream.Read ( ucProperty ) )
    {
        // Grab it and check its type
        CClientEntity* pEntity = CElementIDs::GetElement ( ID );
        if ( pEntity && pEntity->GetType () == CCLIENTVEHICLE )
        {
            // Grab the vehicle handling entry and the original handling
            CClientVehicle& Vehicle = static_cast < CClientVehicle& > ( *pEntity );
            CHandlingEntry* pHandlingEntry = Vehicle.GetHandlingData ();
            const CHandlingEntry* pOriginalEntry = Vehicle.GetOriginalHandlingData ();

            // Depending on what property
            switch ( ucProperty )
            {
                case PROPERTY_MASS:
                    pHandlingEntry->SetMass ( pOriginalEntry->GetMass () );
                    break;
                    
                case PROPERTY_TURNMASS:
                    pHandlingEntry->SetTurnMass ( pOriginalEntry->GetTurnMass () );
                    break;

                case PROPERTY_DRAGCOEFF:
                    pHandlingEntry->SetDragCoeff ( pOriginalEntry->GetDragCoeff () );
                    break;

                case PROPERTY_CENTEROFMASS:
                    pHandlingEntry->SetCenterOfMass ( pOriginalEntry->GetCenterOfMass () );
                    break;

                case PROPERTY_PERCENTSUBMERGED:
                    pHandlingEntry->SetPercentSubmerged ( pOriginalEntry->GetPercentSubmerged () );
                    break;

                case PROPERTY_TRACTIONMULTIPLIER:
                    pHandlingEntry->SetTractionMultiplier ( pOriginalEntry->GetTractionMultiplier () );
                    break;

                case PROPERTY_DRIVETYPE:
                    pHandlingEntry->SetCarDriveType ( pOriginalEntry->GetCarDriveType () );
                    break;

                case PROPERTY_ENGINETYPE:
                    pHandlingEntry->SetCarEngineType ( pOriginalEntry->GetCarEngineType () );
                    break;

                case PROPERTY_NUMOFGEARS:
                    pHandlingEntry->SetNumberOfGears ( pOriginalEntry->GetNumberOfGears () );
                    break;

                case PROPERTY_ENGINEACCELLERATION:
                    pHandlingEntry->SetEngineAccelleration ( pOriginalEntry->GetEngineAccelleration () );
                    break;

                case PROPERTY_ENGINEINERTIA:
                    pHandlingEntry->SetEngineInertia ( pOriginalEntry->GetEngineInertia () );
                    break;

                case PROPERTY_MAXVELOCITY:
                    pHandlingEntry->SetMaxVelocity ( pOriginalEntry->GetMaxVelocity () );
                    break;

                case PROPERTY_BRAKEDECELLERATION:
                    pHandlingEntry->SetBrakeDecelleration ( pOriginalEntry->GetBrakeDecelleration () );
                    break;

                case PROPERTY_BRAKEBIAS:
                    pHandlingEntry->SetBrakeBias ( pOriginalEntry->GetBrakeBias () );
                    break;

                case PROPERTY_ABS:
                    pHandlingEntry->SetABS ( pOriginalEntry->GetABS () );
                    break;

                case PROPERTY_STEERINGLOCK:
                    pHandlingEntry->SetSteeringLock ( pOriginalEntry->GetSteeringLock () );
                    break;

                case PROPERTY_TRACTIONLOSS:
                    pHandlingEntry->SetTractionLoss ( pOriginalEntry->GetTractionLoss () );
                    break;

                case PROPERTY_TRACTIONBIAS:
                    pHandlingEntry->SetTractionBias ( pOriginalEntry->GetTractionBias () );
                    break;

                case PROPERTY_SUSPENSION_FORCELEVEL:
                    pHandlingEntry->SetSuspensionForceLevel ( pOriginalEntry->GetSuspensionForceLevel () );
                    break;

                case PROPERTY_SUSPENSION_DAMPING:
                    pHandlingEntry->SetSuspensionDamping ( pOriginalEntry->GetSuspensionDamping () );
                    break;

                case PROPERTY_SUSPENSION_HIGHSPEEDDAMPING:
                    pHandlingEntry->SetSuspensionHighSpeedDamping ( pOriginalEntry->GetSuspensionHighSpeedDamping () );
                    break;

                case PROPERTY_SUSPENSION_UPPER_LIMIT:
                    pHandlingEntry->SetSuspensionUpperLimit ( pOriginalEntry->GetSuspensionUpperLimit () );
                    break;

                case PROPERTY_SUSPENSION_LOWER_LIMIT:
                    pHandlingEntry->SetSuspensionLowerLimit ( pOriginalEntry->GetSuspensionLowerLimit () );
                    break;

                case PROPERTY_SUSPENSION_FRONTREARBIAS:
                    pHandlingEntry->SetSuspensionFrontRearBias ( pOriginalEntry->GetSuspensionFrontRearBias () );
                    break;

                case PROPERTY_SUSPENSION_ANTIDIVEMULTIPLIER:
                    pHandlingEntry->SetSuspensionAntidiveMultiplier ( pOriginalEntry->GetSuspensionAntidiveMultiplier () );
                    break;

                case PROPERTY_COLLISIONDAMAGEMULTIPLIER:
                    pHandlingEntry->SetCollisionDamageMultiplier ( pOriginalEntry->GetCollisionDamageMultiplier () );
                    break;

                case PROPERTY_SEATOFFSETDISTANCE:
                    pHandlingEntry->SetSeatOffsetDistance ( pOriginalEntry->GetSeatOffsetDistance () );
                    break;

                case PROPERTY_HANDLINGFLAGS:
                    pHandlingEntry->SetHandlingFlags ( pOriginalEntry->GetHandlingFlags () );
                    break;

                case PROPERTY_MODELFLAGS:
                    pHandlingEntry->SetModelFlags ( pOriginalEntry->GetModelFlags () );
                    break;

                case PROPERTY_HEADLIGHT:
                    pHandlingEntry->SetHeadLight ( pOriginalEntry->GetHeadLight () );
                    break;

                case PROPERTY_TAILLIGHT:
                    pHandlingEntry->SetTailLight ( pOriginalEntry->GetTailLight () );
                    break;

                case PROPERTY_ANIMGROUP:
                    pHandlingEntry->SetAnimGroup ( pOriginalEntry->GetAnimGroup () );
                    break;
            }

            Vehicle.ApplyHandling();
        }
    }
}


void CHandlingRPCs::RestoreVehicleHandling ( NetBitStreamInterface& bitStream )
{
    // Read out the handling id
    ElementID ID;
    if ( bitStream.Read ( ID ) )
    {
        // Grab it and check its type
        CClientEntity* pEntity = CElementIDs::GetElement ( ID );
        if ( pEntity && pEntity->GetType () == CCLIENTVEHICLE )
        {
            // Grab the vehicle handling entry and restore all data
            CClientVehicle& Vehicle = static_cast < CClientVehicle& > ( *pEntity );
            Vehicle.GetHandlingData()->Restore();

            Vehicle.ApplyHandling();
        }
    }
}
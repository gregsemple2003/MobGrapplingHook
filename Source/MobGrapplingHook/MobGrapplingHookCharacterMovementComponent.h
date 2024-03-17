#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MobGrapplingHookCharacterMovementComponent.generated.h"

UCLASS(config = Game)
class UMobGrapplingHookCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

    void StartGrappling(FVector GrapplePoint);
    void StopGrappling();

protected:
    // The point the player is grappling to; is a static point to keep the server/client simulations in sync.
    // We may need to add this to FSavedMove_Character, but for our simple case its probably okay to let
    // the client/server diverge a little by having slight differences in this value depending on where the
    // grapple projectile hits the wall.
    FVector GrappleTargetPoint;

    static const uint8 CUSTOM_MovementMode_Grapple = MOVE_Custom;
};

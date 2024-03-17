#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MobGrapplingHookCharacterMovementComponent.generated.h"

UCLASS(config = Game)
class UMobGrapplingHookCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    /** The speed at which the grappling hook pulls the player once it lands */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grappling)
    float PullSpeed = 600.0f;

    /** How close the character is to the grappling hook before it stops pulling */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grappling)
    float StopPullingThreshold = 50.0f;

    virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

    void StartGrappling(FVector GrapplePoint);
    void StopGrappling();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastStartGrappling(FVector GrapplePoint);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastStopGrappling();

    UFUNCTION(Server, Reliable)
    void RequestStopGrappling();

protected:
    // The point the player is grappling to; the server chooses the point and replicates this to the client,
    // so the simulations are in sync enough that rubberbanding (ClientAdjustPosition) should not happen.
    // Also, we're assuming that the point never changes, but in practice if we allow the grappling hook
    // to attach to dynamic objects like physics sims or elevators, we might need to revisit, possibly
    // adding this to FSavedMove_Character.  Though in practice, seems unlikely.
    FVector GrappleTargetPoint;

    static const uint8 CUSTOM_MovementMode_Grapple = MOVE_Custom;
};

#include "MobGrapplingHookCharacterMovementComponent.h"

void UMobGrapplingHookCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
    if (MovementMode == CUSTOM_MovementMode_Grapple)
    {
        // Check if the actor is already within the minimum distance to the grapple target
        FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
        float DistanceToTarget = FVector::Dist(CurrentLocation, GrappleTargetPoint);
        if (DistanceToTarget <= StopPullingThreshold)
        {
            // Close enough, do nothing
            return;
        }

        // Use the same function to move as the CMC does for normal moves (PhysWalking, PhysFalling)
        // I believe this should work with other forms of additive velocity (e.g. from an explosion), but would need more testing
        FVector Direction = (GrappleTargetPoint - CurrentLocation).GetSafeNormal();
        FVector DesiredMovement = Direction * PullSpeed * DeltaTime;

        // Attempt the linear move, stop on hit
        FHitResult Hit(1.f);
        SafeMoveUpdatedComponent(DesiredMovement, Direction.ToOrientationQuat(), true, Hit);

        // If we hit something, slide along it (this may allow sliding up walls, etc.)
        if (Hit.IsValidBlockingHit())
        {
            SlideAlongSurface(DesiredMovement, (1.f - Hit.Time), Hit.Normal, Hit, true);
        }
    }
} 

void UMobGrapplingHookCharacterMovementComponent::StartGrappling(FVector GrapplePoint)
{
    GrappleTargetPoint = GrapplePoint;
    SetMovementMode(MOVE_Custom, CUSTOM_MovementMode_Grapple);

    if (GetOwnerRole() == ROLE_Authority)
    {
        MulticastStartGrappling(GrapplePoint);
    }
}

void UMobGrapplingHookCharacterMovementComponent::StopGrappling()
{
    // Check if we've already stopped, e.g. due to local prediction
    if (GrappleTargetPoint == FVector::ZeroVector)
    {
        return;
    }

    // Reset movement mode to walking, which should immediately put us into falling/walking as appropriate
    GrappleTargetPoint = FVector::ZeroVector;
    SetMovementMode(MOVE_Walking);

    if (GetOwnerRole() == ROLE_Authority)
    {
        MulticastStopGrappling();
    }
}

void UMobGrapplingHookCharacterMovementComponent::MulticastStartGrappling_Implementation(FVector GrapplePoint)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        StartGrappling(GrapplePoint);
    }
}

void UMobGrapplingHookCharacterMovementComponent::MulticastStopGrappling_Implementation()
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        StopGrappling();
    }
}

void UMobGrapplingHookCharacterMovementComponent::RequestStopGrappling_Implementation()
{
    StopGrappling();
}


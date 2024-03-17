#include "MobGrapplingHookCharacterMovementComponent.h"

void UMobGrapplingHookCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
    if (MovementMode == CUSTOM_MovementMode_Grapple)
    {
        // Use the same function to move as the CMC does for normal moves (PhysWalking, PhysFalling)
        // I believe this should work with other forms of additive velocity (e.g. from an explosion), but would need more testing
        FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
        FVector Direction = (GrappleTargetPoint - CurrentLocation).GetSafeNormal();
        float Speed = 600.0f; // Grapple speed, adjust as necessary, todo make a property in the class
        FVector DesiredMovement = Direction * Speed * DeltaTime;

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
}

void UMobGrapplingHookCharacterMovementComponent::StopGrappling()
{
    // Reset movement mode to walking, which should immediately put us into falling/walking as appropriate
    SetMovementMode(MOVE_Walking);
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobGrapplingHookProjectile.h"
#include "MobGrapplingHookCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AMobGrapplingHookProjectile::AMobGrapplingHookProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMobGrapplingHookProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AMobGrapplingHookProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bHit)
	{
		return;
	}

	bHit = true;

	// Deactivate the projectile's movement component so it stops moving
	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->SetActive(false);
		ProjectileMovement->SetComponentTickEnabled(false);
	}

	// Optionally, attach the projectile to the surface it hit so it stays in place even if the surface moves
	if (OtherComp && OtherComp->Mobility == EComponentMobility::Movable)
	{
		AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);
	}

	auto InstigatorCharacter = Cast<AMobGrapplingHookCharacter>(GetInstigator());
	auto InstigatorCharacterMovement = InstigatorCharacter ? InstigatorCharacter->GetCharacterMovement<UMobGrapplingHookCharacterMovementComponent>() : nullptr;
	InstigatorCharacterMovement->StartGrappling(GetActorLocation());
}
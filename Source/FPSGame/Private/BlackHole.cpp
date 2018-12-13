// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComp);


	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetupAttachment(MeshComp);

	SphereCenter = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCenterComponent"));
	SphereCenter->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OnOverlapCenterSphere);
	SphereCenter->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OutOverlappingObjects;
	SphereComp->GetOverlappingComponents(OutOverlappingObjects);

	ERadialImpulseFalloff Falloff = ERadialImpulseFalloff::RIF_Linear;

	for (auto Comp : OutOverlappingObjects)
	{
		Comp->AddRadialForce(GetActorLocation(), SphereComp->GetScaledSphereRadius() , -2000, Falloff, true);
	}
}

void ABlackHole::OnOverlapCenterSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		OtherActor->Destroy();
	}
}
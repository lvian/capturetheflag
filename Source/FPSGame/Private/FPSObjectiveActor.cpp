// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectiveActor.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
	MeshComp = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComp);

	SphereComp = CreateOptionalDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);
	
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	PlayEffects();
}

void AFPSObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickUpFx, GetActorLocation());
}

void AFPSObjectiveActor::PlayEffects(AActor* OtherActor)
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickUpFx, OtherActor->GetActorLocation());
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	PlayEffects(OtherActor);

	if (Role == ROLE_Authority)
	{
		AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			PlayerCharacter->bIsCarryingObjective = true;

			Destroy();
		}
	}
	
}
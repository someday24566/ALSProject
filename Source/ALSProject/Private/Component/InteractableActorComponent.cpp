// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InteractableActorComponent.h"

// Sets default values for this component's properties
UInteractableActorComponent::UInteractableActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractableActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractableActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

   /*当函数具有BlueprintNativeEvent时，实现时函数名必须添加后缀 _Implementation*/
   
void  UInteractableActorComponent::InteractionStarted_Implementation(APawn* Pawn)
{
	OnInteractionStarted.Broadcast(Pawn);
}


void  UInteractableActorComponent::InteractionUpdated_Implementation(APawn* Pawn)
{
	OnInteractionUpdated.Broadcast(Pawn);
}

void  UInteractableActorComponent::InteractionEnded_Implementation(APawn* Pawn)
{
	OnInteractionEnded.Broadcast(Pawn);
}
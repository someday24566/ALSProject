// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableActorComponent.generated.h"

//定义组播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionStarted, APawn*, Pawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionUpdated, APawn*, Pawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionEnded, APawn*, Pawn);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent,DisplayName="可交互Actor组件") )
class ALSPROJECT_API UInteractableActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableActorComponent();

	//定义组播委托变量
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionStarted OnInteractionStarted;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionUpdated OnInteractionUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionEnded OnInteractionEnded;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*
	 ******分发事件
	 BlueprintNativeEvent表示这个函数既可以在蓝图中实现，也可以在C++中实现。如果在蓝图中没有实现这个函数，那么就会调用C++中的默认实现。
	 当函数具有BlueprintNativeEvent时，实现时函数名必须添加后缀 _Implementation
	*/
	//事件:交互前
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void InteractionStarted(APawn* Pawn);

	//事件:交互中
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void InteractionUpdated(APawn* Pawn);

	//事件:交互结束
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void InteractionEnded(APawn* Pawn);
};

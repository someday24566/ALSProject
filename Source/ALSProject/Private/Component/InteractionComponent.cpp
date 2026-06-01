// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InteractionComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn)
	{
		PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	}

	StartInteractionTimer();

	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// 交互检测函数：定期检查玩家前方是否有可交互的Actor
void UInteractionComponent::InteractionCheck()
{ 
	// 如果没有拥有者Pawn，直接返回
	if (!OwnerPawn) return;

	// 获取世界对象，如果不存在则返回
	UWorld* World = GetWorld();
	if (!World) return;


	// 如果PlayerController未设置，尝试获取并设置
	if (!PlayerController)
	{
		AController* CurrentController = OwnerPawn->GetController();
		PlayerController = CurrentController ? Cast<APlayerController>(CurrentController) : nullptr;
	}


	// 获取Pawn的视角位置作为起始点
	FVector StartLocation = OwnerPawn->GetPawnViewLocation();
	FVector ForwardVector;

	// 如果有PlayerController，使用玩家的视角旋转向量
	if (PlayerController)
	{
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(StartLocation, CameraRotation);
		ForwardVector = CameraRotation.Vector();
	}
	else
	{
		// 否则，使用控制器的控制旋转向量
		AController* Controller = OwnerPawn->GetController();
		if (Controller)
		{
			ForwardVector = Controller->GetControlRotation().Vector();
		}
		else
		{
			// 如果没有控制器，使用Pawn的前向向量
			ForwardVector = OwnerPawn->GetActorForwardVector();
		}
	}

	// 计算结束位置：起始位置 + 前向向量 * 交互范围
	FVector EndLocation = StartLocation + ForwardVector * InteractionRange;

	// 设置碰撞查询参数，忽略自身
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(InteractionTrace), false, GetOwner());

	// 可视化调试：射线起点、终点与球形检测范围
	if(bDrawDebug)
	{
	DrawDebugLine(World, StartLocation, EndLocation, FColor::Blue, false, InteractionFrequency, 0, 2.0f);
	DrawDebugSphere(World, EndLocation, InteractionRadius, 16, FColor::Blue, false, InteractionFrequency, 0, 1.0f);
	}

	// 初始化命中结果数组
	TArray<FHitResult> HitResults;

	// 执行球形扫描，检测指定通道上的对象
	bool bHit = World->SweepMultiByChannel(
		HitResults,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		InteractionTraceChannel,
		FCollisionShape::MakeSphere(InteractionRadius),
		QueryParams
	);

	// 初始化最佳命中Actor和最小距离平方
	AActor* BestHitActor = nullptr;
	float BestDistanceSq = FLT_MAX;

	// 如果有命中结果
	if (bHit)
	{
		// 遍历所有命中结果
		for (const FHitResult& HitResult : HitResults)
		{
			// 获取命中的Actor
			AActor* HitActor = HitResult.GetActor();

			if (!HitActor) continue;  // 如果Actor不存在，跳过
			// 检查Actor是否有UInteractableActorComponent组件
			if (!HitActor->GetComponentByClass<UInteractableActorComponent>()) continue;

			// 计算从Pawn到Actor的方向向量
			FVector PawnLocation = OwnerPawn->GetActorLocation();
			FVector ActorLocation = HitActor->GetActorLocation();
			FVector DirectionToActor = (ActorLocation - PawnLocation).GetSafeNormal();

			// 检查Actor是否在Pawn前方（夹角小于90度）
			float DotProduct = FVector::DotProduct(ForwardVector, DirectionToActor);
			
			if (DotProduct <= 0.0f) continue; // 如果不在前方，跳过
            
			// 计算命中点到起始位置的距离平方
			const float DistanceSq = FVector::DistSquared(HitResult.Location, StartLocation);

			// 如果距离更近，更新最佳Actor
			if (DistanceSq < BestDistanceSq)
			{
				BestDistanceSq = DistanceSq;
				BestHitActor = HitActor;
			}
		}
	}

	// 如果找到了最佳命中Actor
	if (BestHitActor)
	{
	   // 如果当前没有交互Actor或与最佳Actor不同
	   if (!InteractedActor || InteractedActor != BestHitActor)
	  {
		// 设置当前交互Actor
		InteractedActor = BestHitActor;
		// 触发交互检查事件
		OnInteractionChecked(BestHitActor);
		// 屏幕调试信息
		
		if (GEngine)
		{
			if(bDrawDebug)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Hit Actor: %s"), *BestHitActor->GetName()));
			}
		}
	  }
	}
	else if (InteractedActor)
	{
		// 如果没有命中但之前有交互Actor，结束交互检查
		OnInteractionCheckEnded(InteractedActor);
		InteractedActor = nullptr;
	}
}


// 开始交互计时器：启动定期调用InteractionCheck的计时器
void UInteractionComponent::StartInteractionTimer()
{
	// 获取世界对象
	UWorld* World = GetWorld();
	if (!World) return;

	// 设置计时器，每InteractionFrequency秒调用一次InteractionCheck，循环执行
	World->GetTimerManager().SetTimer(InteractionTimerHandle, this, &UInteractionComponent::InteractionCheck, InteractionFrequency, true);
}

// 停止交互计时器：清除交互检测计时器
void UInteractionComponent::StopInteractionTimer()
{
	// 获取世界对象
	UWorld* World = GetWorld();
	if (!World) return;

	// 清除计时器
	World->GetTimerManager().ClearTimer(InteractionTimerHandle);
}

	//输入事件:开始交互

	void UInteractionComponent::Server_StartInteract_Implementation()
{ 
    if(InteractedActor)
	{
		OnInteractionStarted(InteractedActor);
		InteractedActor->GetComponentByClass<UInteractableActorComponent>()->InteractionStarted(OwnerPawn);
	}
}

	//输入事件:交互中

    void UInteractionComponent::Server_Interacting_Implementation()
{
     if(InteractedActor)
	{
		OnInteractionUpdated(InteractedActor);
		InteractedActor->GetComponentByClass<UInteractableActorComponent>()->InteractionUpdated(OwnerPawn);
	}
}
	//输入事件:结束交互

	void UInteractionComponent::Server_EndInteract_Implementation	()
{	
	if(InteractedActor)
	{
		OnInteractionEnded(InteractedActor);
		InteractedActor->GetComponentByClass<UInteractableActorComponent>()->InteractionEnded(OwnerPawn);
		InteractedActor = nullptr;
	}
}
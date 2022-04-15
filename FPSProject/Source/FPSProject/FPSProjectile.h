// Fill out your copyright notice in the Description page of Project Settings.
//子弹
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"	//球体组件
#include "GameFramework/ProjectileMovementComponent.h"
#include "FPSProjectile.generated.h"

UCLASS()
class FPSPROJECT_API AFPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//球体碰撞组件
	//VisibleDefaultsOnly：此属性只在原型的属性窗口可见，无法被编辑
	UPROPERTY(VisibleDefaultsOnly,Category=Projectile)
		USphereComponent* CollisionComponent;
	//发射物移动组件（我的理解是：控制发射出去的子弹位置情况）
	//该组件中内置了许多运行相关的参数，例如最大速度、初始速度等
	UPROPERTY(VisibleAnywhere,Category=Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;
	//初始化射击方向上发射物速度（负责发射子弹）
	void FireInDirection(const FVector& ShootDirection);
	//发射物网格体
	UPROPERTY(VisibleDefaultsOnly,Category=Projectile)
		UStaticMeshComponent* ProjectileMeshComponent;
	//发射物材质
	UPROPERTY(VisibleDefaultsOnly,Category=Movement)
		UMaterialInstanceDynamic* ProjectileMaterialInstance;
};

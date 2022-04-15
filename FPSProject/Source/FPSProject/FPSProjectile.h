// Fill out your copyright notice in the Description page of Project Settings.
//�ӵ�
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"	//�������
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

	//������ײ���
	//VisibleDefaultsOnly��������ֻ��ԭ�͵����Դ��ڿɼ����޷����༭
	UPROPERTY(VisibleDefaultsOnly,Category=Projectile)
		USphereComponent* CollisionComponent;
	//�������ƶ�������ҵ�����ǣ����Ʒ����ȥ���ӵ�λ�������
	//����������������������صĲ�������������ٶȡ���ʼ�ٶȵ�
	UPROPERTY(VisibleAnywhere,Category=Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;
	//��ʼ����������Ϸ������ٶȣ��������ӵ���
	void FireInDirection(const FVector& ShootDirection);
	//������������
	UPROPERTY(VisibleDefaultsOnly,Category=Projectile)
		UStaticMeshComponent* ProjectileMeshComponent;
	//���������
	UPROPERTY(VisibleDefaultsOnly,Category=Movement)
		UMaterialInstanceDynamic* ProjectileMaterialInstance;
};

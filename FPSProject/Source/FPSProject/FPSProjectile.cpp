// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//���ø�������������������ڵĻ���
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileScenceComponent"));
	}
	//��ʼ��������ײ���
	if (!CollisionComponent)
	{
		//�����������ײʵ��
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		//��������뾶
		CollisionComponent->SetSphereRadius(15.0f);
		//���������Ϊ��ײ���
		RootComponent = CollisionComponent;
	}

	//��ʼ���������ƶ����
	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		//��Ҫ���Ƶ��������ײ����������Ŀ���λ����Ϊ�ɴ������֧��
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		//���ó�ʼ�ٶ�
		ProjectileMovementComponent->InitialSpeed=3000.0f;
		//��������ٶ�
		ProjectileMovementComponent->MaxSpeed=3000.0f;
		//�ӵ�����ת����ÿһ֡�и��£���ƥ�������ٶȷ���
		ProjectileMovementComponent->bRotationFollowsVelocity=true;
		//���÷���
		ProjectileMovementComponent->bShouldBounce=true;
		//���÷���ϵ��������������ǿ��
		ProjectileMovementComponent->Bounciness=0.3f;
		//��������ϵ��
		ProjectileMovementComponent->ProjectileGravityScale=0.0f;
		//������������
		InitialLifeSpan=3.0f;
	}

	//���ò���
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Material/SphereMaterial.SphereMaterial'"));
	if (Material.Succeeded())
	{
		//�ⲿ�ֻ�û������Ϊɶ����Ӿ�̬������
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
		
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));	//�������������丸����ı���
	ProjectileMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	//Ϊ�ӵ���ʼ����̬�����壬����ʾ��� 
	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Sphere.Sphere"));
		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//��ʼ���ٶ�
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	//�ٶȷ���*�ٶȴ�С=�ٶ���������С��ProjectileMovementComponent��������
	ProjectileMovementComponent->Velocity=ShootDirection*ProjectileMovementComponent->InitialSpeed;
}

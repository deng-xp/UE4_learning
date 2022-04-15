// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�����ʼ��
	FPSCamerComponent=CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCamerComponent!=nullptr);
	//���ӵ���������������������(SetupAttachment����ӦΪUSceneComponent���ͣ��˴����ж����ͣ�����������ת���ĺ��壩
	FPSCamerComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	//�������λ��
	FPSCamerComponent->SetRelativeLocation(FVector(0.0f,0.0f,50.0f+BaseEyeHeight));		//�����Ը����۾���λ��
	FPSCamerComponent->SetRelativeRotation(FRotator(0.0f,0.0f,0.0f));	//�������
	//����Pawn�����������ת
	FPSCamerComponent->bUsePawnControlRotation=true;

	//Ϊ������Ҵ�����һ�˳����������
	FPSMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh!=nullptr);
	//��������ҿɿ����������壨��ʾ����ӵ�д˽�ɫ��PlayerController�ɼ���
	FPSMesh->SetOnlyOwnerSee(true);
	//��FPS�����帽�ӵ�FPS�����
	FPSMesh->SetupAttachment(FPSCamerComponent);
	//����ĳЩ�����µ���Ӱ��ʵ��ֻ�е���������ĸо�
	FPSMesh->bCastDynamicShadow=false;
	FPSMesh->CastShadow = false;

	//������ҿ��������棨�����˳ƣ�ȫ��������
	GetMesh()->SetOwnerNoSee(true); 
	
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter"));
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input ��������������󶨣�
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//���ƶ�����
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	//��������ӽǱ䶯
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	//����Ծ
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
	//���ӵ�����
	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&AFPSCharacter::Fire);
}

//ǰ��
void AFPSCharacter::MoveForward(float value)
{
	// �ҳ�"ǰ��"���򣬲���¼�������÷����ƶ���
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value);

}
//����
void AFPSCharacter::MoveRight(float value)
{
	// �ҳ�"�Ҳ�"���򣬲���¼�������÷����ƶ���
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
}
//��Ծ
void AFPSCharacter::StartJump()
{
	bPressedJump = true;	//Acharacter�����еı�����֧����Ծ
}
void AFPSCharacter::StopJump()
{
	bPressedJump = false;	
}
//�ӵ�����
//�ӵ�����Ĺؼ����ڣ���1���ӵ��Ӻδ���ʼ���䣻��2���ӵ��������ɶ������ý�ɫ�˽��ӵ���
void AFPSCharacter::Fire()
{
	
	//���Է����ӵ�
	if (ProjectileClass)	//�ӵ������Ѱ�
	{
		//��ȡ������任������ȷ��ǹ��λ�ã�
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation,CameraRotation);	//�����ӽǵı任
		//����MuzzleOffset����ǹ������������λ�õ�ƫ����
		MuzzleOffset.Set(100.0f,0.0f,0.0f);
		//��MuzzleOffset�������������ռ�ת������������ϵ��
		//������������������ǵģ���ʱ�������᷽������������ϵ�в��죬��������ת��
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		//ʹĿ�귽����������б�������Ϊ��������
		FRotator MuzzleRotation =CameraRotation;
		MuzzleRotation.Pitch+=10.0f;

		UWorld* World=GetWorld();	//��������ָ��
		if (World)
		{
			//���ݸ���������Ŀ�ѡ��������һ���ṹ�壬����һЩActor���ɵĲ������ã�
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner=this;		//�������ɵ����ӵ����Ϊ���࣬��FPSCharacter��
			SpawnParams.Instigator=GetInstigator();		//���������ɵ�Actorʩ���˺��Ķ���

			//��ǹ��λ�����ɷ�������ӵ�
			/*SpawnActor������
			�ú���ģ����ָ����λ�á���ʹ��ָ���ġ���ת�ȡ�����ָ���ࡰProjectileClass����ʵ����
			�����غ��Ǹ�ģ��������һ����ʵ����ָ�룬Ҳ���� T*`����Ҫ��ָ����ProjectileClass���
			����ģ����T�����ࡣ�����ࡢλ�ü���ת���⣬������ָ��ӵ����Actor �����Ƶĵ�Pawn �� 
			��ָ��������ɵ� Actor ����ռ����ײ��һ���������Ѿ����ڵ� Actor ���ɲ���
			*/
			AFPSProjectile* Projectile=World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				//���÷�����ĳ�ʼ�켣
				//�����ӵ�����ת�任��ȡ�䡰��ǰ�ķ���
				FVector LaunchDirection=MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}
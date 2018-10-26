// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBaseMeshComponent.h"




UWeaponBaseMeshComponent::UWeaponBaseMeshComponent()
{
	//GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	//MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	bReceivesDecals = true;
	CastShadow = true;
	SetCollisionObjectType(ECC_WorldDynamic);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	

	BulletSum = BulletCount = 20;

	SetIsReplicated(true);
}

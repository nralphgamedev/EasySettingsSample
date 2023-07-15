// Copyright Nathan Ralph. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SettingTypes/SettingBase.h"
#include "EasySettingsSubsystem.generated.h"

struct FOpposingTogglesSetting;
struct FFloatSetting;

/**
 * Subsystem for storing delegates and TMaps related to our various setting types.
 */
UCLASS(DisplayName = "EasySettingsSubsystem")
class EASYSETTINGS_API UEasySettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFloatSettingChangedSignature, FSettingBase, SettingID, float, NewCurrent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOpposingTogglesSettingChangedSignature, FSettingBase, SettingID, bool, bNewFirstIsActive);

	/** Signals listeners whenever a float setting is updated. */
	UPROPERTY(BlueprintAssignable, Category = "Easy Settings")
	FFloatSettingChangedSignature FloatSettingChangedDelegate;

	/** Signals listeners whenever an opposing toggles setting is updated. */
	UPROPERTY(BlueprintAssignable, Category = "Easy Settings")
	FOpposingTogglesSettingChangedSignature OpposingTogglesSettingChangedDelegate;

	/** Float settings that have been registered through the SetupFloatSetting call. */
	TMap<FString, FFloatSetting*> FloatSettings;

	/** Opposing Toggles settings that have been registered through the SetupOpposingTogglesSetting call. */
	TMap<FString, FOpposingTogglesSetting*> OpposingTogglesSettings;
};

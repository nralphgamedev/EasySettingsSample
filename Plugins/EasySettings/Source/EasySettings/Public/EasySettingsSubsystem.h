// Copyright SixlineGames LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SettingTypes/SettingBase.h"
#include "EasySettingsSubsystem.generated.h"

struct FOpposingTogglesSetting;
struct FFloatSetting;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFloatSettingChanged, FSettingBase, SettingID, float, NewCurrent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOpposingTogglesSettingChanged, FSettingBase, SettingID, bool, bNewFirstIsActive);

/**
 * Subsystem for storing delegates and TMaps related to our various setting types.
 */
UCLASS(DisplayName = "EasySettingsSubsystem")
class EASYSETTINGS_API UEasySettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/** Signals listeners whenever a float setting is changed. */
	UPROPERTY(BlueprintAssignable, Category = "Easy Settings")
	FFloatSettingChanged FloatSettingChanged_Delegate;

	/** Signals listeners whenever an opposing toggles setting is changed. */
	UPROPERTY(BlueprintAssignable, Category = "Easy Settings")
	FOpposingTogglesSettingChanged OpposingTogglesSettingChanged_Delegate;

	/** Float settings that have been registered through the SetupFloatSetting call. */
	TMap<FString, FFloatSetting*> FloatSettings;

	/** Opposing Toggles settings that have been registered through the SetupOpposingTogglesSetting call. */
	TMap<FString, FOpposingTogglesSetting*> OpposingTogglesSettings;

	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem
};

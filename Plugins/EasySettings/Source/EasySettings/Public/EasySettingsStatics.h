// Copyright Nathan Ralph. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SettingTypes/OpposingTogglesSetting.h"
#include "SettingTypes/FloatSetting.h"
#include "EasySettingsSubsystem.h"
#include "EasySettingsStatics.generated.h"

/**
 * Static function library for ease of use between blueprint and C++.
 */
UCLASS()
class EASYSETTINGS_API UEasySettingsStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	/** Helper function for getting the Easy Settings Subsystem. */
	static UEasySettingsSubsystem* GetESSubsystem(const UObject* WorldContextObject);

public:
	/**
	 * Stores a pointer to the setting in a TMap. Updates the setting with what is already present in the ini file or writes the defaults out if it isn't already there.
	 * Will overwrite existing entries and print a warning to the output if called multiple times on the same entry.
	 * @see LogEasySettings category in the output log.
	 * @param WorldContextObject	World object used for getting the EasySettingsSubsystem.
	 * @param Setting				The setting we want to setup.
	 * @param bOutSuccess			True if the setting was set up within Min and Max values. False if the Default value could not be used or another setting was already setup using the provided Setting parameter.
	 */
	UFUNCTION(BlueprintCallable, Category = "Easy Settings Statics", meta = (WorldContext = "WorldContextObject"))
	static void SetupFloatSetting(const UObject* WorldContextObject, UPARAM(ref) FFloatSetting& Setting, bool& bOutSuccess);

	/**
	 * Finds the reference that was stored through the Setup function.
	 * @param WorldContextObject	World object used for getting the EasySettingsSubsystem.
	 * @param SettingID				The setting we are trying to get.
	 * @param bOutSuccess			True if the setting was found in our FloatSettings.
	 * @param OutSetting			A reference to the registered setting.
	 */
	UFUNCTION(BlueprintPure, Category = "Easy Settings Statics", meta = (WorldContext = "WorldContextObject"))
	static void GetFloatSetting(const UObject* WorldContextObject, const FSettingBase& SettingID, bool& bOutSuccess, FFloatSetting& OutSetting);

	/**
	 * Attempts to update the setting reference and store the value in an ini file. Ensures the NewCurrent is within min and max values. Triggers the FloatSettingChanged_Delegate if successful.
	 * @param WorldContextObject	World object used for getting the EasySettingsSubsystem.
	 * @param SettingID				The setting we are trying to update.
	 * @param NewCurrent			The new current value we are trying to update to.
	 * @param bOutSuccess			True if the update was successful. False if nothing changed or the setting was never setup.
	 */
	UFUNCTION(BlueprintCallable, Category = "Easy Settings Statics", meta = (WorldContext = "WorldContextObject"))
	static void UpdateFloatSetting(const UObject* WorldContextObject, const FSettingBase& SettingID, float NewCurrent, bool& bOutSuccess);

	/**
	 * Stores a pointer to the setting in a TMap. Updates the setting with what is already present in the ini file or writes the defaults out if it isn't already there.
	 * Will overwrite existing entries and print a warning to the output if called multiple times on the same entry.
	 * @see LogEasySettings category in the output log.
	 * @param WorldContextObject	World object used for getting the EasySettingsSubsystem.
	 * @param Setting				The setting we want to setup.
	 * @param bOutSuccess			False if another setting was already setup using the provided Setting parameter.
	 */
	UFUNCTION(BlueprintCallable, Category = "Easy Settings Statics", meta = (WorldContext = "WorldContextObject"))
	static void SetupOpposingTogglesSetting(const UObject* WorldContextObject, UPARAM(ref) FOpposingTogglesSetting& Setting, bool& bOutSuccess);

	/**
	 * Finds the reference that was stored through the Setup function.
	 * @param WorldContextObject	World object used for getting the EasySettingsSubsystem.
	 * @param SettingID				The setting we are trying to get.
	 * @param bOutSuccess			True if the setting was found in our OpposingTogglesSettings.
	 * @param OutSetting			A reference to the registered setting.					
	 */
	UFUNCTION(BlueprintPure, Category = "Easy Settings Statics", meta = (WorldContext = "WorldContextObject"))
	static void GetOpposingTogglesSetting(const UObject* WorldContextObject, const FSettingBase& SettingID, bool& bOutSuccess, FOpposingTogglesSetting& OutSetting);

	/**
	 * Attempts to update the setting reference and store the value in an ini file. Triggers the OpposingTogglesSettingChanged_Delegate if successful.
	 * @param WorldContextObject	World object used for getting the EasySettingsSubsystem.
	 * @param SettingID				The setting we are trying to update.
	 * @param bActivateFirst		What we will set as the updated active toggle.
	 * @param bOutSuccess			True if the update was successful. False if nothing changed or the setting was never setup.
	 */
	UFUNCTION(BlueprintCallable, Category = "Easy Settings Statics", meta = (WorldContext = "WorldContextObject"))
	static void UpdateOpposingTogglesSetting(const UObject* WorldContextObject, const FSettingBase& SettingID, bool bActivateFirst, bool& bOutSuccess);
};

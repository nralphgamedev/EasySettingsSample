// Copyright Nathan Ralph. All Rights Reserved.

#include "EasySettingsStatics.h"

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogEasySettings, Log, All);

UEasySettingsSubsystem* UEasySettingsStatics::GetESSubsystem(const UObject* WorldContextObject)
{
	UGameInstance* GI = UGameplayStatics::GetGameInstance(WorldContextObject);
	return GI->GetSubsystem<UEasySettingsSubsystem>();
}

void UEasySettingsStatics::SetupFloatSetting(const UObject* WorldContextObject, FFloatSetting& Setting, bool& bOutSuccess)
{
	bOutSuccess = false;

	FString IniFileName = Setting.GetIniFileName();
	FString TMapKey = Setting.GetTMapKey();
	UEasySettingsSubsystem* SubSys = GetESSubsystem(WorldContextObject);

	// Check if the setting is in the config file yet.
	float FloatReceived;
	if (GConfig->GetFloat(*Setting.Section, *Setting.Key, FloatReceived, IniFileName) && Setting.ValidateFloatSetting(FloatReceived))
	{
		// Successfully found a usable float in the ini.
		Setting.Current = FloatReceived;
		bOutSuccess = true;
	}
	else
	{
		float DefaultFloat = 0.f;
		if (Setting.ValidateFloatSetting(Setting.Default))
		{
			DefaultFloat = Setting.Default;
			bOutSuccess = true;
		}
		else
		{
			// Print error to log if the struct was set up incorrectly.
			UE_LOG(LogEasySettings, Error, TEXT("Setting.Default was not within Min and Max values. Setting the Default for this to 0. Setting TMap key was %s"), *TMapKey);
		}

		// We didn't find any usable value in the config file, so set it in ini as the default value.
		GConfig->SetFloat(*Setting.Section, *Setting.Key, DefaultFloat, IniFileName);
		GConfig->Flush(false, IniFileName);
		Setting.Current = DefaultFloat;
	}

	if (SubSys->FloatSettings.Contains(TMapKey))
	{
		// If adding something that is already in our TMap, it will be replaced. Print a warning but continue to replace the element.
		UE_LOG(LogEasySettings, Warning, TEXT("This FloatSetting was already setup. Replacing old setting with new. Setting TMap key was %s"), *TMapKey);
		bOutSuccess = false;
	}

	SubSys->FloatSettings.Add(TMapKey, &Setting);
}

void UEasySettingsStatics::GetFloatSetting(const UObject* WorldContextObject, const FSettingBase& SettingID, bool& bOutSuccess, FFloatSetting& OutSetting)
{
	bOutSuccess = false;

	FString TMapKey = SettingID.GetTMapKey();
	UEasySettingsSubsystem* SubSys = GetESSubsystem(WorldContextObject);

	if (SubSys->FloatSettings.Contains(TMapKey))
	{
		// We don't need to handle the possibility of someone changing this setting manually in the ini, as we are just referencing what is already in memory.
		// The Setup function ensures what is in the ini falls within bounds prior to storing it in memory.
		OutSetting = **SubSys->FloatSettings.Find(TMapKey);
		bOutSuccess = true;
	}
	else
	{
		// It was never registered through the Setup function, don't signal a success.
		//UE_LOG(LogEasySettings, Warning, TEXT("GetFloatSetting() called on a FloatSetting that was never registered with SetupFloatSetting(). Setting TMap key was %s"), *TMapKey);
	}
}

void UEasySettingsStatics::UpdateFloatSetting(const UObject* WorldContextObject, const FSettingBase& SettingID, float NewCurrent, bool& bOutSuccess)
{
	bOutSuccess = false;

	FString IniFileName = SettingID.GetIniFileName();
	FString TMapKey = SettingID.GetTMapKey();
	UEasySettingsSubsystem* SubSys = GetESSubsystem(WorldContextObject);

	if (SubSys->FloatSettings.Contains(TMapKey))
	{
		FFloatSetting* FoundFloatSetting = *SubSys->FloatSettings.Find(TMapKey);
		if (FoundFloatSetting->ValidateFloatSetting(NewCurrent))
		{
			// Make sure the new setting is actually different than the old.
			if (FoundFloatSetting->Current == NewCurrent)
			{
				return;
			}

			// Update the ini file.
			GConfig->SetFloat(*FoundFloatSetting->Section, *FoundFloatSetting->Key, NewCurrent, IniFileName);
			GConfig->Flush(false, IniFileName);

			// Update the TMap then let listeners know of the change.
			FoundFloatSetting->Current = NewCurrent;
			SubSys->FloatSettingChangedDelegate.Broadcast(SettingID, NewCurrent);

			// Successfully updated.
			bOutSuccess = true;
		}
		else
		{
			UE_LOG(LogEasySettings, Warning, TEXT("UpdateFloatSetting() called on a FloatSetting that was outside the Min and Max values. Nothing was changed. Setting TMap key was %s"), *TMapKey);
		}
	}
	else
	{
		// Either it was never setup, or it was out of bounds. We changed nothing.
		UE_LOG(LogEasySettings, Warning, TEXT("UpdateFloatSetting() called on a FloatSetting that was never setup with SetupFloatSetting(). Setting TMap key was %s"), *TMapKey);
	}
}

void UEasySettingsStatics::SetupOpposingTogglesSetting(const UObject* WorldContextObject, FOpposingTogglesSetting& Setting, bool& bOutSuccess)
{
	bOutSuccess = false;

	FString IniFileName = Setting.GetIniFileName();
	FString TMapKey = Setting.GetTMapKey();
	UEasySettingsSubsystem* SubSys = GetESSubsystem(WorldContextObject);

	// Check if the setting is in the config file yet.
	FString StringReceived;
	if (GConfig->GetString(*Setting.Section, *Setting.Key, StringReceived, IniFileName) && Setting.ParseOpposingToggleSetting(StringReceived))
	{
		switch (Setting.ParseOpposingToggleSetting(StringReceived))
		{
		case 1:
			Setting.bFirstIsActive = true;
			break;
		case 2:
			Setting.bFirstIsActive = false;
			break;
		}
	}
	else
	{
		// The string we found in the ini file didn't match our FirstToggle or SecondToggle or wasn't there.
		// Overwrite what is there, then print a warning.
		if (Setting.bFirstIsDefault)
		{
			GConfig->SetString(*Setting.Section, *Setting.Key, *Setting.FirstToggle.ToLower(), IniFileName);
			Setting.bFirstIsActive = true;
		}
		else
		{
			GConfig->SetString(*Setting.Section, *Setting.Key, *Setting.SecondToggle.ToLower(), IniFileName);
			Setting.bFirstIsActive = false;
		}
		GConfig->Flush(false, IniFileName);
		UE_LOG(LogEasySettings, Warning, TEXT("Stored OpposingTogglesSettings did not match either the first or second toggles or was not present in the ini file. Overwriting with default. Setting TMap key was %s"), *TMapKey);
	}
	// Either way, we should have a valid default value now.
	bOutSuccess = true;

	if (SubSys->OpposingTogglesSettings.Contains(TMapKey))
	{
		// If adding something that is already in our TMap, it will be replaced. Print a warning but continue to replace the element.
		UE_LOG(LogEasySettings, Warning, TEXT("This OpposingTogglesSettings was already setup. Replacing old setting with new. Setting TMap key was %s"), *TMapKey);
		bOutSuccess = false;
	}

	SubSys->OpposingTogglesSettings.Add(TMapKey, &Setting);
}

void UEasySettingsStatics::GetOpposingTogglesSetting(const UObject* WorldContextObject, const FSettingBase& SettingID, bool& bOutSuccess, FOpposingTogglesSetting& OutSetting)
{
	bOutSuccess = false;

	FString TMapKey = SettingID.GetTMapKey();
	UEasySettingsSubsystem* SubSys = GetESSubsystem(WorldContextObject);

	if (SubSys->OpposingTogglesSettings.Contains(TMapKey))
	{
		// We don't need to handle the possibility of someone changing this setting manually in the ini file, as we are just referencing what is already in memory.
		// The Setup function ensures what is in the ini file is valid prior to storing it in memory.
		OutSetting = **SubSys->OpposingTogglesSettings.Find(TMapKey);
		bOutSuccess = true;
	}
	else
	{
		// It was never registered through the Setup function, don't signal a success.
		//UE_LOG(LogEasySettings, Warning, TEXT("GetOpposingTogglesSetting() called on an OpposingTogglesSetting that was never registered with SetupOpposingTogglesSetting(). Setting TMap key was %s"), *TMapKey);
	}
}

void UEasySettingsStatics::UpdateOpposingTogglesSetting(const UObject* WorldContextObject, const FSettingBase& SettingID, bool bActivateFirst, bool& bOutSuccess)
{
	bOutSuccess = false;

	FString IniFileName = SettingID.GetIniFileName();
	FString TMapKey = SettingID.GetTMapKey();
	UEasySettingsSubsystem* SubSys = GetESSubsystem(WorldContextObject);

	if (SubSys->OpposingTogglesSettings.Contains(TMapKey))
	{
		FOpposingTogglesSetting* FoundOpposingToggle = *SubSys->OpposingTogglesSettings.Find(TMapKey);
		// Make sure the new setting is actually different than the old.
		if (FoundOpposingToggle->bFirstIsActive != bActivateFirst)
		{
			// Update the ini file.
			FString NewActive;
			if (bActivateFirst)
			{
				NewActive = FoundOpposingToggle->FirstToggle;
			}
			else
			{
				NewActive = FoundOpposingToggle->SecondToggle;
			}

			GConfig->SetString(*FoundOpposingToggle->Section, *FoundOpposingToggle->Key, *NewActive.ToLower(), IniFileName);
			GConfig->Flush(false, IniFileName);

			// Update the TMap then let listeners know of the change.
			FoundOpposingToggle->bFirstIsActive = bActivateFirst;
			SubSys->OpposingTogglesSettingChangedDelegate.Broadcast(SettingID, bActivateFirst);

			bOutSuccess = true;
		}
	}
	else
	{
		// This setting was never setup, don't signal a success.
		UE_LOG(LogEasySettings, Warning, TEXT("UpdateOpposingTogglesSetting() called on a OpposingTogglesSetting that was never setup with SetupOpposingTogglesSetting(). Setting TMap key was %s"), *TMapKey);
	}
}

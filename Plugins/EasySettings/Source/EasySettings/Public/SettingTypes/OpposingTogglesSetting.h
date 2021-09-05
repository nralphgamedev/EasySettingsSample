// Copyright SixlineGames LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingBase.h"
#include "OpposingTogglesSetting.generated.h"

 /**
  * Setting used to ensure only one of the provided toggles can be active at a time.
  */
USTRUCT(BlueprintType, Category = "Easy Settings")
struct FOpposingTogglesSetting : public FSettingBase
{
	GENERATED_BODY()
public:
	/** The name of the first toggle. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString FirstToggle = "";

	/** The name of the second toggle. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString SecondToggle = "";

	/**
	 * True if the setting is currently set to the first toggle.
	 * This setting is initialized from the ini file, or defaults to bFirstIsDefault if not present or valid.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bFirstIsActive = false;

	/** True if the first toggle should be active by default. Should only be set from the object that instantiates this setting. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bFirstIsDefault = false;

	/** Disabled default constructor. */
	FOpposingTogglesSetting() {}

	FOpposingTogglesSetting(EIniFile ConfigFile, FString ConfigSection, FString ValueKey, FString FirstToggleName, FString SecondToggleName, bool bDefaultAndActiveValue = true)
	{
		IniFile = ConfigFile;
		Section = ConfigSection;
		Key = ValueKey;

		if (FirstToggleName != SecondToggleName)
		{
			FirstToggle = FirstToggleName;
			SecondToggle = SecondToggleName;
		}
		else
		{
			SecondToggle = FString("SecondToggleCannotMatchFirstToggle");
		}

		if (FirstToggleName.IsEmpty())
		{
			FirstToggle = FString("FirstToggleCannotBeEmpty");
		}
		else if (SecondToggleName.IsEmpty())
		{
			SecondToggle = FString("SecondToggleCannotBeEmpty");
		}

		bFirstIsActive = bDefaultAndActiveValue;
		bFirstIsDefault = bDefaultAndActiveValue;
	}

	/**
	 * Ensures that the provided string matches either the first or second toggle.
	 * @param CheckThis		The string we'll be checking against the first and second toggles.
	 * @return				0 if nothing matched.
	 * @return				1 if the FirstToggle matches the CheckThis value.
	 * @return				2 if the SecondToggle matches the CheckThis value.
	 */
	int ParseOpposingToggleSetting(FString CheckThis)
	{
		if (FirstToggle.ToLower() == CheckThis.ToLower())
		{
			return 1;
		}
		else if (SecondToggle.ToLower() == CheckThis.ToLower())
		{
			return 2;
		}

		// Nothing matched, return 0.
		return 0;
	}
};
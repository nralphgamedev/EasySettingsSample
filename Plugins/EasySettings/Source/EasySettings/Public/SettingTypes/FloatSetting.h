// Copyright SixlineGames LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingBase.h"
#include "FloatSetting.generated.h"

 /**
  * Setting used to ensure values stay within a specific range.
  */
USTRUCT(BlueprintType, Category = "Settings")
struct FFloatSetting : public FSettingBase
{
	GENERATED_BODY()

	/** The minimum value this setting can be set to. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Min = 0.f;

	/** Stored and retrieved from the ini file. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Current = 50.f;

	/**
	 * The Current value will default to this if outside the Min or Max values.
	 * The ini file will be populated with this, should it be missing.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Default = 50.f;

	/** The maximum value this setting can be set to. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Max = 100.f;

	/** Disabled default constructor. */
	FFloatSetting() {}

	FFloatSetting(EIniFile ConfigFile, FString ConfigSection, FString ValueKey, float MinValue, float DefaultAndCurrentValue, float MaxValue)
	{
		IniFile = ConfigFile;
		Section = ConfigSection;
		Key = ValueKey;

		Min = MinValue;
		Current = DefaultAndCurrentValue;
		Default = DefaultAndCurrentValue;
		Max = MaxValue;
	}

	/** Returns true if the CheckThis float is within min and max values. */
	bool ValidateFloatSetting(float CheckThis)
	{
		return CheckThis >= this->Min && CheckThis <= this->Max;
	}
};
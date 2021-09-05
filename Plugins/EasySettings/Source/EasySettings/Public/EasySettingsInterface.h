// Copyright SixlineGames LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EasySettingsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEasySettingsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Expected functionality that every widget should implement.
 */
class EASYSETTINGS_API IEasySettingsInterface
{
	GENERATED_BODY()
public:
	/** Signals the widget to reset to default values. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Easy Settings")
	void ResetToDefault();

	/** Tells the widget to revert to the value that was last applied by the user. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Easy Settings")
	void ResetToLastApplied();

	/** Update the last applied value and setting reference. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Easy Settings")
	void SettingApplied();

	/** Returns true if the setting is currently set to the default value. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Easy Settings")
	bool IsSettingDefault();

	/** Returns true if the setting is different than the last applied value. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Easy Settings")
	bool DidSettingChange();
};

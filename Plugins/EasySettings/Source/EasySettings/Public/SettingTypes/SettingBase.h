// Copyright SixlineGames LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingBase.generated.h"

/**
 * Used to determine which ini file we want to store/retrieve settings from.
 * All ini files can be found under Game/Saved/Config/Windows
 * 
 * Game.ini
 * GameUserSettings.ini
 * Input.ini
 * Engine.ini
 */
UENUM(BlueprintType, Category = "Easy Settings")
enum EIniFile
{
	Game,
	GameUserSettings,
	Input,
	Engine
};

/**
 * The various structs that derive from FSettingBase can be used in any uclass or blueprint that needs settings saved or retrieved from an ini file.
 * This allows settings to be adjusted through the editor for individual objects, rather than directly hard-coded in the game instance or manipulated through the ini file itself.
 *
 * If the default settings are changed in the editor while existing in an ini file, then the ini must be deleted (or reset through an in game menu) for the new defaults to take effect.
 */
USTRUCT(BlueprintType, Category = "Easy Settings")
struct FSettingBase
{
	GENERATED_BODY()

	/** The ini file we want to populate with this setting. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TEnumAsByte<EIniFile> IniFile = EIniFile::Game;

	/** The section in the ini file that this setting will be stored. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Section = "";

	/** The key in the ini file that the setting will be stored with. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Key = "";

	/** Returns the FileName associated with the current IniFile. */
	FString GetIniFileName()
	{
		switch (IniFile)
		{
		case EIniFile::Game:
			return GGameIni;
		case EIniFile::GameUserSettings:
			return GGameUserSettingsIni;
		case EIniFile::Input:
			return GInputIni;
		case EIniFile::Engine:
			return GEngineIni;
		}

		return FString("No match found!");
	}

	/** Returns the FileName associated with the current IniFile. */
	FString GetIniFileName() const 
	{
		switch (IniFile)
		{
		case EIniFile::Game:
			return GGameIni;
		case EIniFile::GameUserSettings:
			return GGameUserSettingsIni;
		case EIniFile::Input:
			return GInputIni;
		case EIniFile::Engine:
			return GEngineIni;
		}

		return FString("No match found!");
	}

	/** Returns a key used for our TMaps. */
	FString GetTMapKey() { return *UEnum::GetValueAsString(IniFile) + Section + Key; }

	/** Returns a key used for our TMaps. */
	FString GetTMapKey() const { return *UEnum::GetValueAsString(IniFile) + Section + Key; }
};
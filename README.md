Overview
========

This repository contains an Unreal Engine project with a working example of my EasySettings plugin. The plugin handles boilerplate logic for storing and retrieving custom ini variables via either C++ or blueprint.

This project provides a very simple pause menu with two setting types for adjusting turn rate and inverted controls.

The pause menu can be accessed through a new PIE session by pressing Shift + Esc.

![Pause Menu](ReadmeImages\PauseMenu.png)


SettingTypes
------------

**[SettingBase](Plugins\EasySettings\Source\EasySettings\Public\SettingTypes\SettingBase.h)**
- The various structs that derive from FSettingBase can be used in any uclass or blueprint that needs settings saved or retrieved from an ini file.
- This allows settings to be adjusted through the editor for individual objects, rather than directly hard-coded in the game instance or manipulated through the ini file itself.

**[FloatSetting](Plugins\EasySettings\Source\EasySettings\Public\SettingTypes\FloatSetting.h)**
- Used to ensure values stay within a specific range.

**[OpposingTogglesSetting](Plugins\EasySettings\Source\EasySettings\Public\SettingTypes\OpposingTogglesSetting.h)**
- Used to ensure only one of the toggles can be active at a time.


Widget Templates
---------------

Setting templates are available for use, and can be found in the plugin's content folder.

**WBP_NumberedSlider**
- Setup with a FloatSetting. Allows the user to adjust a FloatSetting by either manually entering a new value or adjusting the slider.

![Numbered Slider Widget](ReadmeImages\NumberedSlider.png)

**WBP_OpposingToggles**
- Setup with an OpposingTogglesSetting. Allows the user to choose between two options.

![Opposing Toggles Widget](ReadmeImages\OpposingToggles.png)

[EasySettingsSubsystem](Plugins\EasySettings\Source\EasySettings\Public\EasySettingsSubsystem.h)
---------------------

Stores all setting types that have been successfully setup. 
Contains delegates that notify whenever a setting is successfully updated.

![Subsystem Delegates](ReadmeImages\SubsystemDelegates.png)


[EasySettingsInterface](Plugins\EasySettings\Source\EasySettings\Public\EasySettingsInterface.h)
---------------------

An interface intended to be implemented by widgets that affect the individual setting types.

![Widget Interface](ReadmeImages\WidgetInterface.png)

**ResetToDefault**
- Signals the widget to reset to default values.
 
**ResetToLastApplied**
- Tells the widget to revert to the value that was last applied by the user.
 
**SettingApplied**
- Update the last applied value and setting reference.
 
**IsSettingDefault**
- Returns true if the setting is currently set to the default value.
 
**DidSettingChange**
- Returns true if the setting is different than the last applied value.


[EasySettingsStatics](Plugins\EasySettings\Source\EasySettings\Public\EasySettingsStatics.h)
-------------------

Static function library for ease of use between blueprint and C++.

![Static Functions](ReadmeImages\StaticFunctions.png)

**SetupFloatSetting**
- Stores a pointer to the setting in a TMap. Updates the setting with what is already present in the ini file or writes the defaults out if it isn't already there.

**GetFloatSetting**
- Finds the reference that was stored through the Setup function.

**UpdateFloatSetting**
- Attempts to update the setting reference and store the value in an ini file. Ensures the NewCurrent is within min and max values. Triggers the FloatSettingChanged_Delegate if successful.

**SetupOpposingTogglesSetting**
- Stores a pointer to the setting in a TMap. Updates the setting with what is already present in the ini file or writes the defaults out if it isn't already there.

**GetOpposingTogglesSetting**
- Finds the reference that was stored through the Setup function.

**UpdateOpposingTogglesSetting**
- Attempts to update the setting reference and store the value in an ini file. Triggers the OpposingTogglesSettingChanged_Delegate if successful.

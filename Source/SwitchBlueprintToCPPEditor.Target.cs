// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SwitchBlueprintToCPPEditorTarget : TargetRules
{
	public SwitchBlueprintToCPPEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("SwitchBlueprintToCPP");
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class MemoryGame_CPPTarget : TargetRules
{
	public MemoryGame_CPPTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "MemoryGame_CPP" } );
	}
}

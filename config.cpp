class CfgPatches
{
	class classslection_scripts
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"RPC_Scripts"
		};
	};
};

class CfgMods
{
	class classslection
	{
		
		dir = "d3xters-class-selction";
		picture = "";
		hideName = 1;
		hidePicture = 1;
		name = "DayZ Class-Selection Mod";
		action = "https://github.com/d3xter-dev";
		author = "D3XTER-dev";
		authorID = "LemmingTV"; 
		version = "0.1"; 
		extra = 0;
		type = "mod";
		dependencies[] = {"Game", "World", "Mission"};
		
		class defs
		{	
			class gameScriptModule
			{
				value = "";
				files[] = {"d3xters-class-selction/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"d3xters-class-selction/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"d3xters-class-selction/Scripts/5_Mission"};
			};
		};
	};
};
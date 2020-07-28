# DayZ Class-Selection

This mod adds an Class-Selection Menu to the game, server-owners can customize the classes and the weapons available. (via JSON files)
Each class can have many weapons to select from.

## Features

 * __Players will receive a fully loaded and ready to shoot weapon.__
 * Players can choose from diffrent items in each class
 * Players will respawn with the same class after death, but they can change it by pressing ","
 * Players will be forced to select a class on first join
 * Attachments already have batteries if needed


## Status: Beta
________________________________________________________

## Config:
Make sure to use the -profiles startparameter for your server! (e.g. -profiles=configs)  

Use the ClassDataExample.json as template and create your classes in the classes folder.
![Folder](https://i.imgur.com/GzOtUqg.png)

## Incompatible Mods:
* Expansion Mod

### Important!
__(Following needed if you override the "EquipCharacter()" function in your CustomMission (init.c) or via. a modded MissionServer class.)__
Make sure that you call "GetClassSelection().GiveClassEquipment(m_player);" and don't clear player items after that call, for obvious reasons.

## Future-Development:

*  Json for general items given to all players
*  Whitelist for classes
*  Add Quantity Field to utility items / add own type
*  Add support for container items e.g. first aid kit etc.
* Attachment selection Menu
* Clothing selection Menu

### Please give me credits if you use / modify the sourcecode of my mod!

# Useful Commands:
## Restart & Rebuild Mod

```Stop-Process -Name "DayZDiag_x64";Start-Process -FilePath "C:\Program Files\PBO Manager v.1.4 beta\PBOConsole.exe" -ArgumentList '-pack "P:\d3xters-class-selection\Scripts" "P:\d3xters-class-selection\Addons\d3xters-class-selection.pbo"';./DayZDiag_x64.exe -server -config="serverDZ.cfg" "-mod=RPCFramework;d3xters-class-selection" -nopause -filePatching -profiles=configs;./DayZDiag_x64.exe -nopause "-mod=RPCFramework;d3xters-class-selction" -filePatching -connect=192.168.178.39 -port=2302;```


## Restart Only Server & Rebuild Mod

```Stop-Process -Name "DayZDiag_x64";Start-Process -FilePath "C:\Program Files\PBO Manager v.1.4 beta\PBOConsole.exe" -ArgumentList '-pack "P:\d3xters-class-selection\Scripts" "P:\d3xters-class-selection\Addons\d3xters-class-selection.pbo"';./DayZDiag_x64.exe -server -config="serverDZ.cfg" "-mod=RPCFramework;d3xters-class-selection" -nopause -filePatching -profiles=configs;```



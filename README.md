# DayZ Class-Selection

This mod adds an Class-Selection Menu to the game, server-owners can customize the classes and the weapons available. (via JSON files)
Player will be forced to select a class on first join, after that they can change their class with "," and will respawn with the same Class on death,

Each class can have many weapons to select from.
__The player will receive a fully loaded and ready to shoot weapon.__

## Status: Beta
________________________________________________________

## Config:
Make sure to use the -profiles startparameter for your server! (e.g. -profiles=configs)  

Use the ClassDataExample.json as template and create your classes in the classes folder.
![Folder](https://i.imgur.com/GzOtUqg.png)

## Incompatible Mods:
* Expansion Mod

__Make sure that you DON'T override the "EquipCharacter()" function in your CustomMission or via. a modded MissionServer class / don't clear the player items there, if you use it.__

## Future-Development:

* Json for general items given to all players
* Add Quantity Field to utility items / add own type
* Add support for container items e.g. first aid kit etc.
* Attachment selection Menu
* Clothing selection Menu

### Please give me credits if you use / modify the sourcecode of my mod!

# Useful Commands:
## Restart & Rebuild Mod

```Stop-Process -Name "DayZDiag_x64";Start-Process -FilePath "C:\Program Files\PBO Manager v.1.4 beta\PBOConsole.exe" -ArgumentList '-pack "P:\d3xters-class-selection\Scripts" "P:\d3xters-class-selection\Addons\d3xters-class-selection.pbo"';./DayZDiag_x64.exe -server -config="serverDZ.cfg" "-mod=RPCFramework;d3xters-class-selection" -nopause -filePatching -profiles=configs;./DayZDiag_x64.exe -nopause "-mod=RPCFramework;d3xters-class-selction" -filePatching -connect=192.168.178.39 -port=2302;```


## Restart Only Server & Rebuild Mod

```Stop-Process -Name "DayZDiag_x64";Start-Process -FilePath "C:\Program Files\PBO Manager v.1.4 beta\PBOConsole.exe" -ArgumentList '-pack "P:\d3xters-class-selection\Scripts" "P:\d3xters-class-selection\Addons\d3xters-class-selection.pbo"';./DayZDiag_x64.exe -server -config="serverDZ.cfg" "-mod=RPCFramework;d3xters-class-selection" -nopause -filePatching -profiles=configs;```



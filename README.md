# dayz-class-selection
DayZ Class Selection Mod

# Useful Commands:
## Restart & Rebuild Mod

```Stop-Process -Name "DayZDiag_x64";Start-Process -FilePath "C:\Program Files\PBO Manager v.1.4 beta\PBOConsole.exe" -ArgumentList '-pack "P:\Test Mod\Addons\Scripts" "P:\Test Mod\Addons\Scripts.pbo"';./DayZDiag_x64.exe -server -config="serverDZ.cfg" "-mod=RPCFramework;Test Mod" -nopause -filePatching -profiles=C:\Users\jakob\Documents\DayZ Other Profiles\Server;./DayZDiag_x64.exe -nopause "-mod=RPCFramework;Test Mod" -filePatching -connect=192.168.178.39 -port=2302;```


## Restart Only Server & Rebuild Mod

```Stop-Process -Name "DayZDiag_x64";Start-Process -FilePath "C:\Program Files\PBO Manager v.1.4 beta\PBOConsole.exe" -ArgumentList '-pack "P:\Test Mod\Addons\Scripts" "P:\Test Mod\Addons\Scripts.pbo"';./DayZDiag_x64.exe -server -config="serverDZ.cfg" "-mod=RPCFramework;Test Mod" -nopause -filePatching -profiles=C:\Users\jakob\Documents\DayZ Other Profiles\Server;```

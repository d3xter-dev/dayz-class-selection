# dayz-class-selection
DayZ Class Selection Mod

# Useful Commands:
## Restart & Rebuild Mod

```Stop-Process -Name "DayZDiag_x64";Start-Process -FilePath "C:\Program Files\PBO Manager v.1.4 beta\PBOConsole.exe" -ArgumentList '-pack "P:\d3xters-class-selection\Scripts" "P:\d3xters-class-selection\Addons\d3xters-class-selection.pbo"';./DayZDiag_x64.exe -server -config="serverDZ.cfg" "-mod=RPCFramework;d3xters-class-selection" -nopause -filePatching -profiles=configs;./DayZDiag_x64.exe -nopause "-mod=RPCFramework;d3xters-class-selction" -filePatching -connect=192.168.178.39 -port=2302;```


## Restart Only Server & Rebuild Mod

```Stop-Process -Name "DayZDiag_x64";Start-Process -FilePath "C:\Program Files\PBO Manager v.1.4 beta\PBOConsole.exe" -ArgumentList '-pack "P:\d3xters-class-selection\Scripts" "P:\d3xters-class-selection\Addons\d3xters-class-selection.pbo"';./DayZDiag_x64.exe -server -config="serverDZ.cfg" "-mod=RPCFramework;d3xters-class-selection" -nopause -filePatching -profiles=configs;```



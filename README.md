# dayz-class-selection
DayZ Class Selection Mod

# Useful Commands:
## Restart & Rebuild Mod

```Stop-Process -Name "DayZDiag_x64";Start-Process -FilePath "C:\Program Files\PBO Manager v.1.4 beta\PBOConsole.exe" -ArgumentList '-pack "P:\Test Mod\Addons\Scripts" "P:\Test Mod\Addons\Scripts.pbo"';./DayZDiag_x64.exe -server -config="serverDZ.cfg" "-mod=Test Mod" -nopause -filePatching;./DayZDiag_x64.exe -config="serverDZ.cfg"  -nopause "-mod=Test Mod" -filePatching -connect=192.168.178.39 -port=2302;```

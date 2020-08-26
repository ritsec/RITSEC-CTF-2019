This was only supposed to be one challenge but I sent the admins the "unpatched" win32k.sys with the flag in it.  
Patch Tuesday
=============
`strings -e l win32k.sys | grep RIT`  
OR
explorer.exe -> Right click -> Properties -> Details -> Look in Copyright field.  
Flag was in UTF-16LE (windows strings) so you couldn't just run regular strings.  
The included delta was worthless.  

Patch 2sday
===========
The challenge that was supposed to be the only challenge.  
The patch file is a PA30 msdelta patch delta generated with msdelta.CreateDeltaA and then it is prepended with a CRC32 checksum of the rest of the patch. This is something that actual Microsoft patch deltas do for some reason.  
To apply the patch use ApplyDeltaB, ApplyDeltaA, or ApplyDeltaW and then the flag is in the same place as the first one.  
More info here:  https://docs.microsoft.com/en-us/windows/deployment/update/psfxwhitepaper  
Interface here:  https://github.com/nihon-tc/Rtest/blob/master/header/Microsoft%20SDKs/Windows/v7.0A/Include/msdelta.h  

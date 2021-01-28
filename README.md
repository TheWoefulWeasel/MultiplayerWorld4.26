# MultiplayerWorld4.26

Uses Git LFS and is required to properly download some of the files
------------------------------------------------------------------------------------------------

Steam API enabled project allowing clients to connect to sessions via internet using Steam API

------------------------------------------------------------------------------------------------

The default setting for the project is set to the NULL Subsystem.

This is because to test through Steam you must have a logged in Steam client running on the pc.

This also means that to test the Steam functionality you will require 2 separate pc's with different
Steam accounts innorder to connect.

If you would like to test with Steam..
- Replace the Config-> DefaultEngine.ini file with the one in STEAMSUB
- Head to the GameInstance_Root.cpp -- Lines 103 -> 111 -- comment out which Subsystem is not in use


Finally.. To launch the game for steam, either launch from the .uproject or through windows powershell
because playing through the UE4 editor doesn't work with steam

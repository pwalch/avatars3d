#!/bin/sh

# 3D Avatars, Pierre Walch
# If multiple configuration files must be processed, this script can be used as an example.
# Please note this script is run from the "Contexts" working directory.


cd ./wdISSIA
T1=$(date +%s)
# Do something here
../../Resources/Launchers/launcher.pl ../../Code/Build-Release/Avatars config_ISSIA_Record_Traj.xml
T2=$(date +%s)
diffsec="$(expr $T2 - $T1)"
echo "Football done in $diffsec seconds"

sleep 3

cd ../wdBrazilRussia
T1=$(date +%s)
../../Resources/Launchers/launcher.pl ../../Code/Build-Release/Avatars config_BrazilRussia_Record_TrajBall.xml
T2=$(date +%s)
diffsec="$(expr $T2 - $T1)"
echo "Volleyball done in $diffsec seconds"


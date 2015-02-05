#!/bin/sh

# Simplification script to call default launcher and default executable directly.

if [ $# -eq 1 ]; then
	config="$1"
	../../Resources/Launchers/launcher.pl ../../Code/Build-Release/Avatars "$config"
fi

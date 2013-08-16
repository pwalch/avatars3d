#!/bin/sh

# 3D Avatars, Pierre Walch

usage () {
    echo "USAGE : ./avatars.sh configFile"
}

help () {
    usage
    echo "-h or -help   : Displays help message"
    echo "configFile    : program settings XML file"
    exit 0;
}

error () {
    echo "ERROR : $1" >&2
    usage
    exit 1;
}

# Providing help
if [ $# -eq 1 ]; then
    if [ "$1" = "-h" ] || [ "$1" = "-help" ]; then
        help
    else
        case "$1" in
            *.xml)
                cfgFile="$1"
                ./Avatars "$cfgFile"
                # videoNameAVI=$(echo "$line" | awk -F"|" '{print $1}')
                # videoNameMP4=$(basename "$videoNameAVI" .avi).mp4
                # if [ -e "$videoNameMP4" ]; then
                #     rm "$videoNameMP4"
                # fi
                # ffmpeg -loglevel panic -i "$videoNameAVI" -sameq "$videoNameMP4" >/dev/null 2>&1
                # rm "$videoNameAVI"
            ;;

            *)
                error "settings file must be an XML file"
            ;;
        esac
    fi
else
    error "Invalid number of arguments"
fi

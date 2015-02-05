#!/usr/bin/perl

# 3D Avatars, Pierre Walch

# This script generates a camera trajectory performing
# a linear move from a point to another on an axis parallel to X-axis

use strict;
use warnings;

open my $dest, ">", "BrazilRussia_camera.tc";

my $frameNumber = 1500;
my $lastFrame = $frameNumber - 1;

foreach my $i (0..$lastFrame) {
	my $startPosX = 24100.0;
	my $endPosX = 6100.0;
	my $posX = $startPosX - $i * abs($endPosX - $startPosX) / $frameNumber;
	my $posY = 22000.0;
	my $posZ = 8000.0;
	my $rotX = 30.0;
	my $rotY = 180.0;
	print $dest $i, " ", $posX, " ", $posY, " ", $posZ, " ", $rotX, " ", $rotY, " ", 0, "\n";
}

close $dest;
#!/usr/bin/perl

# 3D Avatars, Pierre Walch

# This script generates a camera trajectory performing
# a linear uprising move

use strict;
use warnings;

open my $dest, ">", "BrazilRussia_camera.tc";

my $frameNumber = 1500;
my $lastFrame = $frameNumber - 1;

foreach my $i (0..$lastFrame) {
	my $posX = 15100.0;
	
	my $startPosY = 22000.0;
	my $endPosY = 17000.0;
	my $posY = $startPosY - $i * abs($endPosY - $startPosY) / $frameNumber;
	
	my $startPosZ = 2000.0;
	my $endPosZ = 25000.0;
	my $posZ = $startPosZ + $i * abs($endPosZ - $startPosZ) / $frameNumber;

	my $startRotX = 0.0;
	my $endRotX = 89.0;
	my $rotX = $startRotX + $i * abs($endRotX- $startRotX) / $frameNumber;

	my $rotY = 180.0;
	print $dest $i, " ", $posX, " ", $posY, " ", $posZ, " ", $rotX, " ", $rotY, " ", 0, "\n";
}

close $dest;
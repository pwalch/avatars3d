#!/usr/bin/perl

# 3D Avatars, Pierre Walch

# This script generates a camera trajectory performing a circular move around a point of the scene

use strict;
use warnings;

open my $dest, ">", "ISSIA_camera.tc";

my $frameNumber = 2900;
my $lastFrame = $frameNumber - 1;

foreach my $i (0..$lastFrame) {
	my $angle = 180 - $i / 1500.0 * 180;

	my $theta = (3 * 3.14 / 2) + $i / 1500 * (3.14);
	my $centreX = 52000;
	my $centreY = 36000;
	my $radius = 40000;
	my $height = 20000;
	my $x = $centreX + $radius * (cos $theta);
	my $y = $centreY + $radius * (- sin $theta);
	print $dest $i, " ", $x, " ", $y, " ", $height, " ", 30, " ", $angle, " ", 0, "\n"; 
}

close $dest;

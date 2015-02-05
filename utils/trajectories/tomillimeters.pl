#!/usr/bin/perl

use strict;
use warnings;

open my $src, "<", "src.txt";
open my $dest, ">", "dest.txt";

while(<$src>) {
	my @line = split /\s+/, $_;
	foreach my $i (1..3) {
		$line[$i] = $line[$i] * 1000.0;
	}
	print $dest $line[0], " ", $line[1], " " , $line[2], " ", $line[3], "\n";
	#print $dest join @line, " ";
}

close $dest;
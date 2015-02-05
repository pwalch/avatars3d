#!/usr/bin/perl

use strict;
use warnings;

# Transforms ball coordinates by taking account of an offset

open my $src, "<", "src.txt";
open my $dest, ">", "dest.txt";

while(<$src>) {
	my @line = split /\s+/, $_;
	 
	$line[1] = $line[1] + 3400;
	$line[2] = $line[2] + 3400;
	print $dest $line[0], " ", $line[1], " " , $line[2], " ", $line[3], "\n";
	#print $dest join @line, " ";
}

close $dest;

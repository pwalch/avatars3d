#!/usr/bin/perl

# 3D Avatars, Pierre Walch

use strict;
use warnings;


sub error {
	my @args = @_;
	print $args[0], "\n";
	&usage();
	exit(1);
}

sub usage {
	print "Usage : ./avatars.pl CONFIG_FILE\n";
}

my $argNumber = scalar @ARGV;

if ($argNumber == 1) {
	my $path = $ARGV[0];
	open my $cfg, "<", $path;
	while (<$cfg>) {
		my @matches = ($_ =~ m/<video name=\"(.+\.avi)/g);
		if (scalar @matches == 1) {
			my $nameAVI = $matches[0];
			# Execute program
        	system("./Avatars $path");
			# Check if output file has been created
            if (-f $nameAVI) {
            	my $nameMP4 = $nameAVI;
            	$nameMP4 =~ s/.avi/.mp4/;
            	# Delete converted file if it already exists, we will create a new one
            	if (-f $nameMP4) {
	            	unlink $nameMP4;
            	}
                # Use FFMPEG to convert the AVI file to MP4
            	system("ffmpeg -loglevel panic -i $nameAVI -sameq $nameMP4 >/dev/null 2>&1");
            	unlink $nameAVI;
				exit(0);
            }
		}
	}
	&error("Configuration file is invalid, or program did not work correctly");
} else {
	&error("Number of arguments is invalid");
}

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
	my $nameAVI = "unknown";
	my $isConsole = "unknown";
	while (<$cfg>) {
		my @matchesVideo = ($_ =~ m/<video name=\"(.+\.avi)/);
		
		if (scalar @matchesVideo == 1) {
			$nameAVI = $matchesVideo[0];
		}

		my @matchesConsole = ($_ =~ m/<window console=\"(true|false)/);
		if(scalar @matchesConsole == 1) {
			$isConsole = $matchesConsole[0];
		}
	}

	if($nameAVI eq "unknown" || $isConsole eq "unknown") {
		error("Video name or console mode could not be found in configuration file");
	}
	# Execute program
	system("./Avatars $path");

	if($isConsole eq "true") {
		# Check if output file has been created
		if (! -f $nameAVI) {
			error("Output XVID video file could not be found after running the program");
		}

		my $outputFormat = "mkv";
		my $nameConverted = $nameAVI;
		$nameConverted =~ s/\.avi/\.$outputFormat/;

		# Delete converted file if it already exists, we will create a new one
		if (-f $nameConverted) {
	    	unlink $nameConverted;
		}

	    # Use FFMPEG to convert the AVI file to MP4

	    # only repacking container
		system("ffmpeg -loglevel panic -i $nameAVI -acodec copy -vcodec copy $nameConverted >/dev/null 2>&1");

		# complete transcoding (quality loss may occur)
		# system("ffmpeg -loglevel panic -i $nameAVI -sameq $nameConverted >/dev/null 2>&1");
		
		# Delete XVID file because we now have a proper MP4 file
		unlink $nameAVI;
	}
	exit(0);

	&error("Configuration file is invalid, or program did not work correctly");
} else {
	&error("Number of arguments is invalid");
}

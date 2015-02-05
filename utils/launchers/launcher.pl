#!/usr/bin/perl

# 3D Avatars, Pierre Walch
# Launcher script of the program. The program must not be called directly.

use strict;
use warnings;


sub error {
	my @args = @_;
	print $args[0], "\n";
	&usage();
	exit(1);
}

sub usage {
	print "Usage from working context : /path/to/launcher.pl /path/to/executable config.xml\n";
}

my $argNumber = scalar @ARGV;

if ($argNumber == 2) {
	my $exePath = $ARGV[0];
	my $cfgPath = $ARGV[1];

	open my $cfg, "<", $cfgPath or error("Unable to load configuration file");
	my $nameAVI = "unknown";
	my $isConsole = "unknown";
	my $fullscreen = "unknown";
	while (<$cfg>) {
		my $line = $_;

		my @matchesVideo = ($line =~ m/<video.+name=\"(.+\.avi)/);
		if (scalar @matchesVideo == 1) {
			$nameAVI = $matchesVideo[0];
		}

		my @matchesConsole = ($line =~ m/<mode\s+console=\"(true|false)/);
		if(scalar @matchesConsole == 1) {
			$isConsole = $matchesConsole[0];
			if($isConsole eq "true") {
				$isConsole = 1;
			} elsif($isConsole eq "false") {
				$isConsole = 0;
			} else {
				$isConsole = "unknown";
			}
		}

		my @matchesFullscreen = ($line =~ m/<window.+fullscreen=\"(.+)\"/);
		if (scalar @matchesFullscreen == 1) {
			$fullscreen = $matchesFullscreen[0];
			if($fullscreen eq "true") {
				$fullscreen = 1;
			} elsif($fullscreen eq "false") {
				$fullscreen = 0;
			} else {
				$fullscreen = "unknown";
			}
		}
	}

	if($nameAVI eq "unknown" || $isConsole eq "unknown" || $fullscreen eq "unknown") {
		&error("Video name, console mode or fullscreen attribute could not be found in configuration file");
	}

	if((!$isConsole) && $fullscreen) {
		&error("Fullscreen cannot be used in GUI mode.");
	}

	# Execute program
	my $avOut = `$exePath $cfgPath 2>&1`;
	my $avStatus = $?;
	print "$avOut";

	if(($avStatus == 0) && $isConsole) {
		# Check whether output file has been created
		if (! -f $nameAVI) {
			&error("Output XVID video file could not be found after running the program");
		}

		my $outputFormat = "mp4";
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

} else {
	&error("Number of arguments is invalid");
}

#!/usr/bin/perl
 
use strict;
use warnings;
use Getopt::Long;
use File::Copy;
use Cwd;

sub generate_target
{
	my @vars = (@_);

	mkdir "build";
	chdir "build";

	system("cmake -G $vars[1] -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DCMAKE_TOOLCHAIN_FILE=../$vars[0]ToolChain.txt \"-DBinaries=$vars[4]\" -DMode:STRING=$vars[3] -DSolutionName=$vars[2] -DOptMode:STRING=$vars[5] -DCMAKE_INSTALL_PREFIX:STRING=$vars[6] VERBOSE=1 ..");

	chdir "..";
}

sub showHelp
{
	print "For executing properly this bat file you shall provide a project to be produced and a supported platform (default value is Windows).";
}

my $project = '';	# option variable with default value
my $platform = '';
if($^O eq 'linux')
{
	$platform = 'Linux';
}
elsif($^O eq "darwin")
{
	$platform = 'iOS';
}
elsif($^O eq "MSWin32")
{
	$platform = 'Windows';
}

my $Compiler = "\"Visual Studio 14 2015 Win64\"";
my $SolutionName = '';
my $Mode='';
my $Binaries='';
my $optmode='Release';
my $installdir=getcwd . '/bin';

GetOptions ('project=s' => \$project, 'platform=s' => \$platform, 'Mode=s' => \$optmode, 'install_dir=s' => \$installdir);

if($project eq "Windows")
{
	$Mode='ALL_Windows';
	$Binaries='EwasFramework';
	$SolutionName='eWasWindows';
}
elsif($project eq "Linux")
{
	$Mode='ALL_Linux';
	$Binaries='EwasFramework';
	$SolutionName='eWasLinux';
}
elsif($project eq "iOS")
{
	$Mode='ALL_iOS';
	$Binaries='EwasFramework';
	$SolutionName='eWasIOS';
}
else
{
	showHelp();
	
	exit -1;
}

if($platform eq "Windows")
{
	if($Mode eq "Pilot32")
	{
		generate_target('win32',"\"Visual Studio 10 2010\"",$SolutionName,$Mode,$Binaries,$optmode,$installdir);
	}
	else
	{
		generate_target('win64',$Compiler,$SolutionName,$Mode,$Binaries,$optmode,$installdir);
	}
}
elsif($platform eq "Linux")
{
	generate_target('linux64',"\"CodeBlocks - Unix Makefiles\"",$SolutionName,$Mode,$Binaries,$optmode,$installdir);
}
elsif($platform eq "iOS")
{
	generate_target('iOS',"\"Xcode\"",$SolutionName,$Mode,$Binaries,$optmode,$installdir);
}
else
{
	showHelp();
	
	exit -1;
}

exit 0;

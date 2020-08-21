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

	system("cmake -G $vars[1] -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DCMAKE_TOOLCHAIN_FILE=../$vars[0]ToolChain.txt \"-DBinaries=$vars[4]\" -DMode:STRING=$vars[3] -DSolutionName=$vars[2] -DOptMode:STRING=$vars[5] -DCMAKE_INSTALL_PREFIX:STRING=$vars[6] -DARCHITECTURE:STRING=$vars[7] VERBOSE=1 ..");

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

my $Compiler = "\"Visual Studio 15 2017 Win64\"";
my $SolutionName = '';
my $Mode='';
my $Binaries='';
my $optmode='Release';
my $installdir=getcwd . '/bin';
my $architecture= 'x86-64';

GetOptions ('project=s' => \$project, 'platform=s' => \$platform, 'Mode=s' => \$optmode, 'install_dir=s' => \$installdir, 'arch=s' => \$architecture);

if($project eq "Windows")
{
	$Mode='ALL_Windows';
	$Binaries='ddkFramework';
	$SolutionName='ddkWindows';
}
elsif($project eq "ALL_Linux")
{
	$Mode='ALL_Linux';
	$Binaries='ddkFramework';
	$SolutionName='ddkLinux';
}
elsif($project eq "iOS")
{
	$Mode='ALL_iOS';
	$Binaries='ddkFramework';
	$SolutionName='ddkIOS';
}
elsif($project eq "Test")
{
    if($platform eq "Windows")
    {
	$Mode='ALL_Windows';
    }
    elsif($platform eq "Linux")
    {
	$Mode='ALL_Linux';
    }
    
    $optmode='Debug';
    $Binaries='ddkFrameworkTests';
    $SolutionName='Test';
}
else
{
	print "$platform";
	showHelp();
	
	exit -1;
}

if($platform eq "Windows")
{
	if($Mode eq "Pilot32")
	{
		generate_target('win32',"\"Visual Studio 10 2010\"",$SolutionName,$Mode,$Binaries,$optmode,$installdir,$architecture);
	}
	else
	{
		generate_target('win64',$Compiler,$SolutionName,$Mode,$Binaries,$optmode,$installdir,$architecture);
	}
}
elsif($platform eq "Linux")
{
	generate_target('linux64',"\"CodeBlocks - Unix Makefiles\"",$SolutionName,$Mode,$Binaries,$optmode,$installdir,$architecture);
}
elsif($platform eq "iOS")
{
	generate_target('iOS',"\"Xcode\"",$SolutionName,$Mode,$Binaries,$optmode,$installdir,$architecture);
}
else
{
	echo $platform
	showHelp();
	
	exit -1;
}

exit 0;

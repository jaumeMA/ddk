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

if($project eq 'WinServices')
{
	$Mode='Dispatch';
	$Binaries='WDSPAlertServer;WDSPDispatchServer;WDSPPilotServer;WeatherDataDetectionImportation;DispatchDataProcessor;GlobalPositionDSP';
	$SolutionName='eWasWinServices';
}
elsif($project eq "Test")
{
	$Mode='Dispatch';
	$Binaries='DispatchSerializationLibTests;EffNodeApiTests;EwasUtilsTests;MetarTafLibTests;MeteoParserLibTests;NavigationTabsDisplayTests;PilotLibTests;PilotSerializationLibTests;SerializationLibTests;ServerDataLibTests;UnitTests;WDSPConnectionLibTests;WDSPServerCommonTests;eWasPilotTests;EuroControlFlightPlanExporterTests;EwasFrameworkTests';
	$SolutionName='Test';
}
elsif($project eq "ALL_Windows")
{
	$Mode='ALL_Windows';
	$Binaries='eWasPilot;eWasDispatch;eWASTest;eWASLicense;WDSPAlertServer;WDSPDispatchServer;WDSPPilotServer;WeatherDataDetectionImportation;DispatchDataProcessor;GlobalPositionDSP';
	$SolutionName='eWasWindows';
}
elsif($project eq "ALL_Linux")
{
	$Mode='ALL_Linux';
	$Binaries='eWASTest;WeatherDataDetectionImportation;DispatchDataProcessor;WDSPAlertServer;eWASMQClient;eWASFirehoseClient;eWASAMQPClient;eWASPPSClient;EuroControlFlightPlanExporter;WDSPAlertServer;WDSPDispatchServer;WDSPPilotServer;GlobalPositionDSP';
	$SolutionName='eWasLinux';
}
elsif($project eq "ALL_iOS")
{
	$Mode='ALL_iOS';
	$Binaries='ConsoleController';
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
elsif($platform eq "Dassault")
{
	generate_target('win64',$Compiler,$SolutionName,$Mode,$Binaries,$optmode,$installdir);	
	
	system('perl -pi.bak -e s/"<AdditionalOptions>[^<]*<\/AdditionalOptions>"/"<AdditionalOptions><\/AdditionalOptions>"/g ./build/eWASdisplayActiveX/eWASdisplayActiveX.vcxproj');
	copy("src/FalconSphere_eWAS/FalconSphere_eWAS.csproj","build/FalconSphere_eWAS/FalconSphere_eWAS.csproj");
	copy("src/FalconSphereConnectorLib/FalconSphereConnectorLib.csproj","build/FalconSphereConnectorLib/FalconSphereConnectorLib.csproj");
}
else
{
	showHelp();
	
	exit -1;
}

exit 0;

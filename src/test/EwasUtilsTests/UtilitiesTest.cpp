#include <gtest/gtest.h>

#include "Utilities.h"

#include <boost/regex.hpp>

using namespace testing;

class WrapTextTest : public TestWithParam<std::string>
{
};

TEST_P(WrapTextTest, WrapText)
{
	std::string text = GetParam();

	for(unsigned i = 0; i < text.size(); ++i)
	{
		Utilities::wrapText(text, i);

		text = boost::regex_replace(text, boost::regex("<br>"), " ");

		EXPECT_EQ(text, GetParam());
	}
}

INSTANTIATE_TEST_SUITE_P(UtilitiesTest, WrapTextTest, Values(
	"LEBL 251500Z 16006KT 9999 -DZ FZ FEW025 SCT035 11/03 Q1016 NOSIG",
	"LEBL 251100Z 2512/2612 13007KT 9999 SCT020 SCT040 TX12/2512Z TN06/2606Z PROB30 TEMPO 2512/2513 33008KT DS BECMG 2515/2517 23007KT TEMPO 2518/2524 VRB33KT SS BECMG 2522/2601 33007KT BECMG 2607/2609 06012KT PROB30 TEMPO 2609/2612 4000 SHRA FEW015CB",
	"LFPG 081100Z AUTO VRB07KT 9999 SCT027 SCT036 BKN043 17/10 Q1016 TEMPO SCT030TCU",
	"LFPG 080500Z 0806/0912 36008KT CAVOK TX19/0814Z TN10/0904Z TEMPO 0810/0818 SCT030TCU PROB30 TEMPO 0815/0817 36015G25KT -SHRA BKN040TCU PROB30 TEMPO 0904/0906 BKN008",
	"WSZA21 FAOR 081013 FAJO SIGMET F01 VALID 081010/081400 FAOR- FAJO JOHANNESBURG OCEANIC FIR SEV TURB FCST WI S3158 E01153 - S3258 E01500 - S3524 E01500 - S3521 E01449 - S3158 E01153 FL390/450=",
	"WSAU21 AMMC 080701 YMMM SIGMET B10 VALID 080722/081122 YMMC- YMMM MELBOURNE FIR SEV TURB FCST WI S3116 E08449 - S3406 E08359 - S4010 E09240 - S4000 E08410 - S3240 E07500 - S2640 E07500 FL150/300 MOV N 10KT NC="));

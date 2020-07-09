#pragma once

#include "eWASConstants.h"
#include "FMSStructures.h"

#define MAX_RANGE        						926000 //=500NM
#define ZERO_NEGATIVE_LAT						127
#define ZERO_NEGATIVE_LON						255
#define TIME_RESOLUTION_IN_S					30
#define PERIOD_RESOLUTION_IN_M					5
#define PRODUCT_NAME_SIZE						64
#define DEVICE_ID_SIZE							128
#define DEVICE_DESC_SIZE						128
#define VERSION_DESC_SIZE						128
#define MODEL_AC_SIZE							128
#define	FLIGHT_NUMBER_SIZE						8
#define MAX_FLIGHT_REQUEST_DESC					512
#define MAX_FPLIST_SIZE_V1						50
#define MAX_FPLIST_SIZE_V2						511
#define MAX_FPLIST_SIZE_DISPATCH				10
#define TIME_STRING_SIZE						19
#define MAX_AIRPORTS_LIST_SIZE					127
#define MAX_AIRPORTS_LIST_SIZE_V2				511
#define MAX_AIRPORTS_LIST_SIZE_V3				1023
#define MAX_AIRLINE_CODE_SIZE					3
#define DECLINATION_RESOLUTION					10.0
#define DISPATCH_MAX_OFFICELIST_SIZE			63
#define DISPATCH_MAX_FPLIST_SIZE				63
#define OST_TRACK_ID_SIZE						32
#define DISPATCH_MAX_HOURS_OFFSET				31
#define POS_MESSAGE_TYPE_SIZE					4
#define CRUISE_SPEED_MODE_SIZE					10
#define STATIC_AIR_TEMP_SIZE					4
#define WV_MIX_RATIO_SIZE						5
#define TURBULENCE_SIZE							5

typedef enum {
	MessageTypeBits,
	MessageIdBits,
	MessageSizeBits,
	PacketLengthBits,
	UserIdBits,
	VersionBits,
	CodeBits,
	ProductBits,
	AnalysisTimeBits,
	ExpirationTimeBits,
	NextIssuedTimeBits,
	ValidityTimeBits,
	ValidityPeriodBits,
	NbPredictionsBits,
	HazardBits,
	NbWeatherObjectsBits,
	NbPolygonPointsBits,
	AltitudeBits,
	PolygonHeightBits,
	SeverityBits,
	SignalBits,
	DecimalBits,
	IntegerLatitudeBits,
	IntegerLongitudeBits,
	DiffLatLonBits,
	LayerBits,
	SpeedBits,
	HeadingBits,
	FlightLevelBits,
	ACModelCharBits,
	FNCharBits,
	CharBits,
	SigmetReportBits,
	SigmetCoverageBits,
	SigmetOutlookBits,
	PirepSubtypeBits,
	PirepNofValuesBits,
	PirepNbCharsCommentBits,
	PirepNbCharsPropertiesBits,
	PirepValuesBits,
	LightningIntensityBits,
	GroundSpeedBits,
	AOILatLonBits,
	AOIWidthBits,
	TOIBehindBits,
	TOIAheadBits,
	TOITTABits,
	ModeBits,
	SimulationTimeBits,
	RequestTimeBits,
	ErrorCodeBits,
	FPFlightNumberBits,
	FPDepartureTimeBits,
	FPDescLengthBits,
	FPNbWPBits,
	FPWPNameBits,
	FPWPTypeBits,
	FPConstraintBits,
	UserTypeBits,
	UserNickBits,
	UserPwdBits,
	TestSizeBits,
	TropoAltitudeBits,
	NWDListSizeBits,
	NWDModeBits,
	NWDStatusBits,
	NWDOOCBits,
	NWDUpdTimeBits,
	NWDLastTimeBits,
	NWDProductBits,
	CustoEnableBits,
	CustoProductListSizeBits,
	CustoLogoSizeBits,
	AirportBits,
	AirlineBits,
	FlightPlanListMaxSizeBitsV1,
	TrendBits,
	WindSpeedBits,
	WindDirectionBits,
	WindTemperatureBits,
	WeatherReqFPNbPointsBits,
	WeatherReqFPCruiseFlBits,
	CustoFeedbackBits,
	NbAirportsBits,
	AirportNameBits,
	MetarReportTypeBits,
	MetarTafNbCharBits,
	AuthTypeBits,
	AuthParamLengthBits,
	CustoSimulationBits,
	CustoExpirationTimeBits,
	AirportElevationBits,
	AirportIFRBits,
	NbRunwaysBits,
	RunwayIdentifierBits,
	RunwayLatLonDecBits,
	RunwayDistanceBits,
	RunwayDisplacementBits,
	RunwayStopwayBits,
	RunwayWidthBits,
	RunwayBearingBits,
	RunwayLandingSystemBits,
	RunwayNOfApproachBits,
	RunwayApproachSystemBits,
	AirportModeBits,
	AirportLevelBits,
	ACTypeBits,
	FlightPlanListMaxSizeBitsV2,
	JetstreamNbPointsBits,
	JetstreamLatBits,
	JetstreamLonBits,
	JetstreamDirectionBits,
	JetstreamValueBits,
	EnterpriseIdBits,
	NOfLicensesBits,
	NotamNbTextBits,
	NotamNbCharBits,
	FeedbackNameSizeBits,
	FeedbackSizeBits,
	WindMatrixLatBits,
	WindMatrixLonBits,
	FileBlockBits,
	FileTypeBits,
	FileNameSizeBits,
	AirportFavouriteBit,
	AirportVisibleBit,
	AppTypeBits,
	LatitudeWindIndexBits,
	LongitudeWindIndexBits,
	OFPVersionBits,
	ValidityTimeBitsV2,
	FPNbWPBitsV2,
	DecimalBitsV2,
	GroundSpeedBitsV2,
	TaxiTimeBits,
	AnalysisTimeBitsV2,
	AirportSuitabilityBits,
	CustoAirlineRegisteredBits,
	NbAirportsBitsV2,
	TAOITypeBits,
	FPListTypeBits,
	ProviderIdBits,
	DataConsumption,
	FlightTimeBits,
	FlightPlansNumberBits,
	OfficesNumberBits,
	OfficeIDBits,
	EtopsTypeBits,
	EtopsTimeBits,
	AlternateRoutesNbBits,
	WeatherRequestType,
	WeatherNbFlightLevels,
	OSTNumTracks,
	OSTTrackTypeNumBits,
	OSTTrackId,
	NbAirportsBitsV3,
	OffsetTime,
	AirportModeBitsV2,
	FuelBits,
	PosMessageTypeBits,
	DuplicateNumberBits,
	MachNumberBits,
	CruiseSpeedModeBits,
	StaticAirTemperatureBits,
	RollAngleBits,
	WVMixRatioBits,
	TurbulenceBits,
	PositionMessageNumberBits,
	OperationMessageSequenceNumber,
	OperationMessageDelayFieldBits,
	OfficesNumberBitsV2,
	SigmetNbCharBits,
	SigmetReportBitsV2,
	DeclinationIntBits,
	DeclinationDecBits,
	AlertTitleNumBits,
	AlertDescriptionNumBits,
	AlertTypeIdentifierNumBits,
	AlertTypeIdentifierNumBitsV2,
	FlightAlertKeyNumBits,
	CompanyAreaNameNumBits,
	CompanyAreaDescriptionNumBits,
	CompanyAreaNameNumBitsV2,
	CompanyAreaDescriptionNumBitsV2,
	CompanyAreaHoursBits,
	CompanyAreaMinutesBits,
	CompanyAreaOperationBits,
	CompanyAreaDefinitionTypeNumBits,
	CompanyAreaCircleCenterNumBits,
	CompanyAreaCircleDistanceNumBits,
	FirNameNumBits,
	CompanyAreaPolygonTextNumBits,
	PositionMessageNumberBitsV2,
	PositionMessageSourceNumberBits,
	MaxSizeStringBits,
	CustoMessageTypeNumBits,
	CustoMessageVersionNumBits,
	CustoMessageMaxLengthNumBits,
	PreAlertMetaDataKeyMaxNumBits,
	PreAlertMetaDataValueMaxNumBits,
	MaxStringSizeBits,
	PreAlertInstanceIdBits,
	LightningNoFlashesBits,
	FlightKeyNumBitsV2,
	OperationMessageTypeBits,
	FunctionalitiesListSizeBits,
	CompanyRouteNameSizeBits,
	CompanyRouteFieldSizeBits,
	AirportSuitableBits,
	CompanyAreaAlertMonitoringNumBits,
	OfficesNumberBitsV3,
	OptiFlightMessagesNbBits,
	OptiFlightMsgTypeBits,
	OptiFlightMessageBits,
	FuelBitsV2,
	AcarsMsgTypeBits,
	AcarsMsgStatusBits,
	AcarsMsgNbFlightsBits,
	AcarsMsgIdBits,
	AcarsMsgTelexAddressBits,
	AcarsMsgTextBits,
	AcarsMsgIsReadBits,
	AcarsMsgNbBits,
	NbAcarsMessagesBits,
	UserConfigurationMapKeyMaxBits,
	UserConfigurationMapValueMaxBits,
	CompanyAreaSubTypeNumBits,
	CountryNameNumBits,
	PirepNumBits,
	ImageFileRelPathNumBits,
	ImageFileNameNumBits,
	ImageFileChunkSizeNumBits,
	ImageFileWidthNumBits,
	ImageFileHeightNumBits,
	ImageFileBytesPerLineNumBits,
	ProductBitsV2,
	SurfaceFrontLineSizeBits,
	HazardBitsV2,
	ElevationBits,
	ElevationSizeBits,
	EtopsRadiusBits,
	ValidityPeriodBitsV2,
	SigmetIdentifier,
	NbPredictionsBitsV2,
	NbRGBColorNumBits,
	NbRGBAColorNumBits,
	NotamQCodeTypeBits,
	CompanyAreaCircleCenterNumBitsV2,
	CRCBits,
	MinimumCriteriaBits,
	MinimumCriteriaMaskBits,
	NotamModificationTimeBits,
	DatisStationIdBits,
	DatisAddressBits,
	DatisMsgBits,
	DatisMsgNumberBits,
	DatisHeaderBits,
	DatisMessageTypeBits,
	DatisTextBits,
	DatisReportTimeBits,
	UserDevicesNumBits,
	UserDeviceIdBits,
	UserDeviceStartTimeBits,
	UserDeviceMaxLengthNumBits,
	SERIALIZATION_FIELD_SIZE
}SerializationField;

static int const SERIALIZATION_FIELD_BITSIZE[SERIALIZATION_FIELD_SIZE] = {
	8,		//  0: MessageTypeBits
	16,		//  1: MessageIdBits
	32, 	//  2: MessageSizeBits
	32, 	//  3: PacketLengthBits                    
	32, 	//  4: UserIdBits                           
	16, 	//  5: VersionBits                          
	8,		//  6: CodeBits                             
	6,  	//  7: ProductBits: up to 63 (0..63)             
	12,		//  8: AnalysisTimeBits: up to 4095        
	13,		//  9: ExpirationTimeBits: up to 8191      
	12,		// 10: NextIssuedTimeBits: up to 4095     
	12,		// 11: ValidityTimeBits: up to 4095        
	9,		// 12: ValidityPeriodBits: up to 511       
	7,		// 13: NbPredictionsBits: up to 127  
	8,		// 14: HazardBits: up to 255
	13,		// 15: NbWeatherObjectsBits: up to 8191   
	11,		// 16: NbPolygonPointsBits: up to 2047	 
	10,		// 17: AltitudeBits: up to 1023        		
	10,		// 18: PolygonHeightBits: up to 1023                 		
	2 ,		// 19: SeverityBits: up to 3         		
	1,		// 20: SignalBits: up to 1           		
	7,		// 21: DecimalBits: up to 127         		
	7,		// 22: IntegerLatitudeBits: up to 127      
	8,		// 23: IntegerLongitudeBits: up to 255     
	12,		// 24: DiffLatLonBits: up to 4095        	
	1,		// 25: LayerBits: up to 1           	 
	7,		// 26: SpeedBits: up to 127         	 
	9,		// 27: HeadingBits: up to 511         		
	9,		// 28: FlightLevelBits: up to 511         	
	4,		// 29: ACModelCharBits: up to 15 (0..15)    	 
	4,		// 30: FNCharBits: up to 15 (0..15)    	 
	8,		// 31: CharBits: up to 255         	         		       	 
	2,		// 32: SigmetReportBits: up to 3           
	3,		// 33: SigmetCoverageBits: up to 7         
	2,		// 34: SigmetOutlookBits: up to 3         
	4,		// 35: PirepSubtypeBits: up to 15           
	2,		// 36: PirepNofValuesBits: up to 3         
	8,		// 37: PirepNbCharsCommentBits: up to 255
	9,		// 38: PirepNbCharsPropertiesBits: up to 511
	10,		// 39: PirepValuesBits: up to 1023        	 
	8,  	// 40: LightningIntensityBits: up to 255
	10,		// 41: GroundSpeedBits: up to 1023        	 
	12,		// 42: AOILatLonBits: up to 4095        	 
	9,		// 43: AOIWidthBits: up to 511         		
	7,		// 44: TOIBehindBits: up to 127         		
	8,		// 45: TOIAheadBits: up to 255         		
	5,		// 46: TOITTABits: up to 31               
	3,		// 47: ModeBits: up to 7 (0..7)    	 
	32,		// 48: SimulationTimeBits	                 
	32,		// 49: RequestTimeBits		         
	4,		// 50: ErrorCodeBits: up to 15
	FLIGHT_NUMBER_SIZE * 8,	// 51: FPFlightNumberBits
	32,		// 52: FPDepartureTimeBits	         
	9,		// 53: FPDescLengthBits: up to 512
	7,		// 54: FPNbWPBits: up to 127
	K_WP_NAME_MAX_LENGTH * 8,	// 55: FPWPNameBits
	3,		// 56: FPWPTypeBits: up to 7    
	2,		// 57: FPConstraintBits: up to 3          	
	2,		// 58: UserTypeBits: up to 3  
	USER_NICK_SIZE * 8,		// 59: UserNickBits
	USER_PWD_SIZE * 8,		// 60: UserPwdBits
	32,		// 61: TestSizeBits                        
	10,		// 62: TropoAltitudeBits: up to 1023
	32,		// 63: NWDListSizeBits
	3,		// 64: NWDModeBits: up to 7
	4,		// 65: NWDStatusBits: up to 16
	1,		// 66: NWDOOCBits: 0/1
	32, 	// 67: NWDUpdTimeBits
	32,		// 68: NWDLastTimeBits
	8,		// 69: NWDProductBits
	1,  	// 70: CustoEnableBits
	8,  	// 71: CustoProductListSizeBits: up to 255
	32,		// 72: CustoLogoSizeBits
	AIRPORT_ICAO_SIZE * 8, // 73: AirportBits
	AIRLINE_ICAO_SIZE * 8, // 74: AirlineBits
	6,  	// 75: FlightPlanListMaxSizeBitsV1: up to 63
	3,		// 76: TrendBits: up to 7
	9,		// 77: WindSpeedBits: up to 512
	9,  	// 78: WindDirectionBits: up to 512
	7,  	// 79: WindTemperatureBits: up to 128
	9,  	// 80: WeatherReqFPNbPointsBits: up to 511
	6,		// 81: WeatherReqFPCruiseFlBits,
	1,  	// 82: CustoFeedbackBits
	7,  	// 83: NbAirportsBits: up to 127
	AIRPORT_NAME_SIZE * 8,		// 84: AirportNameBits
	1,		// 85: MetarReportTypeBits
	10,		// 86: MetarTafNbCharBits: up to 1023
	3,  	// 87: AuthTypeBits: up to 7
	7,		// 88: AuthParamLengthBits: up to 128
	1,  	// 89: CustoSimulationBits
	32, 	// 90: CustoExpirationTimeBits
	16, 	// 91: AirportElevationBits
	1,		// 92: AirportIFRBits : up to 1
	5,		// 93: NbRunwaysBits: up to 31
	RUNWAY_IDENT_SIZE * 8, // 94: RunwayIdentifierBits
	14, 	// 95: RunwayLatLonDecBits
	16, 	// 96: RunwayDistanceBits
	14,		// 97: RunwayDisplacementBits	
	12, 	// 98: RunwayStopwayBits
	10, 	// 99: RunwayWidthBits: up to 1023
	12, 	// 100: RunwayBearingBits: up to 4095
	4,  	// 101: RunwayLandingSystemBits: up to 15
	3,		// 102: RunwayNOfApproachBits: up to 7	
	5,		// 103:	RunwayApproachSystemBits: up to 63	
	3,		// 104: AirportModeBits: up to 7
	3,		// 105: AirportLevelBits: up to 7
	K_AC_TYPE_SIZE * 8, // 106: ACTypeBits
	9,  	// 107: FlightPlanListMaxSizeBitsV2: up to 511
	11, 	// 108: JetstreamNbPointsBits: up to 2048
	9,  	// 109: JetstreamLatBits
	10, 	// 110: JetstreamLonBits
	9,  	// 111: JetstreamDirectionBits
	8,  	// 112: JetstreamValueBits
	8,		// 113: EnterpriseIdBits: up to 255
	4,		// 114: NOfLicensesBits: up to 15
	9,  	// 115: NotamNbTextBits: up to 511
	11,  	// 116: NotamNbCharBits: up to 2047
	6,  	// 117: FeedbackNameSizeBits: up to 63
	32,  	// 118: FeedbackSizeBits
	8,		// 119: WindMatrixLatBits
	9,		// 120: WindMatrixLonBits
	8,		// 121: FileBlockBits: up to 255
	8,		// 122: FileTypeBits: up to 255
	8,		// 123: FileNameSizeBits: up to 255 	
	1,		// 124: AirportFavouriteBit: up to 1
	1,		// 125: AirportVisibleBit: up to 1
	8, 		// 126: AppTypeBits: up to 255  
	8, 		// 127: LatitudeWindIndexBits: up to 255 
	9, 		// 128: LongitudeWindIndexBits: up to 512
	K_OFP_STRING_SIZE * 8, // 129:	OFPVersionBits
	13,		// 130: ValidityTimeBitsV2: up to 8191  
	8,		// 131: FPNbWPBitsV2: up to 256
	14,		// 132: DecimalBitsV2: up to 16383      
	14,		// 133: GroundSpeedBitsV2: up to 16383
	8,		// 134: TaxiTimeBits: up to 255
	13,		// 135: AnalysisTimeBitsV2: up to 8191  
	12,		// 136: AirportSuitabilityBits: up to 4095
	1,		// 137: CustoAirlineRegisteredBits
	9,		// 138: NbAirportsBitsV2: up to 511
	2,		// 139: TAOITypeBits: up to 3
	2,		// 140:	FPListTypeBits: up to 3
	7,		// 141: ProviderIdBits: up to 128
	32,		// 142: DataConsumption
	12,		// 143: FlightTimeBits: up to 2048
	11,		// 144: FlightPlansNumberBits: up to 2048
	4,		// 145: OfficesNumberBits: up to 15
	OFFICE_ID_SIZE * 8, // 146:	OfficeIDBits
	2,		// 147: EtopsTypeBits up to 3
	9,		// 148: EtopsTimeBits up to 511
	4,		// 149: AlternateRoutesNbBits up to 16
	3,		// 150: WeatherRequestType up to 7
	4,		// 151: WeatherNbFlightLevels up to 16
	6,		// 152: OSTNumTracks up to 64
	1,		// 153: OSTTrackTypeNumBits
	OST_TRACK_ID_SIZE * 8, // 154: OSTTrackId
	10,		// 155: NbAirportsBitsV3 (up to 1024 for dispatch)
	5,		// 156: OffsetTime (up to 32)
	4,		// 157: AirportModeBitsV2: up to 15        
	13,		// 158: FuelBits up to 8191
	POS_MESSAGE_TYPE_SIZE * 8, // 159: PosMessageTypeBits
	4,		// 160: DuplicateNumberBits up to 16
	12,		// 161: MachNumberBits up to 4095
	CRUISE_SPEED_MODE_SIZE * 8, // 162: CruiseSpeedModeBits
	STATIC_AIR_TEMP_SIZE * 8,   // 163: StaticAirTemperatureBits
	8,		// 164: RollAngleBits
	WV_MIX_RATIO_SIZE * 8,      // 165: WVMixRatioBits
	TURBULENCE_SIZE * 8,        // 166: TurbulenceBits
	9,		// 167: PositionMessageNumberBits up to 512
	15,		// 168: OperationMessageSequenceNumber
	12 * 8,   // 169: OperationMessageDelayFieldBits
	5,		// 170: OfficesNumberBitsV2: up to 32
	11,		// 171: SigmetNbCharBits: up to 2048
	3,		// 172: SigmetReportBitsV2: up to 7   
	7,		// 173: DeclinationIntBits up to 128
	4,		// 174: DeclinationIntDec up to 16
	32 * 8,	// 175: AlertTitleNumBits
	512 * 8,	// 176: AlertDescriptionNumBits: up to 32 characters
	16 * 8,	// 177: AlertTypeIdentifierNumBits
	32 * 8,	// 178: AlertTypeIdentifierNumBitsV2
	(FLIGHT_NUMBER_SIZE + AIRPORT_ICAO_SIZE + 10) * 8, // 179: FlightAlertKeyNumBits: flight_number:departure_airport:departure_date (YYYYMMDD)
	COMPANY_AREA_NAME_SIZE_OLD * 8,					 // 180: CompanyAreaNameNumBits
	COMPANY_AREA_DESCRIPTION_SIZE_OLD * 8,			 // 181: CompanyAreaDescriptionNumBits
	COMPANY_AREA_NAME_SIZE * 8,					 // 182: CompanyAreaNameNumBitsV2
	COMPANY_AREA_DESCRIPTION_SIZE_V2 * 8,			 // 183: CompanyAreaDescriptionNumBitsV2
	5,		// 184: CompanyAreaHoursBits: up to 31
	6,		// 185: CompanyAreaMinutesBits: up to 63
	2,		// 186: CompanyAreaOperationBits: up to 3
	3,      // 187: CompanyAreaDefinitionTypeNumBits: up to 7
	MAX_COMPANY_AREA_COORD_SIZE * 8,		// 188: CompanyAreaCircleCenterNumBits
	12,     // 189: CompanyAreaCircleDistanceNumBits up to 4096
	FIR_NAME_SIZE * 8,		// 190: FirNameNumBits
	MAX_COMPANY_AREA_POLYGON_SIZE * 8	,	// 191: CompanyAreaPolygonTextNumBits
	12,		// 192: PositionMessageNumberBitsV2 up to 4096
	4,		// 193: PositionMessageSourceNumberBits up to 16
	12,		// 194: MaxSizeStringBits up to 4096
	4,      // 195: CustoMessageTypeNumBits up to 15
	3,      // 196: CustoMessageVersionNumbBits up to 7
	9,		// 197: CustoMessageMaxLengthNumBits up to 512
	32 * 8, // 198: PreAlertMetaDataKeyMaxNumBits
	64 * 8, // 199: PreAlertMetaDataValueMaxNumBits
	SERIALIZE_STRING_MAX_SIZE * 8, //200: MaxStringSizeBits
	32,		// 201: PreAlertInstanceIdBits
	12,		// 202: LightningNoFlashesBits up to 4096
	(FLIGHT_NUMBER_SIZE + 1 + AIRPORT_ICAO_SIZE + 1 + 8 + 1 + 2) * 8,		// 203: FlightKeyNumBitsV2 flight_number:departure_airport:departure_date(YYYYMMDD):leg_number(XX)
	OPS_TYPE_SIZE * 8,	// 204: OperationMessageTypeBits
	8,		// 205: FunctionalitiesListSizeBits up to 256
	COMPANY_ROUTE_NAME_SIZE * 8, // 206: CompanyRouteNameSizeBits
	COMPANY_ROUTE_FIELD_SIZE * 8, // 207: CompanyRouteFieldSizeBits
	1,		// 208: AirportSuitableBits
	2,		// 209: CompanyAreaAlertMonitoringNumBits up to 4,
	6,		// 210: OfficesNumberBitsV3: up to 64
	5,		// 211: OptiFlightMessagesNbBits: up to 32
	2,		// 212: OptiFlightMsgTypeBits: up to 4
	OPTIFLIGHT_MSG_SIZE * 8, // 213: 	OptiFlightMessageBits
	18,		// 214: FuelBitsV2 up to 262.143
	3,		// 215: AcarsMsgTypeBits up to 8
	3,		// 216: AcarsMsgStatusBits up to 8
	8,		// 217: AcarsMsgNbFlightsBits up to 256
	10,		// 218: AcarsMsgIdBits up to 1024
	TELEX_ADDRESS_SIZE * 8,	// 219: AcarsMsgTelexAddressBits
	ACARS_MAX_MSG_SIZE * 8,		// 220: AcarsMsgTextBits
	1,		// 221: AcarsMsgIsReadBits
	12,		// 222: AcarsMsgNbBits up to 4096
	10,		// 223: NbAcarsMessagesBits up to 1024
	32 * 8,  // 224: UserConfigurationMapKeyMaxBits up to 32 chars for user configuration key field
	512 * 8, // 225: UserConfigurationMapValueMaxBits up to 512 chars for acars text messages signature max length
	3,		 // 226: CompanyAreaSubTypeNumBits up to 7
	COUNTRY_NAME_SIZE * 8,		// 227: CountryNameNumBits	5,		// 228: PirepNumBits up to 32
	5,		// 228: PirepNumBits up to 32
	128 * 8, // 229: ImageFileRelPathNumBits
	128 * 8, // 230: ImageFileNameNumBits up to 32 chars
	1024 * 8,// 231: ImageFileChunkSizeNumBits
	12,		// 232: ImageFileWidthNumBits,
	12,		// 233: ImageFileHeightNumBits,
	14,		// 234: ImageFileBytesPerLineNumBits,
	8,		// 235: Max Number of product id (newer versions need more than 6 bits...)
	11,		// 236: SurfaceFrontLineSizeBits up to 2047
	10,		// 237: HazardBitsV2 up to 1023
	14,		// 238: ElevationBits
	12,		// 239: ElevationSizeBits
	12,		// 240: EtopsRadiusBits up to 4096
	12,		// 241: ValidityPeriodBitsV2 up to 4096
	64,		// 242: SigmetIdentifier
	10,		// 243: NbPredictionsBitsV2: up to 1024
	24,		// 244: NbRGBColorNumBits
	32,		// 245: NbRGBAColorNumBits
	3,		// 246: NotamQCodeTypeBits: up to 7
	MAX_COMPANY_AREA_COORD_SIZE_V2 * 8,		// 247: CompanyAreaCircleCenterNumBitsV2
	16,		// 248: CRCBits
	14,		// 249: MinimumCriteriaBits: up to 16384 (9999 actually used)
	4,		// 250: MinimumCriteriaMaskBits: 4 bits
	32,	    // 251: NotamModificationTimeBits
	AIRPORT_ICAO_SIZE * 8,	// 252: DatisStationIdBits
	DATIS_ADDRESS_SIZE * 8,	// 253: DatisAddressBits
	DATIS_MSG_SIZE * 8,		// 254: DatisMsgBits
	11,		// 255: DatisMsgNumberBits: up to 2048
	DATIS_HEADER_SIZE * 8,	// 256: DatisHeaderBits
	3,		//257: DatisMessageTypeBits
	DATIS_TEXT_SIZE * 8,		// 258: DatisTextBits
	32,	    // 259: DatisReportTimeBits
	10,		// 260: UserDevicesNumBits: up to 64
	24,		// 261: UserDeviceIdBits: up to 16777216
	32,		// 262: UserDeviceStartTimeBits
	7		// 263: UserDeviceMaxLengthNumBits
};

#define SERIALIZATION_FIELD_MAXSIZE(x)			((0x01<<SERIALIZATION_FIELD_BITSIZE[x]) - 1)
#define SERIALIZATION_FIELD_BYTES(x)			(SERIALIZATION_FIELD_BITSIZE[x]/8)
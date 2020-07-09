/*!
 * \file FMSStructures.h
 *
 * \brief Input/output interface for the FMS V3.
 * Structures used for the communication.
 *
 */

#ifndef FMS_STRUCTURES_H
#define FMS_STRUCTURES_H

/* CAUTION : - A structure must have a size multiple of 8 bytes, padding is explicitly defined by a structure member */
/*           - A string shall terminate by '\0'                                                                      */


/* CONSTANTS */


/* T_SSI_Waypoint.type constants */
#define K_WP_TYPE_UNDEFINED                      0
#define K_WP_TYPE_USER                           1
#define K_WP_TYPE_DATABASE                       2
#define K_WP_TYPE_AIRPORT                        3
#define K_WP_TYPE_MISSION                        4
#define K_WP_TYPE_AIRCRAFT                       5

/* T_SSI_PseudoWayPoint.type constants */
#define K_PWP_TYPE_UNDEFINED                     0
#define K_PWP_TYPE_TOP_OF_CLIMB                  1
#define K_PWP_TYPE_TOP_OF_DESCENT                2
#define K_PWP_TYPE_START_OF_CLIMB                3
#define K_PWP_TYPE_END_OF_CLIMB                  4
#define K_PWP_TYPE_START_OF_DESCENT              5
#define K_PWP_TYPE_END_OF_DESCENT                6
#define K_PWP_TYPE_START_OF_ACCELERATION         7
#define K_PWP_TYPE_END_OF_ACCELERATION           8
#define K_PWP_TYPE_START_OF_DECELERATION         9
#define K_PWP_TYPE_END_OF_DECELERATION           10
#define K_PWP_TYPE_END_OF_ECO_TAKE_OFF           11
#define K_PWP_TYPE_THRUST_REDUCTION              12
#define K_PWP_TYPE_AIRSPEED_CAS                  13
#define K_PWP_TYPE_AIRSPEED_MACH                 14
#define K_PWP_TYPE_ECO_THRUST_REDUCTION          15
#define K_PWP_TYPE_ECO_START_OF_ACCELERATION     16

/* T_SSI_Waypoint.altitudeConstraintType constants */
#define K_WP_CONSTRAINT_ALT_UNDEFINED            0
#define K_WP_CONSTRAINT_ALT_BELOW                1
#define K_WP_CONSTRAINT_ALT_EQUAL                2
#define K_WP_CONSTRAINT_ALT_ABOVE                3

/* T_SSI_Waypoint.speedConstraintType constants */
#define K_WP_CONSTRAINT_SPD_UNDEFINED            0
#define K_WP_CONSTRAINT_SPD_BELOW                1
#define K_WP_CONSTRAINT_SPD_EQUAL                2

/* T_SSI_TrajectoryElement.type constants */
#define K_TRAJ_STRAIGHT                          0
#define K_TRAJ_BEND                              1

/* Arrays sizes */
#define K_WP_NAME_MAX_LENGTH                     8
#define K_MAX_WAYPOINTS                          200
#define K_MAX_PSEUDO_WAYPOINTS                   200

#define K_AC_TYPE_SIZE							8
#define K_OFP_STRING_SIZE						12

/* WAYPOINT */
typedef struct
{
    char        name[K_WP_NAME_MAX_LENGTH];		// Name of the flight plan point (icao code if airport)
    short       altitudeConstraintType;         // Altitude constraint applied to the flight plan point
    short       speedConstraintType;            // Speed constraint applied to the flight plan point
    short       type;                           // Type of the flight plan point
    float       altitudeConstraint_feet;        // Passing altitude of the flight plan point (Relevant if an altitude constraint is defined) (Unit : feet)
    float       speedConstraint_kt;             // Passing speed of the flight plan point (Relevant if a speed constraint is defined) (Unit : knot)
    double      latitude_deg;                   // Latitude of the flight plan point (Unit : degree)
    double      longitude_deg;                  // Longitude of the flight plan point (Unit : degree)
	// Extra infomation
	int         fuelOnBoard;                    // Fuel On Board (Unit: kg)
    short		isEditable;						// Editable Waypoint
}T_SSI_WayPoint;

/* LEG */
typedef struct {
    long        padding;                    // Padding to be aligned
    float       trackAngle_deg;             // Track angle of the leg (Unit : degree)
    float       distance_nm;                // Distance between the start point and the end point of the leg (Unit : nautic mile)
    float       flightPathAngle_deg;        // Flight path angle of the leg (Unit : degree)
} T_SSI_Leg;


/* TRAJECTORY ELEMENT */
typedef struct {
    short       type;                       // FMS_TRAJ_STRAIGHT if the element is a straight line, FMS_TRAJ_BEND if the element is a bend
    short       padding;                    // Padding to be aligned
    float       bendRadius_nm;              // Radius of the bend if any, 0 otherwise (Relevant if the element is a bend) (Unit : nautic mile)
    double      startLatitude_deg;          // Latitude of the start point of the trajectory element (Unit : degree)
    double      startLongitude_deg;         // Longitude of the start point of the trajectory element (Unit : degree)
    double      endLatitude_deg;            // Latitude of the end point of the trajectory element (Unit : degree)
    double      endLongitude_deg;           // Longitude of the end point of the trajectory element (Unit : degree)
    double      bendCenterLatitude_deg;     // Latitude of the center of the bend if any, 0 otherwise (Relevant if the element is a bend) (Unit : degree)
    double      bendCenterLongitude_deg;    // Longitude of the center of the bend if any, 0 otherwise (Relevant if the element is a bend) (Unit : degree)

} T_SSI_TrajectoryElement;


/* TRAJECTORY ELEMENT (Enhanced version) */
typedef struct {
    short       type;                       // FMS_TRAJ_STRAIGHT if the element is a straight line, FMS_TRAJ_BEND if the element is a bend
    short       padding;                    // Padding to be aligned
    float       bendRadius_nm;              // Radius of the bend if any, 0 otherwise (Relevant if the element is a bend) (Unit : nautic mile)
    double      startLatitude_deg;          // Latitude of the start point of the trajectory element (Unit : degree)
    double      startLongitude_deg;         // Longitude of the start point of the trajectory element (Unit : degree)
    double      startAltitude_feet;         // Altitude of the start point of the trajectory element (Unit : feet)
    double      endLatitude_deg;            // Latitude of the end point of the trajectory element (Unit : degree)
    double      endLongitude_deg;           // Longitude of the end point of the trajectory element (Unit : degree)
    double      endAltitude_feet;           // Altitude of the start point of the trajectory element (Unit : feet)
    double      bendCenterLatitude_deg;     // Latitude of the center of the bend if any, 0 otherwise (Relevant if the element is a bend) (Unit : degree)
    double      bendCenterLongitude_deg;    // Longitude of the center of the bend if any, 0 otherwise (Relevant if the element is a bend) (Unit : degree)
    double      bendCenterAltitude_feet;    // Altitude of the center of the bend if any, 0 otherwise (Relevant if the element is a bend) (Unit : feet)

} T_SSI_TrajectoryElementEnhanced;


/* FLIGHT PLAN IDENTIFIER */
typedef struct {
	short       identifier1;        // First part of the unique identifier of the flight plan (must be different from 0)
    short       identifier2;        // Second part of the unique identifier of the flight plan (only relevant for an alternative flight plan) (if relevant, must be different from 0)
    short       padding[2];         // Padding to be aligned

} T_SSI_FlightPlanIdentifier;


/* FLIGHT PLAN FROM FMS */
typedef struct {
    T_SSI_FlightPlanIdentifier   identifier;                            // Unique identifier of the flight plan
    short                        nbWayPoints;                           // Number of points contained in the flight plan
    short                        nbTrajectoryElements;                  // Number of elements of the trajectory generated from the flight plan
    short                        reservedData;                          // Reserved data (Index of the flight plans set for the HMI)
    short                        padding[3];                            // Padding to be aligned
    float                        initialWeight_kg;                      // Total weight before departure (Not used for now) (Unit : kilogram)
    float                        initialFuel_kg;                        // Total fuel weight before departure (Unit : kilogram)
    float                        fuelConsumptionRate_kg_h;              // Fuel consumption rate (Unit : kilogram per hour)
    float                        cruiseSpeed_kt;                        // Cruise speed (Unit : knot)
    float                        cruiseAltitude_feet;                   // Cruise altitude (Unit : feet)
    T_SSI_WayPoint               wayPoints[K_MAX_WAYPOINTS];			// Set of the ordered points of the flight plan. The number of points is given by nbWayPoints
    T_SSI_Leg                    legs[K_MAX_WAYPOINTS - 1];				// Set of the legs contained in the flight plan
    T_SSI_TrajectoryElement      trajectory[2 * K_MAX_WAYPOINTS - 3];	// Set of elements of the 2-dimensional trajectory generated from the flight plan

} T_SSI_FMSFlightPlan;


/* FLIGHT PLAN FROM FMS (Enhanced version) */
typedef struct {
    T_SSI_FlightPlanIdentifier       identifier;                             // Unique identifier of the flight plan
    short                            nbWayPoints;                            // Number of points contained in the flight plan
    short                            nbTrajectoryElements;                   // Number of elements of the trajectory generated from the flight plan
    short                            reservedData;                           // Reserved data (Index of the flight plans set for the HMI)
    short                            padding[3];                             // Padding to be aligned
    float                            initialWeight_kg;                       // Total weight before departure (Not used for now) (Unit : kilogram)
    float                            initialFuel_kg;                         // Total fuel weight before departure (Unit : kilogram)
    float                            fuelConsumptionRate_kg_h;               // Fuel consumption rate (Unit : kilogram per hour)
    float                            cruiseSpeed_kt;                         // Cruise speed (Unit : knot)
    float                            cruiseAltitude_feet;                    // Cruise altitude (Unit : feet)
    T_SSI_WayPoint                   wayPoints[K_MAX_WAYPOINTS];          // Set of the ordered points of the flight plan. The number of points is given by nbWayPoints
    T_SSI_Leg                        legs[K_MAX_WAYPOINTS - 1];           // Set of the legs contained in the flight plan
    T_SSI_TrajectoryElementEnhanced  trajectory[2 * K_MAX_WAYPOINTS];     // Set of elements of the 3-dimensional trajectory generated from the flight plan

} T_SSI_FMSFlightPlanEnhanced;


/* FLIGHT PLAN TO FMS */
typedef struct
{
    T_SSI_FlightPlanIdentifier   identifier;                     // Unique identifier of the flight plan
    short                        nbWayPoints;                    // Number of points contained in the flight plan
    short                        reservedData;                   // Reserved data (Index of the flight plans set for the HMI)
    float                        initialWeight_kg;               // Total weight before departure (Not used for now) (Unit : kilogram)
    float                        initialFuel_kg;                 // Total fuel weight before departure (Unit : kilogram)
    float                        fuelConsumptionRate_kg_h;       // Fuel consumption rate (Unit : kilogram per hour)
    float                        cruiseSpeed_kt;                 // Cruise speed (Unit : knot)
    float                        cruiseAltitude_feet;            // Requested cruise altitude. (If it is not reachable, it can be different from the altitude of the 'top of' points) (Unit : feet)
    T_SSI_WayPoint               wayPoints[K_MAX_WAYPOINTS];  // Set of the ordered points of the flight plan. The number of waypoints is given by nbWayPoints
}T_SSI_FlightPlan;

/* track */
typedef struct {
    short                        nbWayPoints;                    // Number of points contained in the flight plan
    T_SSI_WayPoint               wayPoints[K_MAX_WAYPOINTS];  // Set of the ordered points of the flight plan. The number of waypoints is given by nbWayPoints
} T_SSI_Track;


/* WAYPOINT PREDICTIONS */
typedef struct {
    long        padding;                    // Padding to be aligned
    long        time_ms;                    // Required time to reach the flight plan point. Relative to the current time (< 0 if the point is passed, > 0 otherwise) (Unit : millisecond)
    float       fuel_kg;                    // Consumed fuel at flight plan point (Unit : kilogram)
    float       altitude_feet;              // Passing altitude of the flight plan point (Unit : feet)
    float       speed_kt;                   // Passing speed of the flight plan point (Unit : knot)
    float       distanceFromOrigin_nm;      // Distance of the flight plan point from the start of the flight plan (Unit : nautic mile)

} T_SSI_WayPointPredictions;


/* PSEUDO WAYPOINT */
typedef struct {
    short                           type;                           // Type of the pseudo flight plan point
    short                           padding[3];                     // Padding to be aligned
    char                            name[K_WP_NAME_MAX_LENGTH];  // name of the pseudo flight plan point.
    double                          latitude_deg;                   // Latitude of the pseudo flight plan point (Unit : degree)
    double                          longitude_deg;                  // Longitude of the pseudo flight plan point (Unit : degree)
    double                          targetSpeed_kt;                 // Speed attribute of the pseudo flight plan point. Only relevant for the pseudo points related to start/end of acceleration or deceleration. (Unit : knot)
    double                          targetAltitude_ft;              // Altitude attribute of the pseudo flight plan point. Only relevant for the pseudo points related to start/end of climb or descent. (Unit : feet)
    T_SSI_WayPointPredictions    predictions;                    // Predictions of the pseudo flight plan point

} T_SSI_PseudoWayPoint;


/* FLIGHT PLAN PREDICTIONS */
typedef struct {
    T_SSI_FlightPlanIdentifier   identifier;                                 // Unique identifier of the flight plan
    short                           nbWayPoints;                                // Number of points contained in the flight plan
    short                           nbPseudoWayPoints;                          // Number of pseudo points related to the flight plan
    short                           padding[2];                                 // Padding to be aligned
    T_SSI_WayPointPredictions    wayPointsPredictions[K_MAX_WAYPOINTS];   // Predictions of the set of waypoints contained in the flight plan
    T_SSI_PseudoWayPoint         pseudoWayPoints[K_MAX_PSEUDO_WAYPOINTS]; // Predictions of the set of pseudo points related to the flight plan

} T_SSI_FlightPlanPredictions;

/**
* @struct T_SSI_ProposedAlternativeFP
* @brief Struct to be sent for alternative routes ATC proposals and revisions
* @var T_SSI_ProposedAlternativeFP::flightPlan
* Member flightPlan The flightplan to be proposed to ATC
* @var T_SSI_ProposedAlternativeFP::status
* Member status Defines the status of the alternative route within the ATC proposal
*/
typedef struct
{
	T_SSI_FlightPlan	 flightPlan;
	int                  status;
	int					 alternativeRouteIndex;
} T_SSI_ProposedAlternativeFP; // 0=NOT_PROPOSED 1=PROPOSED 2=ACCEPTED 3=REJECTED

#endif  // FMS_STRUCTURES_H

#ifndef NGC_H
#define NGC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Constants
#define PI 3.14159265358979323846
#define MAX_POINTS 1000
#define MAX_FILENAME 256

// Data structures
typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    double throat_radius;        // Throat radius (m)
    double exit_radius;          // Exit radius (m)
    double throat_x;             // Throat x-position (m)
    double exit_x;               // Exit x-position (m)
    double expansion_ratio;      // Area expansion ratio
    double bell_angle;           // Bell angle (radians)
    int num_points;              // Number of points in geometry
    Point geometry[MAX_POINTS];  // Nozzle geometry points
} NozzleGeometry;

typedef struct {
    double chamber_pressure;     // Chamber pressure (Pa)
    double ambient_pressure;     // Ambient pressure (Pa)
    double chamber_temperature;  // Chamber temperature (K)
    double molecular_weight;     // Molecular weight (kg/mol)
    double gamma;                // Specific heat ratio
    double gas_constant;         // Universal gas constant (J/mol-K)
} FlowConditions;

typedef struct {
    double thrust;               // Thrust (N)
    double specific_impulse;     // Specific impulse (s)
    double exit_velocity;        // Exit velocity (m/s)
    double exit_pressure;        // Exit pressure (Pa)
    double exit_temperature;     // Exit temperature (K)
    double mass_flow_rate;       // Mass flow rate (kg/s)
    double characteristic_velocity; // Characteristic velocity (m/s)
    double thrust_coefficient;   // Thrust coefficient
} PerformanceResults;

// Function prototypes

// Nozzle geometry functions
int calculate_bell_nozzle_geometry(NozzleGeometry* nozzle, double length_fraction);
int calculate_expansion_ratio(NozzleGeometry* nozzle);
double calculate_nozzle_area(double radius);

// Performance calculation functions
int calculate_performance(const NozzleGeometry* nozzle, const FlowConditions* conditions, PerformanceResults* results);
double calculate_throat_conditions(const FlowConditions* conditions, double* throat_pressure, double* throat_temperature);
double calculate_exit_conditions(const NozzleGeometry* nozzle, const FlowConditions* conditions, 
                                 double* exit_pressure, double* exit_temperature, double* exit_velocity);

// Plotting and output functions
int plot_nozzle_geometry(const NozzleGeometry* nozzle, const char* filename);
int write_geometry_data(const NozzleGeometry* nozzle, const char* filename);
int print_performance_results(const PerformanceResults* results);

// Utility functions
int validate_input_parameters(const NozzleGeometry* nozzle, const FlowConditions* conditions);
void print_usage(const char* program_name);

#endif // NGC_H
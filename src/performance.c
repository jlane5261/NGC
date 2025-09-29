#include "../include/ngc.h"

int calculate_performance(const NozzleGeometry* nozzle, const FlowConditions* conditions, PerformanceResults* results) {
    if (!nozzle || !conditions || !results) {
        return -1;
    }

    // Calculate throat conditions
    double throat_pressure, throat_temperature;
    double throat_area = calculate_nozzle_area(nozzle->throat_radius);
    double exit_area = calculate_nozzle_area(nozzle->exit_radius);
    
    calculate_throat_conditions(conditions, &throat_pressure, &throat_temperature);

    // Calculate exit conditions
    double exit_pressure, exit_temperature, exit_velocity;
    calculate_exit_conditions(nozzle, conditions, &exit_pressure, &exit_temperature, &exit_velocity);

    // Calculate specific gas constant
    double R_specific = conditions->gas_constant / conditions->molecular_weight;

    // Calculate characteristic velocity
    results->characteristic_velocity = sqrt(conditions->gamma * R_specific * conditions->chamber_temperature) /
                                      sqrt(conditions->gamma * pow((2.0 / (conditions->gamma + 1.0)), 
                                           (conditions->gamma + 1.0) / (conditions->gamma - 1.0)));

    // Calculate mass flow rate
    results->mass_flow_rate = throat_area * throat_pressure / 
                             sqrt(R_specific * throat_temperature);

    // Calculate thrust using momentum equation
    results->thrust = results->mass_flow_rate * exit_velocity + 
                     (exit_pressure - conditions->ambient_pressure) * exit_area;

    // Calculate specific impulse
    results->specific_impulse = results->thrust / (results->mass_flow_rate * 9.81);

    // Store other results
    results->exit_velocity = exit_velocity;
    results->exit_pressure = exit_pressure;
    results->exit_temperature = exit_temperature;

    // Calculate thrust coefficient
    results->thrust_coefficient = results->thrust / (conditions->chamber_pressure * throat_area);

    return 0;
}

double calculate_throat_conditions(const FlowConditions* conditions, double* throat_pressure, double* throat_temperature) {
    // Isentropic relations for choked flow
    double pressure_ratio = pow(2.0 / (conditions->gamma + 1.0), 
                               conditions->gamma / (conditions->gamma - 1.0));
    double temperature_ratio = 2.0 / (conditions->gamma + 1.0);

    *throat_pressure = conditions->chamber_pressure * pressure_ratio;
    *throat_temperature = conditions->chamber_temperature * temperature_ratio;

    return 0;
}

double calculate_exit_conditions(const NozzleGeometry* nozzle, const FlowConditions* conditions, 
                                double* exit_pressure, double* exit_temperature, double* exit_velocity) {
    
    // Use isentropic relations for perfect expansion
    double area_ratio = nozzle->expansion_ratio;
    double gamma = conditions->gamma;
    double R_specific = conditions->gas_constant / conditions->molecular_weight;

    // Solve for exit Mach number using area-Mach relation (approximation)
    double mach_exit = sqrt(2.0 / (gamma - 1.0) * (pow(area_ratio, (gamma - 1.0) / gamma) - 1.0));
    
    // More accurate iterative solution for Mach number
    double mach_guess = mach_exit;
    for (int i = 0; i < 10; i++) {
        double f = pow((gamma + 1.0) / 2.0, (gamma + 1.0) / (2.0 * (gamma - 1.0))) *
                   pow(1.0 + (gamma - 1.0) / 2.0 * mach_guess * mach_guess, -(gamma + 1.0) / (2.0 * (gamma - 1.0))) /
                   mach_guess - 1.0 / area_ratio;
        
        double df = -pow((gamma + 1.0) / 2.0, (gamma + 1.0) / (2.0 * (gamma - 1.0))) *
                    (1.0 / (mach_guess * mach_guess) +
                     (gamma + 1.0) / 2.0 * pow(1.0 + (gamma - 1.0) / 2.0 * mach_guess * mach_guess, -(gamma + 3.0) / (2.0 * (gamma - 1.0))));
        
        mach_guess = mach_guess - f / df;
    }
    mach_exit = mach_guess;

    // Calculate exit conditions
    double temp_ratio = 1.0 / (1.0 + (gamma - 1.0) / 2.0 * mach_exit * mach_exit);
    double press_ratio = pow(temp_ratio, gamma / (gamma - 1.0));

    *exit_temperature = conditions->chamber_temperature * temp_ratio;
    *exit_pressure = conditions->chamber_pressure * press_ratio;
    *exit_velocity = mach_exit * sqrt(gamma * R_specific * (*exit_temperature));

    return 0;
}
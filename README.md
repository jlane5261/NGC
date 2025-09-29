# NGC - Nozzle Geometry Calculator
A comprehensive C-based tool for calculating and plotting rocket engine bell nozzle geometries and theoretical propulsion performance.

## Features

- **Bell Nozzle Geometry Calculation**: Generates accurate bell nozzle contours using parabolic approximation methods
- **Propulsion Performance Analysis**: Calculates thrust, specific impulse, exit velocity, and other key performance metrics
- **Data Export**: Outputs geometry data in various formats for further analysis
- **Plotting Support**: Generates gnuplot scripts for visualizing nozzle geometries
- **Command-Line Interface**: Easy-to-use CLI with comprehensive options
- **Library Support**: Can be used as a library in other C programs

## Building

### Prerequisites
- GCC compiler with C99 support
- Make build system
- Math library (libm)
- Optional: gnuplot for generating plots

### Compilation
```bash
make
```

### Other Build Targets
```bash
make clean      # Clean build artifacts
make debug      # Build with debug symbols
make run        # Build and run with default parameters
make example    # Build and run with example parameters
make help       # Show available targets
```

## Usage

### Command Line Interface

Basic usage:
```bash
./bin/ngc [options]
```

### Options

| Option | Long Option | Description | Default |
|--------|-------------|-------------|---------|
| -h | --help | Show help message | - |
| -t | --throat-radius | Throat radius (m) | 0.01 |
| -e | --exit-radius | Exit radius (m) | 0.03 |
| -p | --chamber-pressure | Chamber pressure (Pa) | 1000000 |
| -a | --ambient-pressure | Ambient pressure (Pa) | 101325 |
| -T | --chamber-temp | Chamber temperature (K) | 3000 |
| -M | --molecular-weight | Molecular weight (kg/mol) | 0.02 |
| -g | --gamma | Specific heat ratio | 1.3 |
| -l | --length-fraction | Nozzle length fraction | 0.8 |
| -o | --output | Output plot filename | nozzle_plot.png |
| -d | --data | Output geometry data filename | - |

### Examples

1. **Basic calculation with default parameters:**
```bash
./bin/ngc
```

2. **Custom nozzle dimensions:**
```bash
./bin/ngc -t 0.005 -e 0.025 -p 2000000
```

3. **Vacuum-optimized nozzle:**
```bash
./bin/ngc -t 0.01 -e 0.05 -a 0 -p 3000000 -T 3500
```

4. **Generate data file:**
```bash
./bin/ngc --throat-radius 0.008 --exit-radius 0.032 --data my_nozzle.dat
```

## Theory

### Bell Nozzle Geometry

The tool calculates bell nozzle contours using a parabolic approximation method that provides:
- Smooth transition from throat to exit
- Optimized length for given expansion ratio
- Reduced divergence losses compared to conical nozzles

### Performance Calculations

The performance analysis uses:
- Isentropic flow relations for compressible flow
- Choked flow conditions at the throat
- Perfect gas assumptions
- Momentum and continuity equations

### Key Performance Metrics

- **Thrust (N)**: F = ṁv_e + (p_e - p_a)A_e
- **Specific Impulse (s)**: I_sp = F/(ṁg₀)
- **Characteristic Velocity (m/s)**: c* = √(γRT_c)/√(γ((2/(γ+1))^((γ+1)/(γ-1))))
- **Thrust Coefficient**: C_F = F/(p_c A_t)

## Library Usage

You can use NGC as a library in your C programs:

```c
#include "include/ngc.h"

int main() {
    NozzleGeometry nozzle = {0};
    FlowConditions conditions = {0};
    PerformanceResults results = {0};

    // Set parameters
    nozzle.throat_radius = 0.01;
    nozzle.exit_radius = 0.03;
    conditions.chamber_pressure = 1e6;
    conditions.chamber_temperature = 3000;
    // ... set other parameters

    // Calculate geometry
    calculate_bell_nozzle_geometry(&nozzle, 0.8);
    
    // Calculate performance
    calculate_performance(&nozzle, &conditions, &results);
    
    // Print results
    print_performance_results(&results);
    
    return 0;
}
```

Compile with:
```bash
gcc -Iinclude your_program.c src/*.c -lm -o your_program
```

## Output Files

The tool generates several output files:

1. **nozzle_upper.dat / nozzle_lower.dat**: Geometry data for upper and lower nozzle contours
2. **plot_nozzle.gp**: Gnuplot script for plotting the nozzle geometry
3. **Custom data files**: When using --data option

## Installation

### System-wide Installation
```bash
make install
```
This installs the `ngc` executable to `/usr/local/bin/` (requires sudo).

### Uninstallation
```bash
make uninstall
```

## Validation

The tool validates all input parameters to ensure physical validity:
- Positive throat and exit radii
- Exit radius greater than throat radius
- Positive pressures and temperatures
- Valid specific heat ratio (γ > 1.0)
- Positive molecular weight and gas constant

## Limitations

- Assumes perfect gas behavior
- Uses simplified bell nozzle approximation
- Does not account for viscous effects
- Assumes equilibrium flow conditions
- Does not consider heat transfer effects

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is open source. See the repository for license details.

## References

- Sutton, G. P., & Biblarz, O. (2016). Rocket Propulsion Elements
- Hill, P. G., & Peterson, C. R. (1992). Mechanics and Thermodynamics of Propulsion
- Turner, M. J. L. (2009). Rocket and Spacecraft Propulsion

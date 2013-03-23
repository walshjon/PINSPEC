import numpy
from pinspec import *
import pinspec.SLBW as SLBW
import pinspec.plotter as plotter
import pinspec.process as process
from pinspec.log import *

def main():

    # Set main simulation params
    num_batches = 20
    num_neutrons_per_batch = 10000
    num_threads = 12
    radius_fuel = 0.4096;
    pitch = 1.26
    dancoff = 0.277;
    output_dir = 'Equivalence'

    log_setlevel(INFO)

    # Call SLBW to create XS
    # Call SLBW to create XS	
    #log.py_printf('INFO', 'Creating SLBW xs')
    #T=300 #Temp in Kelvin of target nucleus
    #SLBW.replaceXS() #To clean previous changes to XS files
    #SLBW.SLBWXS('U-238',T,'capture') #To generate Doppler Broadened Res Cap
    #SLBW.SLBWXS('U-238',T,'scatter') #To generate Doppler Broadened Res Scat
    #SLBW.generatePotentialScattering('U-238') #To generate flat Res Scat XS
    #SLBW.compareXS('U-238', XStype='scatter', RI='no')
    #SLBW.compareXS('U-238', XStype='capture', RI='no')   

    # Define isotopes
    h1 = Isotope('H-1')
    o16 = Isotope('O-16')
    u235 = Isotope('U-235')
    u238 = Isotope('U-238')
    
    # Define moderator material
    moderator = Material('moderator')
    moderator.setDensity(1.0, 'g/cc')
    moderator.addIsotope(h1, 2.0)
    moderator.addIsotope(o16, 1.0)

    py_printf('INFO', 'Added isotopes to moderator')

    # Define fuel material
    fuel = Material('fuel')
    fuel.setDensity(10.0, 'g/cc')
    fuel.addIsotope(u235, 0.03)
    fuel.addIsotope(u238, 0.97)
    fuel.addIsotope(o16, 2.0)
    
    py_printf('INFO', 'Added isotopes to fuel')
    
    # Define moderator region
    region_mod = Region('moderator', MODERATOR)
    region_mod.setMaterial(moderator)
    region_mod.setFuelRadius(0.4096)
    region_mod.setPitch(1.26)
    
    py_printf('INFO', 'Made moderator region')
    
    # Define fuel region
    region_fuel = Region('fuel', FUEL)
    region_fuel.setMaterial(fuel)
    region_fuel.setFuelRadius(0.4096)
    region_fuel.setPitch(1.26)

    py_printf('INFO', 'Made fuel region')
    
    # Define geometry
    geometry = Geometry()
    geometry.setSpatialType(HOMOGENEOUS_EQUIVALENCE)
    geometry.addRegion(region_mod)
    geometry.addRegion(region_fuel)
    geometry.setNumBatches(num_batches)
    geometry.setNeutronsPerBatch(num_neutrons_per_batch)
    geometry.setNumThreads(num_threads)
    geometry.setDancoffFactor(dancoff)

    # Create Tallies for the fluxes
    total_flux = TallyFactory.createTally('total flux', geometry, FLUX)
    moderator_flux = TallyFactory.createTally('moderator flux', region_mod, FLUX)
    fuel_flux = TallyFactory.createTally('fuel flux', region_fuel, FLUX)
    total_flux.generateBinEdges(1E-2, 1E7, 2000, LOGARITHMIC)
    moderator_flux.generateBinEdges(1E-2, 1E7, 2000, LOGARITHMIC)
    fuel_flux.generateBinEdges(1E-2, 1E7, 2000, LOGARITHMIC)

	# Register tallies
    TallyBank.registerTally(total_flux, geometry)
    TallyBank.registerTally(moderator_flux, region_mod)
    TallyBank.registerTally(fuel_flux, region_fuel)

	# Run Monte Carlo simulation
    geometry.runMonteCarloSimulation();

	# Dump batch statistics to output files to some new directory
    TallyBank.outputBatchStatistics(output_dir, 'test')

    # Plotting xs, flux, thermal scattering
    plotter.plotFluxes([total_flux, moderator_flux, fuel_flux], output_dir)
    plotter.plotMicroXS(u235, ['capture', 'elastic', 'fission'], output_dir)
    plotter.plotMicroXS(u238, ['capture', 'elastic', 'fission'], output_dir)
    plotter.plotMicroXS(h1, ['capture', 'elastic', 'absorption'], output_dir)
    plotter.plotMicroXS(o16, ['capture', 'elastic', 'absorption'], output_dir)
    plotter.plotMacroXS(fuel, ['capture', 'elastic', 'fission', \
                                            'absorption', 'total'], output_dir)
    plotter.plotMacroXS(moderator, ['capture', 'elastic', 'fission', \
                                            'absorption', 'total'], output_dir)
    plotter.plotThermalScattering(h1, output_dir)
    plotter.plotThermalScattering(u238, output_dir)
    plotter.plotThermalScattering(u235, output_dir)
    plotter.plotThermalScattering(o16, output_dir)


if __name__ == '__main__':
    
    main()


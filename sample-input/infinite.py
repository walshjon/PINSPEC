import numpy
from pinspec import *
import pinspec.SLBW as SLBW
import pinspec.plotter as plotter
import pinspec.process as process
from pinspec.log import *

def main():
    
    setOutputDirectory('infinite')
    py_setlevel('INFO')
    
    py_printf('TITLE', 'Simulating an infinite medium homogenized pin-cell')
    
    py_printf('NORMAL', 'Initializing isotopes...')
    
    # Define isotopes
    h1 = Isotope('H-1')
    b10 = Isotope('B-10')
    o16 = Isotope('O-16')
    u235 = Isotope('U-235')
    u238 = Isotope('U-238')
    zr90 = Isotope('Zr-90')
    
    py_printf('NORMAL', 'Initializing fuel-moderator mix material...')
    
    # Define materials
    mix = Material('Fuel Moderator Mix')
    mix.setDensity(5., 'g/cc')
    mix.addIsotope(b10, .0000001)
    mix.addIsotope(o16, 1.0)
    mix.addIsotope(h1, 1.0)
    mix.addIsotope(u238, 0.01)
    mix.addIsotope(u235, .0025)
    mix.addIsotope(zr90, .16)
    
    py_printf('NORMAL', 'Initializing fuel-moderator mix region...')
    
    # Define region
    region_mix = Region('infinite medium', INFINITE)
    region_mix.setMaterial(mix)
    
    py_printf('NORMAL', 'Initializing the geometry...')
    
    # Define geometry
    geometry = Geometry(INFINITE_HOMOGENEOUS)
    geometry.addRegion(region_mix)
    
    py_printf('NORMAL', 'Initializing flux tally...')
    
    # Create a tally for the flux
    flux = createTally(region_mix, FLUX)
    flux.generateBinEdges(1E-2, 1E7, 10000, LOGARITHMIC)
    TallyBank.registerTally(flux)
    
    # Run Monte Carlo simulation
    geometry.runMonteCarloSimulation()
    
    py_printf('INFO', 'Plotting flux...')
    
    # Plot the flux
    plotter.plotFlux(flux, uselegend=False, filename='flux', \
                     title='Infinite medium flux')
    
    py_printf('INFO', 'Writing tally batch statistics to output file...')
    
    # Dump batch statistics to output files to some new directory
    TallyBank.outputBatchStatistics()
    
    py_printf('TITLE', 'Finished')


if __name__ == '__main__':
    
    main()  


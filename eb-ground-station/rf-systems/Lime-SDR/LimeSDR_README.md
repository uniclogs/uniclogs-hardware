# LimeSDR Setup

To get a LimeSDR working on a linux-based system, there are several different components that need to be installed depending on what functionality is wanted.

This document focuses on getting LimeSDR working with gnuradio based setups.

## The Components
### LimeSuite
The first component needed for interfacing with a LimeSDR is LimeSuite, provided by MyriadRF. This component includes all drivers needed to interface with the board, and provides the `LimeUtil` and `LimeSuiteGUI` tools. `LimeUtil` allows command line programming and updating of the LimeSDR, while `LimeSuiteGUI` is a simple to use frontend for manually manipulating the various registers and parameters of the board. However, `LimeSuiteGUI` is not required for the other components to work with it. It's just a graphical frontend to manually manipulate parameters for testing.

In addition to the tools provided, LimeSuite provides a plugin for the next component, SoapySDR, which allows it to interface with the LimeSDR through the drivers.

### SoapySDR
This component is a generalized SDR interface library, usable with many different SDRs. Interfaces are provided to the various SDRs through plugins for SoapySDR. LimeSuite provides the plugin for LimeSDR, allowing SoapySDR compatible things to interface with the LimeSDR.

### GrOsmoSDR
gnuradio and PothosSDR use osmocom's source and sink blocks. GrOsmoSDR provides gnuradio with the interface to SoapySDR.

## Putting it together
### Arch Linux
On Arch Linux, I had to install `soapysdr-git` `limesuite-git` and `gr-osmosdr-git` in order to get the full data path working. I also installed `gqrx-git` in order to test the LimeSDR with GQRX.

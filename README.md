# oresat-ground-station
OreSat ground station hardware and software for both RF and antenna positioning


## OreSat Ground Station Infrastructure

OreSat will utilize two ground stations for this project, a primary station, and a fully-capable secondary station. The primary station is being constructed by the students involved in the project and will be located on the Portland State University Engineering Building. The secondary station is an existing amateur satellite ground station located at a different location for spatial diversity. It can be used to help verify and check out the primary station as construction progresses, and serve as a backup to the primary station. The two stations are identical.

## RF Links

The ground station infrastructure provides two communications functions between the ground and the spacecraft, (1) up and down communications on UHF for the Telemetry and Command (T&C) subsystem onboard the spacecraft, and (2) downlink communications on S-band for mission payload and camera data.

The UHF component is centered in the 435 to 438 MHz amateur spacecraft band and utilizes both a RHCP and LHCP directional helix antennas mounted on an azimuth/elevation antenna positioner. Coax switches connect one of the antennas to either a Low Noise Amplifier (LNA) for receiving, or a Power Amplifier (PA) for transmitting. The LNA and PA are then connected to a Software Defined Radio (SDR) that provides both UHF RF input and output signals.

The S-Band component is centered on the 2.4 to 2.43 GHz WiFi channel in the ISM / amateur service spectrum. A 0.5 meter parabolic reflector with a RHCP/LHCP feed is mounted on the same antenna positioner as the UHF antenna. A coax switch is used to select RHCP or LHCP signal from the feed, is then switched to either an LNA or PA. The LNA and PA are connected to a Software Defined Radio (SDR) that provides both S-Band RF input and output signals.

The SDR used is the LimeSDR from LimeMicro, which utilizes GnuRadio for modulation and demodulation. The emission type is BPSK at 9600 bps, and was chosen because of the capability of the T&C transceiver radio on the spacecraft, an acceptable performance indicated in the link budget, and is somewhat common in the amateur satellite community.

## Antenna positioner

An antenna positioner controller is used to aim the antenna array and is interfaced serially using the EasyComm control protocol accessible through the HamLib control library. The positioner controller is based on the Open Source hardware/software project of SatNOGS, and provides the ability for tracking control locally using GPredict, or by the SatNOGS Network software across the Internet.

## GnuRadio 

Data is passed into/out of the ground station radio through GnuRadio ModCod blocks and can provide raw IQ sample files from the payload data. Doppler shift can be compensated for in real time using the tracking software and an interface into GnuRadio.

## Data

The T&C subsystem provides telemetry data to the ground in two forms, a BPSK modulated data frame in response to a command request, and a very short Morse code modulated telemetry sequence and satellite ID if no activity is present for a given period of time. This Morse code ID allows the satellite to be heard by the general amateur radio community and will help with identifying the satellite once on orbit. There are several satellite housekeeping commands that can be sent up from the ground to control power, attitude and payload functions, and most importantly a command to reliably shut off all RF transmissions if required. All commands up to the satellite are encrypted for security.


## Project Team (so far!)

- [Glenn LeBrasseur](https://github.com/glennl)
- [Jamey Sharp](https://github.com/jameysharp)
- [Miles Simpson](https://github.com/heliochronix)
- [Joe Shields](https://github.com/Joedang)
- Mark Musil mmusil@pdx.edu
- [Kenny M](https://github.com/aSmig)



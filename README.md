# University Class Open Ground Station
UniClOGS ground station hardware for both RF and antenna positioning

![EB Station Block Diagram](https://github.com/oresat/oresat-ground-station/blob/master/doc/GS_3_7.pdf)

![EB Station Block Diagram](https://github.com/oresat/oresat-ground-station/blob/master/doc/FO-29_Pass_Screenshot_from_2019-04-28_17-00-28.png)


## OreSat Ground Station Infrastructure

OreSat will utilize two ground stations for this project, a primary station, and a fully-capable secondary station. The primary station is being constructed by the students involved in the project and will be located on the Portland State University Engineering Building. The secondary station is an existing amateur satellite ground station located at a different location for spatial diversity. It can be used to help verify and check out the primary station as construction progresses, and serve as a backup to the primary station. The two stations are identical.

## RF Links

The ground station infrastructure provides two communications functions between the ground and the spacecraft, (1) downlink communications on UHF for the Telemetry Beacons and Command Responses from the spacecraft, (2) uplink communications on L-Band for the Commands to the spacecraft, and (3) downlink communications on S-band for mission payload and camera data.

The UHF component is centered in the 435 to 438 MHz amateur spacecraft band and utilizes a circular polarized directional Yagi antenna with a RHCP/LHCP polarization sense switch. The L-Band component is centered in the 1260 to 1270 MHz amateur spacecraft band designated for uplink only, and utilizes a RHCP circular polarized 2x2 helix array. All of this is mounted on an azimuth/elevation antenna positioner. A coax switch connects the UHF antenna to either a Low Noise Amplifier (LNA) for receiving, or a Power Amplifier (PA) for transmitting. The LNA and PA is then connected to a Software Defined Radio (SDR) that provides both UHF RF input and output signals. The L-Band antenna is directly connected to a PA for transmitting only, and then to the same SDR to provide the L RF signal.

The S-Band component is centered on the 2.4 to 2.43 GHz WiFi channel in the ISM / amateur service spectrum. A 0.5 meter parabolic reflector with a RHCP/LHCP feed is mounted on the same antenna positioner as the UHF antenna. A coax switch is used to select RHCP or LHCP signal from the feed, is then fed to a bidirectional WiFi PA/LNA. The PA/LNA is connected to a ATH9K WiFi adapter that provides both S-Band RF input and output signals.

The SDR used for UHF and L-Band is the LimeSDR from LimeMicro, which utilizes GnuRadio for modulation and demodulation. The emission type is BPSK or GFSK at 9600 bps, and was chosen for the capability of the T&C transceiver radio on the spacecraft, an acceptable performance indicated in the link budget, and is somewhat common in the amateur satellite community.

## Antenna positioner

An antenna positioner controller is used to aim the antenna array and is interfaced serially using the EasyComm control protocol accessible through the HamLib control library. The positioner controller is based on the Open Source hardware/software project by K3NG, and provides the ability for tracking control through HamLib rotctld both locally using GPredict, or by the SatNOGS Client software which provides interconnectivity to the SatNOGS system over the Internet.

## GnuRadio 

Data is passed into/out of the ground station radio through GnuRadio ModCod blocks and can provide raw IQ sample files from the payload data. Doppler shift can be compensated for in real time using the tracking software and an XML/RPC server in GnuRadio.

## Data

The T&C subsystem provides telemetry data to the ground in two forms, a BPSK or GFSK modulated data frame in response to a command request, and a very short Morse code modulated telemetry sequence and satellite ID if no activity is present for a given period of time. This Morse code ID allows the satellite to be heard by the general amateur radio community and will help with identifying the satellite once on orbit. There are several satellite housekeeping commands that can be sent up from the ground to control power, attitude and payload functions, and most importantly a command to reliably shut off all RF transmissions if required. All commands up to the satellite are encrypted for security.



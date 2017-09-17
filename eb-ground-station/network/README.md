## Establish OreSat VLAN (aka "Blue Pipe")

- Essentially the "Blue Pipe" is a 1 Gb/s VLAN between the EB Penthouse rack and EB. This is officially called the OreSat VLAN.

- Also related is the edge router and firewall to get us out to the Internet.

### Provide a "what we want" document to Dave Burns to move forward with getting the connection established.

- Dave expects to need a pair of Single Mode SFP's to increase the existing bandwidth over the fiber between the Penthouse switch and the EB switch on LL. He's planning on moving from the MultiMode fiber to SingleMode fiber, since MM over the fiber they have installed won't support Gb over the required distance. They have both MM and SM available at both FTB's. Estimated $250 each.

- Estimate the bandwidth we need given the devices on the roof only push I/Q samples from the SDR to the server in EB-91, and doing all the signal processing on the server. Expected this will go across UDP.

- Describe the protocols and ports we want to move across the VLAN, and what we want to push across the edge router.

   * We expect only ports 22, 123 (NTP) and 443 across the edge router.
   * Essentially answer his question "What are you wanting to put on 'my' network?"
      * Since this is a VLAN, our traffic would never really appear on his network, however what goes out through the firewall and onto the Internet is what he is really asking about.

NB: Dave expressed keen interest in getting NTP from our time server. Maybe also provide PTP?

[Miles started this document](https://docs.google.com/document/d/1XI038USdZ7fAQvtVEo2skS9kDt9Eve8YVRDrkvMqaz0/edit)


### Follow through in getting the LAN physically connected and working.

- There needs to be a 1 Gb/s switch in the rooftop enclosure to provide LAN to both of the GS devices in our enclosure. The weather station will use it's own physical CAT-6 circuit to the Penthouse Cisco switch, so it does not enter onto any of our concerns. 

- There currently is a connection in the EB penthouse using CAT-6 to the rooftop.

- There will need to be a connection in a comm closet adjacent to EB-91 to some structured wiring connection and wall plate on EB-91. It will need to be labeled **_OreSat VLAN_**.

### Establish the OreSat server assets in EB-91

- We should expect to put all of our devices in a small 19 inch rack somewhere in the EB-91 lab

- We will want another 1 Gb/s switch in that rack for all things that need to connect to OreSat VLAN

- There will also need to be a connection back to "Dave's Network" where we connect our edge router between the two. Here another structured wiring jack on EB-91 will be needed, and should be labeled **_Dave's Network_** or something like that.

- I would recommend a Juniper SRX-100 for use as the edge router / firewall for the following reasons.
   * We only need 100 Mb/s out to the Internet.
   * It could setup several security zones
   * Low cost, and highly capable packet filtering


Originally identified this issue on: 20-May-2017


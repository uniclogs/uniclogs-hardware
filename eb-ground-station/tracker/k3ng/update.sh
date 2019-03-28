#!/bin/bash
#
# Get latest K3NG code from GitHub
# G.N. LeBrasseur
# 26-Mar-2019
#
# Be sure to save features.h, pins.h, and settings.h into an Old_K3NG_files_DD-MMM-YYY directory first!
# There is need to consult these files to determine that all features / settings are included.
#
# This puts you in a place that any mods to k3ng_rotator_controller.ino are ready to be made if needed.
#
#
echo "=== Getting latest version of Rotator Controller and preparing environment ==="
cd tracker/k3ng
rm -rf k3ng_rotator_controller
git clone https://github.com/k3ng/k3ng_rotator_controller.git
cd k3ng_rotator_controller/k3ng_rotator_controller
cp rotator_features.h rotator_features_original_distro.h
cp rotator_pins.h rotator_pins_original_distro.h
cp rotator_settings.h rotator_settings_original_distro.h
echo ""
echo " Consult these files, don't just take them verbatim!"
echo "  ../../Old_K3NG_files_DD-MMM-YYYY/rotator_features.h"
echo "  ../../Old_K3NG_files_DD-MMM-YYYY/rotator_pins.h"
echo "  ../../Old_K3NG_files_DD-MMM-YYYY/rotator_settings.h"
echo ""
echo "=== Done updating Rotator Controller ==="

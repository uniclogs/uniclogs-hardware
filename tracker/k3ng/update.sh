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
cd ~/GlennStuff/Tracker/k3ng
rm -rf k3ng_rotator_controller
git clone https://github.com/k3ng/k3ng_rotator_controller.git
cd k3ng_rotator_controller/k3ng_rotator_controller
cp rotator_features.h rotator_features_original_distro.h
cp rotator_pins.h rotator_pins_original_distro.h
cp rotator_settings.h rotator_settings_original_distro.h
# Consult these files, don't just take them verbatim.
# cp ../../Old_K3NG_files_26-Mar-2019/rotator_features.h .
# cp ../../Old_K3NG_files_26-Mar-2019/rotator_pins.h .
# cp ../../Old_K3NG_files_26-Mar-2019/rotator_settings.h .
cp k3ng_rotator_controller.ino ../../Old_K3NG_files_26-Mar-2019/k3ng_rotator_controller_original_distro.ino
echo "=== Done updating Rotator Controller ==="

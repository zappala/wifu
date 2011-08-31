#!/bin/bash
python ./preliminary.py -u rbuck -c config/ilab_wired.conf
python ./preliminary.py -u rbuck -c config/ilab_wireless.conf
python ./preliminary.py -u rbuck -c config/mesh_wired.conf
python ./preliminary.py -u rbuck -c config/mesh_wireless.conf
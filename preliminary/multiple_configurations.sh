#!/bin/bash
python ./preliminary.py -u rbuck -c config/ilab_wired_10.conf
python ./preliminary.py -u rbuck -c config/ilab_wired_100.conf
python ./preliminary.py -u rbuck -c config/ilab_wired_1000.conf
python ./preliminary.py -u rbuck -c config/ilab_wireless.conf
python ./preliminary.py -u rbuck -c config/mesh_wired_10.conf
python ./preliminary.py -u rbuck -c config/mesh_wired_100.conf
python ./preliminary.py -u rbuck -c config/mesh_wired_1000.conf
python ./preliminary.py -u rbuck -c config/mesh_wireless.conf
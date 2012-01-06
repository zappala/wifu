#!/bin/bash

# These 12 runs constitute the wired experiments
#python ./preliminary.py -u rbuck -c config/ilab_wired_10_1MB.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_100_1MB.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_1000_1MB.conf

#python ./preliminary.py -u rbuck -c config/ilab_wired_10_100KB.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_100_100KB.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_1000_100KB.conf

#python ./preliminary.py -u rbuck -c config/ilab_wired_10_10MB.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_100_10MB.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_1000_10MB.conf

#python ./preliminary.py -u rbuck -c config/ilab_wired_10_10KB.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_100_10KB.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_1000_10KB.conf

# Mesh wireless 1-3 hops, 1 thread
#cd /home/rbuck/NetBeansProjects/mesh-config/custom_config
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-one-hop.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-one-hop.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-one-hop.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-one-hop.json
#cd /home/rbuck/NetBeansProjects/wifu-end/preliminary
#python ./preliminary.py -u rbuck -c config/mesh_wireless_1_hop.conf

cd /home/rbuck/NetBeansProjects/mesh-config/custom_config
./config.sh -k -r -m "Randy's thesis work" ./conf/other-two-hops.json
./config.sh -k -r -m "Randy's thesis work" ./conf/randy-two-hops.json
./config.sh -k -r -m "Randy's thesis work" ./conf/other-two-hops.json
./config.sh -k -r -m "Randy's thesis work" ./conf/randy-two-hops.json
cd /home/rbuck/NetBeansProjects/wifu-end/preliminary
python ./preliminary.py -u rbuck -c config/mesh_wireless_2_hops.conf

#cd /home/rbuck/NetBeansProjects/mesh-config/custom_config
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-three-hops.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-three-hops.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-three-hops.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-three-hops.json
#cd /home/rbuck/NetBeansProjects/wifu-end/preliminary
#python ./preliminary.py -u rbuck -c config/mesh_wireless_3_hops.conf

# Mesh wireless 1-3 hops, 2 threads
#cd /home/rbuck/NetBeansProjects/mesh-config/custom_config
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-one-hop.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-one-hop.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-one-hop.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-one-hop.json
#cd /home/rbuck/NetBeansProjects/wifu-end/preliminary
#python ./preliminary.py -u rbuck -c config/mesh_wireless_1_hop_2_threads.conf

cd /home/rbuck/NetBeansProjects/mesh-config/custom_config
./config.sh -k -r -m "Randy's thesis work" ./conf/other-two-hops.json
./config.sh -k -r -m "Randy's thesis work" ./conf/randy-two-hops.json
./config.sh -k -r -m "Randy's thesis work" ./conf/other-two-hops.json
./config.sh -k -r -m "Randy's thesis work" ./conf/randy-two-hops.json
cd /home/rbuck/NetBeansProjects/wifu-end/preliminary
python ./preliminary.py -u rbuck -c config/mesh_wireless_2_hops_2_threads.conf

#cd /home/rbuck/NetBeansProjects/mesh-config/custom_config
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-three-hops.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-three-hops.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-three-hops.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-three-hops.json
#cd /home/rbuck/NetBeansProjects/wifu-end/preliminary
#python ./preliminary.py -u rbuck -c config/mesh_wireless_3_hops_2_threads.conf

# Mesh wireless 1-3 hops, 5 threads
#cd /home/rbuck/NetBeansProjects/mesh-config/custom_config
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-one-hop.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-one-hop.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-one-hop.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-one-hop.json
#cd /home/rbuck/NetBeansProjects/wifu-end/preliminary
#python ./preliminary.py -u rbuck -c config/mesh_wireless_1_hop_5_threads.conf

cd /home/rbuck/NetBeansProjects/mesh-config/custom_config
./config.sh -k -r -m "Randy's thesis work" ./conf/other-two-hops.json
./config.sh -k -r -m "Randy's thesis work" ./conf/randy-two-hops.json
./config.sh -k -r -m "Randy's thesis work" ./conf/other-two-hops.json
./config.sh -k -r -m "Randy's thesis work" ./conf/randy-two-hops.json
cd /home/rbuck/NetBeansProjects/wifu-end/preliminary
python ./preliminary.py -u rbuck -c config/mesh_wireless_2_hops_5_threads.conf

#cd /home/rbuck/NetBeansProjects/mesh-config/custom_config
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-three-hops.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-three-hops.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-three-hops.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-three-hops.json
#cd /home/rbuck/NetBeansProjects/wifu-end/preliminary
#python ./preliminary.py -u rbuck -c config/mesh_wireless_3_hops_5_threads.conf

# Mesh wireless 1-3 hops, 10 threads
#cd /home/rbuck/NetBeansProjects/mesh-config/custom_config
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-one-hop.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-one-hop.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-one-hop.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-one-hop.json
#cd /home/rbuck/NetBeansProjects/wifu-end/preliminary
#python ./preliminary.py -u rbuck -c config/mesh_wireless_1_hop_10_threads.conf

cd /home/rbuck/NetBeansProjects/mesh-config/custom_config
./config.sh -k -r -m "Randy's thesis work" ./conf/other-two-hops.json
./config.sh -k -r -m "Randy's thesis work" ./conf/randy-two-hops.json
./config.sh -k -r -m "Randy's thesis work" ./conf/other-two-hops.json
./config.sh -k -r -m "Randy's thesis work" ./conf/randy-two-hops.json
cd /home/rbuck/NetBeansProjects/wifu-end/preliminary
python ./preliminary.py -u rbuck -c config/mesh_wireless_2_hops_10_threads.conf

#cd /home/rbuck/NetBeansProjects/mesh-config/custom_config
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-three-hops.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-three-hops.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/other-three-hops.json
#./config.sh -k -r -m "Randy's thesis work" ./conf/randy-three-hops.json
#cd /home/rbuck/NetBeansProjects/wifu-end/preliminary
#python ./preliminary.py -u rbuck -c config/mesh_wireless_3_hops_10_threads.conf

# ilab scalability
#python ./preliminary.py -u rbuck -c config/ilab_wired_10_1MB_10_threads.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_100_1MB_10_threads.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_1000_1MB_10_threads.conf

#python ./preliminary.py -u rbuck -c config/ilab_wired_10_1MB_50_threads.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_100_1MB_50_threads.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_1000_1MB_50_threads.conf

#python ./preliminary.py -u rbuck -c config/ilab_wired_10_1MB_100_threads.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_100_1MB_100_threads.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_1000_1MB_100_threads.conf

# ilab big files
#python ./preliminary.py -u rbuck -c config/ilab_wired_10_100MB.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_100_100MB.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_1000_100MB.conf

##python ./preliminary.py -u rbuck -c config/ilab_wired_10_500MB.conf
##python ./preliminary.py -u rbuck -c config/ilab_wired_100_500MB.conf
##python ./preliminary.py -u rbuck -c config/ilab_wired_1000_500MB.conf

# ilab scalability
##python ./preliminary.py -u rbuck -c config/ilab_wired_1000_1MB_10_threads.conf
##python ./preliminary.py -u rbuck -c config/ilab_wired_100_1MB_10_threads.conf
##python ./preliminary.py -u rbuck -c config/ilab_wired_10_1MB_10_threads.conf

#python ./preliminary.py -u rbuck -c config/ilab_wired_10_1MB_50_threads.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_100_1MB_50_threads.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_1000_1MB_50_threads.conf

#python ./preliminary.py -u rbuck -c config/ilab_wired_10_1MB_100_threads.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_100_1MB_100_threads.conf
#python ./preliminary.py -u rbuck -c config/ilab_wired_1000_1MB_100_threads.conf

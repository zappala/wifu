WiFu: A Composable Toolkit for Experimental Wireless Transport Protocols
========================================================================

WiFu is a user-level toolkit for developing transport protocols, with
an emphasis on wireless networks. WiFu consists of two parts:

1) WiFu Transport for end-to-end protocol design. WiFu Transport
protocols are based on components, allowing researchers to easily
plug-and-play with new designs. For example, you could pull out TCP's
window-based congestion control algorithm and replace it with
rate-based congestion control. Likewise, you could remove TCP's
ACK-based reliability and replace it with FEC.

2) WiFu Core for cross-layer design. WiFu Core allows packets to be
intercepted and modified on a per-hop basis. For example, you could
implement per-hop retransmission for lossy networks.

Features
--------

* _Flexibility_: Protocols are built out of components, so it is easy
to plug-and-play with new reliability or congestion control protocols

* _Code Reuse_: WiFu comes with event handling, timers, packet
formats, and other functionality so you get the convenience of a
simulator with the reality of an implementation

* _Performance_: WiFu TCP is as fast as the kernel in wireless
networks and on 100 Mbps Ethernet, while having identical fairness
among flows

* _Easy Porting_: WiFu implements the standard BSD socket API, so it
is simple to port any application for use with WiFu

* _Network Debugging_: WiFu comes with a mock network object and
Wireshark packet traces, greatly simplifying the process of debugging
a network protocol

* _Scalability_: Support for multiple applications, built with
threading, written entirely in C++

Papers
======

* Randy Buck, Rich Lee, Philip Lundrigan, and Daniel Zappala. WiFu: A
  Composable Toolkit for Experimental Wireless Transport Protocols. In
  IEEE Ninth International Conference on Mobile Ad-Hoc and Sensor
  Systems, 2012.

* Randy Buck. WiFu Transport: A User-level Protocol
  Framework. M.S. Thesis, Brigham Young University, 2012.

* Xingang Zhang, Randy Buck, and Daniel Zappala. 2011. “Experimental
  Performance Evaluation of ATP in a Wireless Mesh Network.” In 2011
  IEEE Eighth International Conference on Mobile Ad-Hoc and Sensor
  Systems, 122–131. IEEE. doi:http://dx.doi.org/10.1109/MASS.2011.25

Support
=======

This project is supported by the National Science Foundation under
Grant No. 0917240.


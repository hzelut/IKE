#!/bin/sh

cp moon/* /etc/swanctl/conf.d/
cp ./strongswan.conf /etc/
ipsec restart
sleep 1
swanctl --load-all
swanctl -i --ike gw-gw

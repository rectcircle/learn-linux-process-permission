#!/usr/bin/env bash

echo "proc 1 root"
cat /proc/1/status | grep Cap
echo
echo "proc normal root"
sudo -u root sh -c 'cat /proc/$$/status | grep Cap'
echo
echo "proc normal nobody"
sudo -u nobody sh -c 'cat /proc/$$/status | grep Cap'
echo

#!/bin/bash

getconfig()
{
    echo "PCI Devices,"
    lspci | sed s/" "/','/1
    echo "CPU Specification,"
    lscpu | sed s/":"/','/1 | tr -d ' '
}

echo -en "Getting system details..."
getconfig > pci.csv
echo "Done."
ls -l pci.csv

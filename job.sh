#!/bin/sh 

echo "START---------------"
echo "WORKDIR " ${PWD}
cmsRun tmpConfig.py
echo "END OF RUNNING"
ls -al
echo "STOP---------------"

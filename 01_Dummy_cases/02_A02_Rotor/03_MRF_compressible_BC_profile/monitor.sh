#!/bin/bash
foamMonitor -l postProcessing/Residuals/0/residuals.dat &    
foamMonitor -l postProcessing/TurbResiduals/0/residuals.dat & 
foamMonitor postProcessing/maxPressure/0/volFieldValue.dat 

#foamMonitor postProcessing/magMaxValues/0/volFieldValue.dat


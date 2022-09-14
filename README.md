# LeafSim

This repo is part of a research project I worked on at Texas A&M.

The idea is to simulate electron beam therapy using Geant4.

Specifically, we have a 10 MeV electron beam hitting two types of targets, a CsI (Cesium Iodide) and water target. Theses targets are seperated into voxels and act as detectors to record energy desposition. Part of the electron beam is blocked by tungsten plates called 'Leaves'.

The goal is to measure radiation dosage in these voxels based off of the position of these tungsten plates.

Script_Leaf_Analysis is where we analyze the data, utilizing ROOT, after the simulation is done. This produces the histograms TotalWaterEnergy and WaterHitDet listed below.

Some of the pictures below are visual representations of the simulations created by Geant4.

    Red lines with yellow dots -> electron beam
    Blue/cyan rectangular boxes -> tungsten plates (leaves)
    Yellow voxeled sqaure -> CsI detectors
    White voxeled square in Sim_picture -> Water detector
    Green lines shown in screenshots -> Neutral particles (in this case gamma particles)

Yellow dots in the electron beam show step points used by Geant4.

![Screenshot from 2017-11-13 11-08-27](https://user-images.githubusercontent.com/111601247/185709933-d8467415-40f2-4701-9b34-a645d6e52e28.png)

![Sim_picture](https://user-images.githubusercontent.com/111601247/185709940-cb5a2500-7026-44e0-9454-1122170ba7aa.png)

![TotalWaterEnergy](https://user-images.githubusercontent.com/111601247/185709969-f49fe032-04fe-46be-88ca-4857e768c4a1.png)

TotalWaterEnergy: Shows the amount of energy each individual particle (from the electron beam) deposited into the water detectors (white voxeled square).

![WaterHitDet](https://user-images.githubusercontent.com/111601247/185709989-ed2d1beb-7511-4f53-9873-e9805f15950c.png)

WaterHitDet: Shows which voxel the electron from the beam hit. The voxels are numbered from 0 to 99. They start from the top row, then left to right, then second row, left to right, and so on.

# LeafSim

This repo is part of a research project I worked on at Texas A&M.

The idea is to simulate electron beam therapy using Geant4.

Specifically, we have a 10 MeV electron beam hitting two types of targets, a CsI (Cesium Iodide) and water target. Theses targets are seperated into voxels and act as detectors to record energy desposition. Part of the electron beam is blocked by tungsten plates called 'Leaves'.

The goal is to measure radiation dosage in these voxels based off of the position of these tungsten plates.

PrimaryGeneratorAction is where we implement the electron beam and any secondary particles.

DetectorConstrunction is where we build the world box that holds the physics, detectors, tungsten plates, etc.

Script_Leaf_Analysis is where we analyze the data, utilizing ROOT, after the simulation is done. This produces the histograms TotalWaterEnergy and WaterHitDet listed below or in the 'Images' Folder.

Some of the pictures below are visual representations of the simulations created by Geant4.

    Red lines with yellow dots -> electron beam
    Blue/cyan rectangular boxes -> tungsten plates (leaves)
    Yellow voxeled sqaure -> CsI detectors
    White voxeled square in Sim_picture -> Water detector
    Green lines shown in screenshots -> Neutral particles (in this case gamma particles)

Yellow dots in the electron beam show step points used by Geant4.

![image](https://user-images.githubusercontent.com/111601247/185682446-1ef7dd1a-32ea-4ac4-9161-47f1fffcb595.png)

![image](https://user-images.githubusercontent.com/111601247/185682500-c6c094b1-0e40-40ee-b79e-2ab6754b6af4.png)

![image](https://user-images.githubusercontent.com/111601247/185682613-efce91f3-3817-4b6c-bd0d-e3d541032925.png)

TotalWaterEnergy: Shows the amount of energy each individual particle (from the electron beam) deposited into the water detectors (white voxeled square).

![image](https://user-images.githubusercontent.com/111601247/185682655-5a4c2367-4900-4e51-92b3-1d9b4f651b19.png)

WaterHitDet: Shows which voxel the electron from the beam hit. The voxels are numbered from 0 to 99. They start from the top row, then left to right, then second row, left to right, and so on.

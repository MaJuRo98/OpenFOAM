# OpenFOAM
My personal OpenFOAM case collection
Step by step case setup to generate an axial compressor

To add:

X Create segments in blockMesh with a certain radius or copy from nccImpeller 

X Run createNonConformalCouples for the segment

X Cyclic boundary conditions

X Change size of file by input variable

X Automatic slice detection -> create Python script to generate nonConformingCoupling-file

X Generate a mesh with cfMesh


|
-> Create patches x 

|
-> Add boundary layer x

|
-> Refine on patches x
|
-> Refine with box, step wise refinment -> finish cfMesh-File

Mesh rotor and run actual 0.5 stages case

Add static domains

Add IGV and stators


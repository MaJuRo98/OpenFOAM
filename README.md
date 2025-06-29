# OpenFOAM case Collection

This repository contains my personal collection of OpenFOAM case setups, focused on generating simulation cases for an **axial compressor rotor**.  
Each case will later includes a step-by-step configuration process.

---

## ✅ Completed Tasks

### General setup
- [x] Create segments in `blockMesh` with a defined radius or copy from `nccImpeller`
- [x] Run `createNonConformalCouples` for the segments
- [x] Set up cyclic boundary conditions
- [x] Add input variable for dynamic file sizing
- [x] Create automatic slice detection using Python script (for `nonConformingCoupling` file)
- [x] Generate mesh with `cfMesh`
- [x] Mesh rotor with commercial meshing tool
- [x] Find working boundary conditions for case **A02**

### cfMesh setup
- [x] Create patches  
- [x] Add boundary layers  
- [x] Refine on patches  
- [x] Refine using box (stepwise refinement) – finalize `cfMesh` dict

### Profiles and Boundary Conditions
- Create radial profiles of:
  - [x] Turbulent kinetic energy `k`  
  - [x] Dissipation rate `ε`  


---

## 🛠️ To-Do

### Single domain setup
- Write custom boundary condition for **total pressure profile**

### Multiple domains Setup
- Add static domains
- Add IGV and stators
- Add multible stages 



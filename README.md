# OpenFOAM Case Collection

This repository contains my personal collection of OpenFOAM case setups, focused on generating simulation cases for an **axial compressor rotor**.  
Each case includes a step-by-step configuration process.

---

## ✅ Completed Tasks

### General Setup
- Create segments in `blockMesh` with a defined radius or copy from `nccImpeller`
- Run `createNonConformalCouples` for the segments
- Set up cyclic boundary conditions
- Add input variable for dynamic file sizing
- Create automatic slice detection using Python script (for `nonConformingCoupling` file)
- Generate mesh with `cfMesh`
- Mesh rotor with commercial meshing tool
- Find working boundary conditions for case **A02**

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

### Single Domain Setup
- Write custom boundary condition for **total pressure profile**

### Multiple Domain Setup
- Add static domains
- Add IGV (Inlet Guide Vanes) and stators



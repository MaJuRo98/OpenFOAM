rho = thermo.rho();
rho.relax();

const volScalarField rAU("rAU", 1.0/UrelEqn.A());
const surfaceScalarField rhorAUf("rhorAUf", fvc::interpolate(rho*rAU));

tmp<volScalarField> rAtU
(
    simple.consistent()
  ? volScalarField::New("rAtU", 1.0/(1.0/rAU - UrelEqn.H1()))
  : tmp<volScalarField>(nullptr)
);
tmp<surfaceScalarField> rhorAtUf
(
    simple.consistent()
  ? surfaceScalarField::New("rhoRAtUf", fvc::interpolate(rho*rAtU()))
  : tmp<surfaceScalarField>(nullptr)
);

const volScalarField& rAAtU = simple.consistent() ? rAtU() : rAU;
const surfaceScalarField& rhorAAtUf =
    simple.consistent() ? rhorAtUf() : rhorAUf;

volVectorField HbyA(constrainHbyA(rAU*UrelEqn.H(), Urel, p));

tUrelEqn.clear();

surfaceScalarField phiHbyA
(
    "phiHbyA",
    fvc::interpolate(rho)*fvc::flux(HbyA)
);

//MRF.makeRelative(fvc::interpolate(rho), phiHbyA);

bool closedVolume = false;

// Update the pressure BCs to ensure flux consistency
constrainPressure(p, rho, Urel, phiHbyA, rhorAAtUf);

if (simple.transonic())
{
    const surfaceScalarField phid
    (
        "phid",
        fvc::interpolate(psi)*phiHbyA/fvc::interpolate(rho)
    );

    const fvScalarMatrix divPhidp(fvm::div(phid, p));
    phiHbyA -= divPhidp.flux();

    if (simple.consistent())
    {
        phiHbyA += (rhorAAtUf - rhorAUf)*fvc::snGrad(p)*mesh.magSf();
        HbyA += (rAAtU - rAU)*fvc::grad(p);
    }

    while (simple.correctNonOrthogonal())
    {
        fvScalarMatrix pEqn
        (
            fvc::div(phiHbyA) + divPhidp
          - fvm::laplacian(rhorAAtUf, p)
          ==
            fvModels.source(psi, p, rho.name())
        );

        // Relax the pressure equation to ensure diagonal-dominance
        pEqn.relax();

        pEqn.setReference
        (
            pressureReference.refCell(),
            pressureReference.refValue()
        );

        pEqn.solve();

        if (simple.finalNonOrthogonalIter())
        {
            phi = phiHbyA + pEqn.flux();
        }
    }
}
else
{
    closedVolume = adjustPhi(phiHbyA, Urel, p);

    if (simple.consistent())
    {
        phiHbyA += (rhorAAtUf - rhorAUf)*fvc::snGrad(p)*mesh.magSf();
        HbyA += (rAAtU - rAU)*fvc::grad(p);
    }

    while (simple.correctNonOrthogonal())
    {
        fvScalarMatrix pEqn
        (
            fvc::div(phiHbyA)
          - fvm::laplacian(rhorAAtUf, p)
          ==
            fvModels.source(psi, p, rho.name())
        );

        pEqn.setReference
        (
            pressureReference.refCell(),
            pressureReference.refValue()
        );

        pEqn.solve();

        if (simple.finalNonOrthogonalIter())
        {
            phi = phiHbyA + pEqn.flux();
        }
    }
}

#include "incompressible/continuityErrs.H"

// Explicitly relax pressure for momentum corrector
p.relax();

Urel = HbyA - rAAtU*fvc::grad(p);
Urel.correctBoundaryConditions();
fvConstraints.constrain(Urel);

fvConstraints.constrain(p);

// For closed-volume cases adjust the pressure and density levels
// to obey overall mass continuity
if (closedVolume && !thermo.incompressible())
{
    p += (initialMass - fvc::domainIntegrate(psi*p))
        /fvc::domainIntegrate(psi);
    p.correctBoundaryConditions();
}

rho = thermo.rho();
rho.relax();

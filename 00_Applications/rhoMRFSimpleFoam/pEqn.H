rho = thermo.rho();
rho.relax();

const volScalarField rAU("rAU", 1.0/UEqn.A());
const surfaceScalarField rhorAUf("rhorAUf", fvc::interpolate(rho*rAU));

volVectorField HbyA(constrainHbyA(rAU*UEqn.H(), U, p));

tUEqn.clear();


surfaceScalarField phiHbyA
(
    "phiHbyA",
    fvc::interpolate(rho)*fvc::flux(HbyA)
);


//Macht flussgroessen und alles drum herum relativ
MRF.makeRelative(fvc::interpolate(rho), phiHbyA);


// Update the pressure BCs to ensure flux consistency
constrainPressure(p, rho, U, phiHbyA, rhorAUf, MRF);



if (simple.transonic())
{
    //Interpolation der Groeßen vom Zellkern auf die Oberflaechen
    surfaceScalarField psisf = fvc::interpolate(psis);
    surfaceScalarField rhof = fvc::interpolate(rho);

    // Needs to be outside of loop since p is changing, but psi and rho are not.
    surfaceScalarField rhoReff = rhof - psisf*fvc::interpolate(p);

    // Update the pressure BCs to ensure flux consistency
    //constrainPressure(p, rho, U, phiHbyA, rhorAUf, MRF);


    surfaceScalarField phid2
    (
        "phid2",
        (rhoReff/rhof)*phiHbyA
    );

    surfaceScalarField phid
    (
        "phid",
        (psisf/rhof)*phiHbyA
    );


    //Ueberprüft Flusssplitting, sollte null sein!
    const surfaceScalarField splitError
    (
        "splitError",
        phid2
        + phid*fvc::interpolate(p)
        - phiHbyA
    );

    Info<< "Max flux splitting error: "
        << max(mag(splitError)).value()
        << endl;


    while (simple.correctNonOrthogonal())
    {
        if (p.needReference())
        {
            FatalErrorInFunction
                << "Transonic pressure equation requires "
                << "a prescribed absolute pressure boundary "
                << "when setReference() is disabled."
                << exit(FatalError);
        }
        
        fvScalarMatrix pEqn
        (
            fvm::div(phid, p)
            + fvc::div(phid2)
            - fvm::laplacian(rhorAUf, p)
        );

        // Relax the pressure equation to ensure diagonal-dominance
        pEqn.relax();

        //Notwendig fuer inkompressible rechnung, daher wird vernachlassigt
        /*
        pEqn.setReference
        (
            pressureReference.refCell(),
            pressureReference.refValue()
        );
        */

        pEqn.solve();

        /* Alte Fluss-Berechnung
        if (simple.finalNonOrthogonalIter())
        {
            phi = phiHbyA + pEqn.flux();
        }
        */

        if (simple.finalNonOrthogonalIter())
        {
            phi = phid2 + pEqn.flux();
        }

    }
    #include "incompressible/continuityErrs.H"

}
else
{
    adjustPhi(phiHbyA, U, p);    

    while (simple.correctNonOrthogonal())
    {
        fvScalarMatrix pEqn
        (
            fvc::div(phiHbyA)
            - fvm::laplacian(rhorAUf, p)
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

    
    #include "incompressible/continuityErrs.H"

}


// Explicitly relax pressure for momentum corrector
p.relax();

//Velocity correction based on pressure term
U = HbyA - rAU*fvc::grad(p);

U.correctBoundaryConditions();
fvConstraints.constrain(U);
fvConstraints.constrain(p);

rho = thermo.rho();
rho.relax();


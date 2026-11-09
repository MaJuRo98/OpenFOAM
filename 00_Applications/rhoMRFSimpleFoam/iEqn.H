{
    //Solve the rothalpy equation

    //Create relative velocity
    URel == U;
    MRF.makeRelative(URel);
    
    // Bound the relative velocity to preserve the i to h conversion bound
    // HJ, 22/Mar/2017
    volScalarField magURel(mag(URel));

    // Create rotational velocity (= omega x r)
    URot = U - URel;


    // Unterer Block abgeschlossen, oberer muss noch korrigiert werden
    volScalarField& h = thermo.he();

    if (h.name() != "h")
    {
        FatalErrorInFunction
            << "rhoMRFSimpleFoam requires enthalpy formulation (h), "
            << "but thermo.he() returned " << h.name()
            << exit(FatalError);
    }


    i == h + 0.5*magSqr(U) - (U & URotGeom);
    i.correctBoundaryConditions();


    // + thermophysicalTransport->divq(i), erstzt durch - fvm::laplacian, da näher an foam-extend


    //Here the conversation of energy gets solved
    fvScalarMatrix iEqn
    (
        fvm::div(phi, i)
        + fvm::SuSp(-fvc::div(phi), i)
        - fvm::laplacian(thermophysicalTransport->alphaEff(),i)
        + (turbulence->devTau() && fvc::grad(URel))
     ==
        fvModels.source(rho, i)
    );

    iEqn.relax();

    fvConstraints.constrain(iEqn);

    iEqn.solve();

    fvConstraints.constrain(i);
    
    // Calculate enthalpy out of rothalpy
    h == i - 0.5*magSqr(U) + (U & URotGeom);
    h.correctBoundaryConditions();

    // Apply OF10 temperature limits to enthalpy
    fvConstraints.constrain(h);

    thermo.correct();

    //isentropic compressibility
    psis = thermo.psi()/thermo.Cp()*thermo.Cv();

    // TODO:
    // Check whether foam-extend psis is still required by the OF10 pEqn.


}

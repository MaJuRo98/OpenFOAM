    // Solve the Momentum equation

    tmp<fvVectorMatrix> tUrelEqn
    (
        fvm::div(phi, Urel)
      + turbulence->divDevTau(Urel)
      + rho*SRF->Su()
     ==
        fvModels.source(rho, Urel)
    );
    fvVectorMatrix& UrelEqn = tUrelEqn.ref();

    UrelEqn.relax();

    fvConstraints.constrain(UrelEqn);

    solve(UrelEqn == -fvc::grad(p));

    fvConstraints.constrain(Urel);

{
    volScalarField& herel = thermo.he();

    fvScalarMatrix ErelEqn
    (
        fvm::div(phi, herel)
      + (
            herel.name() == "e"
          ? fvc::div(phi, volScalarField("Ekprel", 0.5*magSqr(Urel) + p/rho))
          : fvc::div(phi, volScalarField("Krel", 0.5*magSqr(Urel)))
        )
      + thermophysicalTransport->divq(herel)
     ==
        fvModels.source(rho, herel)
    );

    ErelEqn.relax();

    fvConstraints.constrain(ErelEqn);

    ErelEqn.solve();

    fvConstraints.constrain(herel);

    thermo.correct();
}

Info<< "Reading thermophysical properties\n" << endl;

autoPtr<fluidThermo> pThermo
(
    fluidThermo::New(mesh)
);
fluidThermo& thermo = pThermo();
thermo.validate(args.executable(), "h");

volScalarField& h = thermo.he();
volScalarField& p = thermo.p();

// Isentropic compressibility for the transonic pressure equation
volScalarField psis
(
    IOobject
    (
        "psis",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    thermo.psi()*thermo.Cv()/thermo.Cp()
);

volScalarField rho
(
    IOobject
    (
        "rho",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    thermo.rho()
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

volScalarField UTheta
(
    IOobject
    (
        "UTheta",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar
    (
        "zero",
        dimVelocity,
        0.0
    )
);

// Create relative velocity
volVectorField URel
(
    IOobject
    (
        "URel",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    U
);

// Create rotational velocity (= omega x r)
volVectorField URot
(
    "URot",
    U - URel
);
// Create geometrical rotational velocity (= omega x r) in case of alteration between U and URel
volVectorField URotGeom
(
    IOobject
    (
        "URotGeom",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedVector
    (
        "zero",
        dimVelocity,
        vector::zero
    )
);

// Create rothalpy, in two steps to preserve boundary conditions
volScalarField i
(
    IOobject
    (
        "i",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

i == h + 0.5*magSqr(U) - (U & URot);

#include "compressibleCreatePhi.H"

pressureReference pressureReference
(
    p,
    simple.dict(),
    thermo.incompressible()
);

mesh.schemes().setFluxRequired(p.name());

Info<< "Creating turbulence model\n" << endl;
autoPtr<compressible::momentumTransportModel> turbulence
(
    compressible::momentumTransportModel::New
    (
        rho,
        U,
        phi,
        thermo
    )
);

Info<< "Creating thermophysical transport model\n" << endl;
autoPtr<fluidThermophysicalTransportModel> thermophysicalTransport
(
    fluidThermophysicalTransportModel::New(turbulence(), thermo)
);

dimensionedScalar initialMass = fvc::domainIntegrate(rho);

#include "createMRF.H"
#include "createFvModels.H"
#include "createFvConstraints.H"

{
    // Calculate UTheta from U using MRF-Library
    // Geometric rotational velocity field:
    // URotGeom = Omega x (x - origin) inside MRF zones

    volVectorField URotGeom
    (
        IOobject
        (
            "URotGeom",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedVector
        (
            "zero",
            dimVelocity,
            vector::zero
        )
    );

    // Zero relative velocity transformed to absolute:
    // gives geometric rotational velocity
    MRF.makeAbsolute(URotGeom);

    // Prevent division by zero on rotation axes
    // and in stationary regions
    const dimensionedScalar smallURot
    (
        "smallURot",
        dimVelocity,
        1e-12
    );

    // Signed tangential component of absolute U
    // UTheta = U dot eTheta
    UTheta ==
        (U & URotGeom)
       /max(mag(URotGeom), smallURot);


    Info<< "UTheta min = "
        << gMin(UTheta)
        << ", max = "
        << gMax(UTheta)
        << endl;

}

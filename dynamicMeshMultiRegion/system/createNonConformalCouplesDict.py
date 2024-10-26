# Python script to write the full header with C++ and version info
# and the createNonConformalCouplesDict header

# Define the full header content including C++ and version info


#User input
startPhi = 0
bladeNumber = 63
pitch = 360/bladeNumber          #Carefull, does not allways apply

# Define the file path to save the output
output_file_path = "someTextFile.txt"

header_content = """\
/*--------------------------------*- C++ -*----------------------------------*\\
| =========                 |                                                 |
| \\\\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\\\    /   O peration     | Version:  10                                    |
|   \\\\  /    A nd           | Website:  www.OpenFOAM.org                      |
|    \\\\/     M anipulation  |                                                 |
\\*---------------------------------------------------------------------------*/

FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    location    "constant";
    object      createNonConformalCouplesDict;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

"""

#nonConformalCoupleIn
coupleOneIn = [
    f"nonConformalCoupleIn{startPhi+pitch}\n",
    "{\n",
    "   patches         (nonCoupleIn1 nonCoupleIn2);\n",
    "   transform       none; \n",
    "}\n",
]
coupleTwoIn = [
    f"nonConformalCoupleIn{startPhi+pitch*2}\n",
    "{\n",
    f"   $nonConformalCoupleIn{startPhi+pitch}; \n",
    f"   transform rotational; \n",
    f"   rotationAxis   (0 0 1); \n",
    f"   rotationCentre (0 0 0); \n",
    f"   rotationAngle {startPhi+pitch*2}; \n",
    "}\n",
]

#nonConformalCoupleOut
coupleOneOut = [
    f"nonConformalCoupleOut{startPhi+pitch}\n",
    "{\n",
    "   patches         (nonCoupleOut1 nonCoupleOut2);\n",
    "   transform       none; \n",
    "}\n",
]
coupleTwoOut = [
    f"nonConformalCoupleOut{startPhi+pitch*2}\n",
    "{\n",
    f"   $nonConformalCoupleOut{startPhi+pitch}; \n",
    f"   transform rotational;\n",
    f"   rotationAxis   (0 0 1); \n",
    f"   rotationCentre (0 0 0); \n",
    f"   rotationAngle -{startPhi+pitch*2}; \n",
    "}\n",
]

emptyLine = f"\n"

i = 0
j = 0

# File to 
with open(output_file_path, 'w') as file:
    file.write(header_content)
    file.write(emptyLine)
    for line in coupleOneIn:
        file.write(line)
    file.write(emptyLine)

    for line in coupleTwoIn:
        file.write(line)
    file.write(emptyLine)

    while j < bladeNumber-2:
        value = startPhi+pitch*(j+3)
        if value <= 360:
            value = value
        else:
            value = value - 360
        i = i+1
        file.write(f"nonConformalCoupleIn{value} {{  $nonConformalCoupleIn{startPhi+pitch*2}; rotationAngle {value};  }} \n")
        j = j+1
        print(f"Hi {j}")
    
    file.write(emptyLine)
    file.write(emptyLine)

    for line in coupleOneOut:
        file.write(line)
    
    file.write(emptyLine)
    
    for line in coupleTwoOut:
        file.write(line)
    file.write(emptyLine)
    j = 0
    while j < bladeNumber-2:
        value = startPhi+pitch*(j+3)
        if value <= 360:
            value = value
        else:
            value = value - 360
        i = i+1
        file.write(f"nonConformalCoupleOut{value} {{  $nonConformalCoupleOut{startPhi+pitch*2}; rotationAngle {-value};  }} \n")
        j = j+1
        print(f"Hi {j}")
    file.write(emptyLine)
    file.write("""// ************************************************************************* // """)

print(f"Full header written to {output_file_path}")
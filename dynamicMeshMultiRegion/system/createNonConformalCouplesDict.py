# Python script to write the full header with C++ and version info
# and the createNonConformalCouplesDict header

# Define the full header content including C++ and version info


#User input
startPhi = 30
deltaPhi = 60
i = 0
j = 0
secondValue = 20
currentValue = 0


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


coupleOneIn = [
    f"nonConformalCoupleIn{startPhi+deltaPhi}\n",
    "{\n",
    "   patches         (nonCoupleIn1 nonCoupleIn2);\n",
    "   transform       none; \n",
    "}\n",
]
coupleTwoIn = [
    f"nonConformalCoupleIn{startPhi+deltaPhi*2}\n",
    "{\n",
    f"$nonConformalCoupleIn{startPhi+deltaPhi}; \n",
    f"transform rotational;",
    f"rotationAxis ( 0 0 1);",
    f"rotationAngle {startPhi+deltaPhi*2}; \n",
    "}\n",
]

coupleN = f"nonConformalCoupleIn{startPhi+deltaPhi*2} {{  $nonConformalCoupleIn{startPhi+deltaPhi*(3+i)}; rotationAngle {startPhi+deltaPhi*(3+i)};  }} \n"

emptyLine = f"\n"



# Define the file path to save the output
output_file_path = "someTextFile.txt"

# Write the header to the new file
with open(output_file_path, 'w') as file:
    file.write(header_content)
    for line in coupleOneIn:
        file.write(line)
    for line in coupleTwoIn:
        file.write(line)
    while j < 3:
        value = startPhi+deltaPhi*j
        i = i+1
        file.write(f"nonConformalCoupleIn{value} {{  $nonConformalCoupleIn{startPhi+deltaPhi*2}; rotationAngle {value};  }} \n")
        j = j+1
        print(f"Hi {j}")

print(f"Full header written to {output_file_path}")



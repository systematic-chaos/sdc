#include <stdio.h>
#include <stdlib.h>

#include <math.h>

typedef struct Sample {
    double x1;
    double x2;
    double f;
} Sample;

typedef struct Derivative {
    double dx1;
    double dx2;
} Derivative;

typedef struct DerivedSample {
    double x1;
    double x2;
    struct Derivative fDeriv;
} DerivedSample;

void derivative(double fromX1, double toX1, double fromX2, double toX2,
    unsigned int nSamples, char centerCell);
unsigned int generateBraninMatrix(double fromX1, double toX1, unsigned int numX1Samples,
    double fromX2, double toX2, unsigned int numX2Samples, Sample* braninMatrix);
unsigned int generateCellMatrix(Sample* vertexes, unsigned int height, unsigned int width, Sample** cells);
unsigned int computeCellDerivatives(Sample** cellsVertexes, unsigned int numCells, char centerCall,
    DerivedSample* derivatives);
void printSampleMatrix(Sample* sampleMatrix, unsigned int numSamples);
void printDerivedSampleMatrix(DerivedSample* derivedSampleMatrix, unsigned int numSamples);
double branin(double x1, double x2);
double braninHoo(double x1, double x2, double a, double b, double c, double r, double s, double t);
DerivedSample deriveSampleCell(Sample* cellVertexes[4], char centerCell);
Derivative derive(double f[4], double d[2], double r, double s);
double pythagoras(double a, double b);

int main(int argc, char** argv) {
    double fromX1 = -5.;
    double toX1 = 10.; /*12*/
    double fromX2 = 0.;
    double toX2 = 15.; /*16*/
    unsigned int nSamples = 4;
    char centerCell = 1;
    
    derivative(fromX1, toX1, fromX2, toX2, nSamples, centerCell);
    
    return 0;
}

void derivative(double fromX1, double toX1, double fromX2, double toX2,
        unsigned int nSamples, char centerCell) {
    printf("\n[%.0f , %.0f] -> [%.0f , %.0f]\t%d samples\t%s center cell\n",
        fromX1, fromX2, toX1, toX2, nSamples, centerCell ? "yes" : "no");
    
    struct Sample* sampleMatrix = malloc(sizeof(Sample) * nSamples * nSamples);
    generateBraninMatrix(fromX1, toX1, nSamples, fromX2, toX2, nSamples, sampleMatrix);
    
    Sample** cellsVertexes = malloc(sizeof(Sample*) * (nSamples - 1) * (nSamples - 1) * 4);
    unsigned int numCells = generateCellMatrix(sampleMatrix, nSamples, nSamples, cellsVertexes);
    
    DerivedSample* derivatives = malloc(sizeof(DerivedSample) * numCells);
    computeCellDerivatives(cellsVertexes, numCells, centerCell, derivatives);
    
    printSampleMatrix(sampleMatrix, nSamples * nSamples);
    printDerivedSampleMatrix(derivatives, numCells);
    
    free(derivatives);
    free(cellsVertexes);
    free(sampleMatrix);
}

/** Generate array (actually a bidimensional array indexed by rows) of samples
    from input parameters. Each sample contains its position in the (X1,X2)
    dimension axis and the value of the Branin-Hoo function for such (X1,X2)
    values. Default values are assigned for the other Branin function variables.  */
unsigned int generateBraninMatrix(double fromX1, double toX1, unsigned int numX1Samples,
 double fromX2, double toX2, unsigned int numX2Samples, Sample* braninMatrix) {
    double stepX1 = (toX1 - fromX1) / (numX1Samples - 1);
    double stepX2 = (toX2 - fromX2) / (numX2Samples - 1);
    double x1 = fromX1, x2;
    int i, j;
    
    for (i = 0; i < numX1Samples; i++) {
        x2 = fromX2;
        for (j = 0; j < numX2Samples; j++) {
            braninMatrix[i * numX2Samples + j] = (struct Sample) { x1, x2, branin(x1, x2) };
            x2 += stepX2;
        }
        x1 += stepX1;
    }
    
    return numX1Samples * numX2Samples;
}

/** Generate a matrix of rectangular cells from a matrix of vertices.
    Vertexes is handled as a bidimensional matrix of vertices of given height and width.
    Result is an array (which can be handled as a matrix of dimensions (height - 1) x (width - 1)),
    where each element means a cell and encompasses four memory addresses; hence,
    filling four positions in the resulting array. These four memory addresses point to the
    elements (in the vertexes matrix) which make for a rectangular cell. The output matrix is
    indexed in the same way as the input matrix (rows or columns). The four vertices sequence
    which comprises a cell is indexed as [[x0, y0], [x1, y0], [x0, y1], [x1, y1]] if the original
    vertices matrix is indexed by rows, and [[x0, y0], [x0, y1], [x1, y0], [x1, y1]] if the input
    matrix is indexed by columns. */
unsigned int generateCellMatrix(Sample* vertexes, unsigned int height, unsigned int width, Sample** cells) {
    int i, j, cellBaseIndex;
    for (i = 0; i < height - 1; i++) {
        for (j = 0; j < width - 1; j++) {
            cellBaseIndex = (i * (width - 1) + j) * 4;
            cells[cellBaseIndex]     = vertexes + (i * height + j);
            cells[cellBaseIndex + 1] = vertexes + (i * height + j + 1);
            cells[cellBaseIndex + 2] = vertexes + ((i + 1) * height + j);
            cells[cellBaseIndex + 3] = vertexes + ((i + 1) * height + j + 1);
        }
    }
    
    return cellBaseIndex / 4 + 1;
}

/** Compute the derivative the cells defined in the cellsVertexes matrix.
    A cell comprises four positions of the array, which are pointers to Sample vertices,
    as returned from generateCellMatrix(). NumCells equates to the array size / 4 (in terms
    of memory addresses) and centerCell indicates whether the cell's coordinates should be
    adjusted to the cell's center or just reflect its first vertex. */
unsigned int computeCellDerivatives(Sample** cellsVertexes, unsigned int numCells, char centerCell,
        DerivedSample* derivatives) {
    int n;
    for (n = 0; n < numCells; n++) {
        derivatives[n] = deriveSampleCell(cellsVertexes + n * 4, centerCell);
    }
    return n;
}

/** Compute Branin-Hoo function's value from given x1 and x2 parameters,
    applying default values to the other function variables. */
double branin(double x1, double x2) {
    return braninHoo(x1, x2,
        1.,
        5.1  / (4. * M_PI * M_PI),
        5. / M_PI,
        6.,
        10.,
        1. / (8. * M_PI));
}

/** Compute Branin-Hoo function's value from given parameters. */
double braninHoo(double x1, double x2,
        double a, double b, double c,
        double r, double s, double t) {
    double auxResult = x2 - b * x1 * x1 + c * x1 - r;
    return a * auxResult * auxResult + s * (1. - t) * cos(x1) + s;    
}

/** Compute the derivative of a rectangular cell defined by four sample vertexes
    (bidimensional position and value). centerCell indicates whether the cell
    position should be regarded as the center of the cell itself (true) or the
    position of the first vertex (false). */
DerivedSample deriveSampleCell(Sample* cellVertexes[4], char centerCell) {
    double f[4] = { cellVertexes[0] -> f,
                    cellVertexes[1] -> f,
                    cellVertexes[2] -> f,
                    cellVertexes[3] -> f };
    double d[2] = { fabs((cellVertexes[2] -> x1) - (cellVertexes[0] -> x1)),
                    fabs((cellVertexes[1] -> x2) - (cellVertexes[0] -> x2)) };
    
    double r = cellVertexes[0] -> x1;
    double s = cellVertexes[0] -> x2;
    if (centerCell) {
        r += d[0] / 2.;
        s += d[1] / 2.;
    }
    
    return (DerivedSample){ r, s, derive(f, d, r, s) };
}

/** Compute the partial derivative for a bidimensional cell. */
Derivative derive(double f[4], double d[2], double r, double s) {
    Derivative deriv;
    deriv.dx1 = (1. - s) * (f[1] - f[0]) / d[0] + s * (f[2] - f[3]) / d[0];
    deriv.dx2 = (1. - r) * (f[3] - f[0]) / d[1] + r * (f[2] - f[1]) / d[1];
    return deriv;
}

double pythagoras(double a, double b) {
    return sqrt(a * a + b * b);
}

void printSampleMatrix(Sample* sampleMatrix, unsigned int numSamples) {
    printf("\nSAMPLES MATRIX [x1, x2, f]\n");
    
    Sample* auxSample;
    int n;
    for (n = 0; n < numSamples; n++) {
        auxSample = sampleMatrix + n;
        printf("[%.4f , %.4f ,\t%.4f]\n", auxSample->x1, auxSample->x2, auxSample->f); 
    }
}

void printDerivedSampleMatrix(DerivedSample* derivedSampleMatrix, unsigned int numSamples) {
    printf("\nDERIVED SAMPLES MATRIX [x1, x2, dx1, dx2]\n");
    
    DerivedSample* auxSample;
    int n;
    for (n = 0; n < numSamples; n++) {
        auxSample = derivedSampleMatrix + n;
        printf("[%.4f , %.4f ,\t%.4f , %.4f ,\t%.4f]\n", auxSample->x1, auxSample->x2,
            auxSample->fDeriv.dx1, auxSample->fDeriv.dx2,
            pythagoras(auxSample->fDeriv.dx1, auxSample->fDeriv.dx2));
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BUFSIZE 20

typedef struct Point {
    int index;
    float x;
    float y;
} Point;

typedef struct PointLinkedList {
    struct Point point;
    struct PointLinkedList* next;
} PointLinkedList;

typedef struct Line {
    int from;
    int to;
} Line;

typedef struct LineLinkedList {
    struct Line line;
    struct LineLinkedList* next;
} LineLinkedList;

typedef struct Curve {
    struct PointLinkedList* points;
    struct LineLinkedList* lines;
    int numPoints;
    int numLines;
} Curve;

typedef struct PolylineLinkedList {
    int* polyline;
    int polylineLength;
    struct PolylineLinkedList* next;
} PolylineLinkedList;

PolylineLinkedList marchingSquares(Curve curv);
Curve readFile(char* fileName);
Point readPoint(char* buf);
Line readLine(char* buf);
int readPointsFromFile(FILE* fd, PointLinkedList* pointList);
int readLinesFromFile(FILE* fd, LineLinkedList* lineList);
void storePolyline(PolylineLinkedList* plp, int* plBuf, int nVertexes);
void printPolylines(Curve* c, PolylineLinkedList* lines);
Point** pointLinkedListToArray(PointLinkedList* list, int numPoints);

int main(int argc, char** argv) {
    Curve curv = readFile(argc > 1 ? argv[1] : "curves.txt");
    
    if (curv.numPoints < 1 || curv.numLines < 1) return 1;
    
    PolylineLinkedList polylines = marchingSquares(curv);
    printPolylines(&curv, &polylines);
    
    return 0;
}

PolylineLinkedList marchingSquares(Curve curv) {
    int* polylineBuffer = malloc(sizeof(int) * curv.numLines);
    PolylineLinkedList polylineList = { NULL, 0, NULL };
    PolylineLinkedList* plp = &(polylineList);
    
    LineLinkedList* lp = curv.lines;
    Line* l;
    int previousVertex = -1;
    int numLineVertices;
    int numPolylines = 0;
    int n;
    for (n = 0; n < curv.numLines; n++) {
        l = &(lp -> line);
        if ((l -> from) != (l -> to)) {
            /* New polyline */
            if (previousVertex != (l -> from)) {
                /* This isn't the first polyline, therefore we must store the
                   previous one, just completed, and advance into the list. */
                if (previousVertex >= 0) {
                    storePolyline(plp, polylineBuffer, numLineVertices);
                    plp -> next = malloc(sizeof(PolylineLinkedList));
                    plp = plp -> next;
                    numPolylines++;
                }
                
                /* The counter for the number of vertices in the current line is reset,
                   which implies the start of a new polyline, and the first vertex of
                   this new polyline is stored. */
                numLineVertices = 0;
                polylineBuffer[numLineVertices++] = l -> from;
            }
            
            /* The section's destination vertex is added to the current polyline */
            previousVertex = l -> to;
            polylineBuffer[numLineVertices++] = previousVertex;
        }
        lp = lp -> next;
    }
    
    /* The last polyline completed is stored */
    storePolyline(plp, polylineBuffer, numLineVertices);
    plp -> next = NULL;
    
    free(polylineBuffer);
    
    return polylineList;
}

void storePolyline(PolylineLinkedList* plp, int* plBuf, int nVertexes) {
    plp -> polylineLength = nVertexes;
    plp -> polyline = malloc(sizeof(int) * nVertexes);
    memcpy(plp -> polyline, plBuf, sizeof(int) * nVertexes);
}

void printPolylines(Curve* c, PolylineLinkedList* lines) {
    /* Print vertices indexes for polylines */
    PolylineLinkedList* pl = lines;
    int n;
    do {
        printf("[ ");
        for (n = 0; n < pl -> polylineLength; n++) {
            printf("%d ", pl -> polyline[n]);
        }
        printf("]\n");
    } while ((pl = pl->next) != NULL);
    
    /* Print vertices coordinates for polylines */
    Point** points = pointLinkedListToArray(c -> points, c -> numPoints);
    int indexPadding = points[0] -> index;
    Point* p;
    pl = lines;
    do {
        printf("[ ");
        for (n = 0; n < pl->polylineLength; n++) {
            p = points[pl->polyline[n] - indexPadding];
            printf("(%.1f , %.1f) ", p -> x, p -> y);
        }
        printf("]\n");
    } while ((pl = pl->next) != NULL);
    
    /* For each polyline, print whether it defines an open or close outline */
    pl = lines;
    do {
        printf("%s outline\n",
            (pl -> polyline[0]) == (pl -> polyline[pl -> polylineLength - 1]) ?
                "Closed" : "Open");
    } while ((pl = pl->next) != NULL);
}

Curve readFile(char* fileName) {
    Curve resultCurve;
    FILE* fd = fopen(fileName, "r");
    
    if (fd != NULL) {
        PointLinkedList* pointList = malloc(sizeof(PointLinkedList));
        int numPoints = readPointsFromFile(fd, pointList);
        
        LineLinkedList* lineList = malloc(sizeof(LineLinkedList));
        int numLines = readLinesFromFile(fd, lineList);
        
        fclose(fd);
        
        resultCurve.points = pointList;
        resultCurve.numPoints = numPoints;
        resultCurve.lines = lineList;
        resultCurve.numLines = numLines;
    } else {
        resultCurve.points = NULL;
        resultCurve.lines = NULL;
        resultCurve.numPoints = 0;
        resultCurve.numLines = 0;
    }
    
    return resultCurve;
}

int readPointsFromFile(FILE* fd, PointLinkedList* pointList) {
    PointLinkedList* p = pointList;
    int numPoints = 0;
    
    char* buf = malloc(sizeof(char) * BUFSIZE);
    while(fgets(buf, BUFSIZE, fd) != NULL && buf[0] != '\n') {
        p -> point = readPoint(buf);
        p -> next = malloc(sizeof(PointLinkedList));
        p = p -> next;
        numPoints++;
    }
    p -> next = NULL;
    free(buf);
    
    return numPoints;
}

Point readPoint(char* buf) {
    Point p;
    sscanf(buf, "%d\t%f %f\n", &(p.index), &(p.x), &(p.y));
    return p;
}

Point** pointLinkedListToArray(PointLinkedList* list, int numPoints) {
    Point** array = malloc(sizeof(Point*) * numPoints);
    
    int n;
    for (n = 0; list != NULL; n++) {
        array[n] = &(list -> point);
        list = list -> next;
    }
    return array;
}

int readLinesFromFile(FILE* fd, LineLinkedList* lineList) {
    LineLinkedList* l = lineList;
    unsigned int numLines = 0;
    
    char* buf = malloc(sizeof(char) * BUFSIZE);
    while (fgets(buf, BUFSIZE, fd) != NULL) {
        l -> line = readLine(buf);
        l -> next = malloc(sizeof(LineLinkedList));
        l = l -> next;
        numLines++;
    }
    l -> next = NULL;
    free(buf);
    
    return numLines;
}

Line readLine(char* buf) {
    Line l;
    sscanf(buf, "%d %d", &(l.from), &(l.to));
    return l;
}

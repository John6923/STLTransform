#include <stdio.h>
#include <stdlib.h>

//#define VERBOSE

#define HEADER_SIZE 80

typedef struct {
	float xn, yn, zn;
	float x1, y1, z1;
	float x2, y2, z2;
	float x3, y3, z3;
	short att;
} Triangle;

#ifdef VERBOSE	
void printTriangle(Triangle * tp){
	printf("Triangle:\n\tNormal: %g %g %g \n\tVector 1: %g %g %g\n\tVector 2: %g %g %g\n\tVector 3: %g %g %g\n",
		tp->xn, tp->yn, tp-> zn, 
		tp->x1, tp->y1, tp-> z1,
		tp->x2, tp->y2, tp-> z2,
		tp->x3, tp->y3, tp-> z3);
}
#endif

void readFloat(float * floatp, FILE * fp){
	fread(floatp, sizeof(float), 1, fp);
}

void readTriangle(Triangle * tp, FILE * fp){
	readFloat(&(tp->xn),fp);
	readFloat(&(tp->yn),fp);
	readFloat(&(tp->zn),fp);
	readFloat(&(tp->x1),fp);
	readFloat(&(tp->y1),fp);
	readFloat(&(tp->z1),fp);
	readFloat(&(tp->x2),fp);
	readFloat(&(tp->y2),fp);
	readFloat(&(tp->z2),fp);
	readFloat(&(tp->x3),fp);
	readFloat(&(tp->y3),fp);
	readFloat(&(tp->z3),fp);
	fread(&(tp->att), sizeof(short), 1, fp);
}

void writeFloat(float * floatp, FILE * fp){
	fwrite(floatp, sizeof(float), 1, fp);
}

void writeTriangle(Triangle * tp, FILE * fp){
	writeFloat(&(tp->xn),fp);
	writeFloat(&(tp->yn),fp);
	writeFloat(&(tp->zn),fp);
	writeFloat(&(tp->x1),fp);
	writeFloat(&(tp->y1),fp);
	writeFloat(&(tp->z1),fp);
	writeFloat(&(tp->x2),fp);
	writeFloat(&(tp->y2),fp);
	writeFloat(&(tp->z2),fp);
	writeFloat(&(tp->x3),fp);
	writeFloat(&(tp->y3),fp);
	writeFloat(&(tp->z3),fp);
	fwrite(&(tp->att), sizeof(short), 1, fp);
}

void flipYandZ(Triangle * tp){
	float t;
	//Normal
	t = tp->yn;
	tp->yn = tp->zn;
	tp->zn = t;
	//Vertex 1
	t = tp->y1;
	tp->y1 = tp->z1;
	tp->z1 = t;
	//Vertex 2
	t = tp->y2;
	tp->y2 = tp->z2;
	tp->z2 = t;
	//Vertex 3
	t = tp->y3;
	tp->y3 = tp->z3;
	tp->z3 = t;
	
	
}

int main(int argc, char ** argv){
	if(argc < 3){
		printf("Not enough arguments \nUsage: stlt file_from file_to");
		return -1;
	}
	char header[HEADER_SIZE + 1];
	header[HEADER_SIZE] = '\0';
	unsigned long * number_of_triangles = malloc(sizeof(long));
	Triangle * triangles;
	FILE * fin;
	fin = fopen(argv[1],"r");
	if(fin == NULL){
		printf("Could not open input file\n");
	}
	fread(header,sizeof(char),HEADER_SIZE,fin);
	fread(number_of_triangles, sizeof(long),1,fin);
	triangles = malloc(sizeof(Triangle)*(*number_of_triangles));
	int i;
	for(i = 0; i < *number_of_triangles; i++){
		readTriangle(triangles + i, fin);
	}
	fclose(fin);
	#ifdef VERBOSE
	printf("Read.\n");
	#endif
	
	#ifdef VERBOSE
	//Before
	printf("Before\n");
	for(i = 0; i < *number_of_triangles; i++){
		printTriangle(triangles + i);
	}
	#endif
	
	for(i = 0; i < *number_of_triangles; i++){
		flipYandZ(triangles + i);
	}
	
	#ifdef VERBOSE
	//After
	printf("After\n");
	for(i = 0; i < *number_of_triangles; i++){
		printTriangle(triangles + i);
	}
	#endif
	
	FILE * fout;
	fout = fopen(argv[2],"w");
	if(fout == NULL){
		printf("Could not open output file\n");
	}
	fwrite(header,sizeof(char),HEADER_SIZE,fout);
	fwrite(number_of_triangles, sizeof(long),1,fout);
	for(i = 0; i < *number_of_triangles; i++){
		writeTriangle(triangles + i, fout);
	}
	fclose(fout);
	#ifdef VERBOSE
	printf("Written\n");
	#endif
	
	return 0;
}
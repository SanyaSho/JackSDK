#define VectorCopy(a,b) {b[0]=a[0];b[1]=a[1];b[2]=a[2];}
#define VectorLerp(a, b, c, t) {c[0] = a[0] + (b[0] - a[0]) * t;c[1] = a[1] + (b[1] - a[1]) * t;c[2] = a[2] + (b[2] - a[2]) * t;}

extern vec3_t vec3_origin;

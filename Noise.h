
void createNoise3D(GLuint*);
void SetNoiseFrequency(int);

double noise1(double);
double noise2(double);
double noise3(double);

void normalize2(double);
void normalize3(double);
void initNoise();

double PerlinNoise1D(double, double, double, int);
double PerlinNoise2D(double, double, double, double, int);
double PerlinNoise3D(double, double, double, double, double, int);

void make3DNoiseTexture(void);
void init3DNoiseTexture(int, GLuint*);

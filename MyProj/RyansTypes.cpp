#define Assert(Expression) if (!(Expression)) {*(int *)0 = 0;}
//NOTE AssertM is the same as Assert it just has a message perameter to say how to fix the assert.
// Just so we don't have to comment it every time
#define AssertM(Expression, Message) if (!(Expression)) {*(int *)0 = 0;}

int8_t typedef int8;
int16_t typedef int16;
int32_t typedef int32;
int64_t typedef int64;
unsigned char typedef uint8;
unsigned short typedef uint16;
unsigned int typedef uint32;
unsigned long long typedef uint64;
int32 typedef bool32;
float typedef real32;
double typedef  real64;

#define ARRAY_SIZE(Array, Type) sizeof(Array) / sizeof(Type)

#if UNIT_TESTING
void TestTypeSizes()
{
	Assert(sizeof(uint8) == 1);
	Assert(sizeof(uint16) == 2);
	Assert(sizeof(uint32) == 4);
	Assert(sizeof(uint64) == 8);
}
#endif

#define GameNull 0

#define ID_LINE __LINE__
#define ID_FILE __FILE__
#define ID_METHOD __func__
#define COUNTER __COUNTER__

//	DES function prototypes

#ifdef __cplusplus
extern "C" {
#endif

int desinit(int mode);
void desdone(void);
void setkey(char *key);
void endes(char *block);
void dedes(char *block);

#ifdef __cplusplus
}
#endif

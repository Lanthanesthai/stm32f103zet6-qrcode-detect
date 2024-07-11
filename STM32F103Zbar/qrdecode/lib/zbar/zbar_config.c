#include "zbar_config.h"
#include "stdlib.h"
#include <string.h>
#include "malloc.h"
void *Zbar_malloc(unsigned int size){
//	return malloc(size);
		return mymalloc(SRAMEX,size);
}
void *Zbar_calloc(unsigned int numElements,unsigned int sizeOfElement){
//	return calloc( numElements,sizeOfElement);
	void* result = mymalloc(SRAMEX,numElements*sizeOfElement);
	if(result != NULL){
		mymemset(result,0,numElements*sizeOfElement);
	}
	return result;
}
void *Zbar_realloc(void *ptr, unsigned int size){
//	return realloc(ptr,size);
	
		return myrealloc(SRAMEX,ptr,size);
}
void Zbar_memset(void *p,unsigned char c, unsigned int len){
//	 memset(p,c,len);
	mymemset(p,c,len); 
}
void Zbar_memcpy(void *des, void *src,unsigned int n){
//	 memcpy(des,src,n);
	mymemcpy((u8*)des,(u8*)src,n);
}
void Zbar_free(void *ptr){
//	free(ptr);
	myfree(SRAMEX,ptr);
}

#ifndef __ZBAR_CONFIG_H
#define __ZBAR_CONFIG_H

void *Zbar_malloc(unsigned int  size);
void *Zbar_calloc(unsigned int numElements,unsigned int sizeOfElement);
void *Zbar_realloc(void *ptr, unsigned int size);
void Zbar_memset(void *p,unsigned char c, unsigned int len);
void Zbar_memcpy(void *des,void *src,unsigned int n);
void Zbar_free(void *ptr);

#endif
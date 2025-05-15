#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animal.h"

static int animal_init(void* t, Animal_Attr attr) {
    ANIMAL_CLASS* this = (ANIMAL_CLASS*)t;
    
    memcpy(&this->attr, &attr, sizeof(Animal_Attr));
    
    return 0;
}

static int animal_get_name(void* t, char* name) {
    ANIMAL_CLASS* this = (ANIMAL_CLASS*)t;
    memcpy(name, this->attr.name, strlen(this->attr.name) + 1);
    return 0;
}

static int animal_speak(void* t) {
    ANIMAL_CLASS* this = (ANIMAL_CLASS*)t;

    printf("animal %s say: %s!\r\n", this->attr.name, this->attr.sound);

    return 0;
}

ANIMAL_CLASS* ANIMAL_CLASS_CTOR(void) {
    ANIMAL_CLASS* this = (ANIMAL_CLASS*)malloc(sizeof(ANIMAL_CLASS));

    this->api.init = animal_init;
    this->api.get_name = animal_get_name;
    this->api.speak = animal_speak;
    
    return this;
}

void ANIMAL_CLASS_DTOR(ANIMAL_CLASS* t) {
    free(t);
}

static int dog_speak(void* t) {
    DOG_CLASS* this = (DOG_CLASS*)t;

    printf("animal %s say: Woof!\r\n", this->attr.name);

    return 0;
}

DOG_CLASS* DOG_CLASS_CTOR(ANIMAL_CLASS_IMPLEMENTS* t) {
    DOG_CLASS* this = (DOG_CLASS*)malloc(sizeof(DOG_CLASS));

    t->get_name(t, this->attr.name);

    // 此处没有实现 init get_name 的接口
    this->api.speak = dog_speak;

    return this;
}

void DOG_CLASS_DTOR(DOG_CLASS* t) {
    free(t);
}

static int cat_speak(void* t) {
    CAT_CLASS* this = (CAT_CLASS*)t;

    printf("animal %s say: Meow!\r\n", this->attr.name);

    return 0;
}

CAT_CLASS* CAT_CLASS_CTOR(ANIMAL_CLASS_IMPLEMENTS* t) {
    CAT_CLASS* this = (CAT_CLASS*)malloc(sizeof(CAT_CLASS));

    t->get_name(t, this->attr.name);

    // 此处没有实现 init get_name 的接口
    this->api.speak = cat_speak;

    return this;
}

void CAT_CLASS_DTOR(CAT_CLASS* t) {
    free(t);
}

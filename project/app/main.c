#include <stdio.h>
#include <string.h>
#include "my_config.h"
#include "animal.h"

ANIMAL_CLASS_IMPLEMENTS* anmial_cat = NULL;
ANIMAL_CLASS_IMPLEMENTS* anmial_dog = NULL;

static int animal_sound(void* t) {
    ANIMAL_CLASS_IMPLEMENTS* this = (ANIMAL_CLASS_IMPLEMENTS*)t;

    this->speak(this);

    return 0;
}

int main()
{
    printf(">\r\n");

    char name[10] = {0};

    Animal_Attr Cat_Attr = {
        .name = CONFIG_ANIMAL_NAME_1,
    };

    Animal_Attr Dog_Attr = {
        .name = CONFIG_ANIMAL_NAME_2,
    };

    // 抽象
    anmial_cat = (ANIMAL_CLASS_IMPLEMENTS*)ANIMAL_CLASS_CTOR();
    anmial_cat->init(anmial_cat, Cat_Attr);
    
    anmial_dog = (ANIMAL_CLASS_IMPLEMENTS*)ANIMAL_CLASS_CTOR();
    anmial_dog->init(anmial_dog, Dog_Attr);

    // 封装
    memset(name, 0x00, sizeof(name));
    anmial_cat->get_name(anmial_cat, name);
    printf("animal name is %s\r\n", name);
    memset(name, 0x00, sizeof(name));
    anmial_dog->get_name(anmial_dog, name);
    printf("animal name is %s\r\n", name);

    ANIMAL_CLASS_IMPLEMENTS* dog = NULL;
    ANIMAL_CLASS_IMPLEMENTS* cat = NULL;

    // 继承
    cat = (ANIMAL_CLASS_IMPLEMENTS*)CAT_CLASS_CTOR(anmial_cat);
    dog = (ANIMAL_CLASS_IMPLEMENTS*)DOG_CLASS_CTOR(anmial_dog);

    cat->speak(cat);
    dog->speak(dog);

    // 多态
    animal_sound(cat);
    animal_sound(dog);

    printf("<\r\n");

    return 0;
}

#ifndef OOP_H
#define OOP_H

// Reference: https://stackoverflow.com/questions/415452/object-orientation-in-c
#define MENU_FUNC (void(*)(struct menu_base*))

struct menu_base;
struct menu_base_vtable
{
    //    void (*setup)(struct menu_base *);
    void (*draw)(struct menu_base *);
};

struct menu_base {
    struct menu_base_vtable *vtable;
    //void init();
    //void draw();
};

//void menu_base_init(struct menu_base *base)
//{
//    base->vtable->init(base);
//}

void menu_base_draw(struct menu_base *base)
{
    base->vtable->draw(base);
}

struct menu1 {
    menu_base base;
    //void menu1_init(struct menu1* menu);
    //void menu1_draw(struct menu1* menu);
};

void menu1_setup(struct menu1* menu)
{
    printf("menu1_setup\n");
}

void menu1_draw(struct menu1* menu)
{
    printf("menu1_draw\n");
}

struct menu_base_vtable menu1_vtable =
{
    //  &menu1_setup, /* you might get a warning here about incompatible pointer types */
    MENU_FUNC &menu1_draw   /* you can ignore it, or perform a cast to get rid of it */
};

void menu1_init(struct menu1* menu)
{
    menu->base.vtable = &menu1_vtable;
}

#endif //OOP_H

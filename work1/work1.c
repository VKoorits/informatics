#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define true 1
#define false 0
#define bool int
const float Eps = 0.000001;
bool is_test = false;


struct Vector{
    void *data;
    int n;
    int sizeof_elem;
};
struct complex{
    float x;//действительная часть
    float y;//мнимая часть
};

void add_int(int* n1, int* n2){ *n1 +=  *n2; }
void add_float(float* n1, float* n2){ *n1 +=  *n2; }
void add_complex(struct complex* n1, struct complex* n2){n1->x += n2->x; n1->y += n2->y; }

void mult_int_num(int* rez, int* num1, int* num2){ *rez += *num1 * *num2; }
void mult_float_num(float* rez, float* num1, float*num2){ *rez += *num1 * *num2; }
void mult_complex_num(struct complex* rez, struct complex* num1, struct complex* num2){
    float x1 = num1->x;
    float y1 = num1->y;
    float x2 = num2->x;
    float y2 = num2->y;
    rez->x += x1*x2 - y1*y2;
    rez->y += x1*y2 + x2*y1;
}

bool get_int_element(struct Vector* vector, int index, int* rez){
    if(vector->data && index < vector->n){
        *rez = *((int*)vector->data + index);
        return true;
    }
    return false;
}
bool get_float_element(struct Vector* vector, int index, float* rez){
    if(vector->data && index < vector->n){
        *rez = *((float*)vector->data + index);
        return true;
    }
    return false;
}
bool get_complex_element(struct Vector* vector, int index, struct complex* rez){
    if(vector->data && index < vector->n){
        *rez = *((struct complex*)vector->data + index);
        return true;
    }
    return false;
}

bool set_int_element(struct Vector* vector, int index, int val){
    if(vector->data && index < vector->n){
        *((int*)vector->data + index) = val;
        return true;
    }
    return false;
}
bool set_float_element(struct Vector* vector, int index, float val){
    if(vector->data && index < vector->n){
        *((float*)vector->data + index) = val;
        return true;
    }
    return false;
}
bool set_complex_element(struct Vector* vector, int index, struct complex val){
    if(vector->data && index < vector->n){
        *((struct complex*)vector->data + index) = val;
        return true;
    }
    return false;
}

bool eq_array_int(int arr1[], int arr2[], int n){
    int i;
    for(i=0; i<n; i++)
        if(arr1[i] != arr2[i])
            return false;
    return true;
}
bool eq_array_float(float arr1[], float arr2[], int n){
    int i;
    for(i=0; i<n; i++)
        if( abs(arr1[i] - arr2[i]) > Eps )
            return false;

    return true;
}
bool eq_array_complex(struct complex arr1[], struct complex arr2[], int n){
    int i;
    for(i=0; i<n; i++){
        if( abs(arr1[i].x - arr2[i].x) > Eps )
            return false;
        if( abs(arr1[i].y - arr2[i].y) > Eps )
            return false;
    }
    return true;
}


void print_int_num(int *num){printf("%d ", *num); }
void print_float_num(float *num){printf("%f ", *num); }
void print_complex_num(struct complex* num){
    printf("{%3.2f + i*%3.2f} ",  num->x, num->y );
}

void scan_int(int* num){ scanf("%d", num); }
void scan_float(float* num){ scanf("%f", num); }
void scan_complex(struct complex* num){
    scanf("%f", &(num->x));
    scanf("%f", &(num->y));
}

void test(void);
//########################################################################################



bool allocate_vector(struct Vector* vector, unsigned int count_elem, unsigned int sizeof_element){
    vector->data = 0;
    vector->data = malloc( count_elem * sizeof_element );
    if(!vector->data){
        printf("programm can`t allocate %d byte in memory", count_elem * sizeof_element );
        return false;
    }
    else{
        vector->n = count_elem;
        vector->sizeof_elem = sizeof_element;
        return true;
    }
}
bool copy_vector(struct Vector* new_vector, struct Vector* old_vector){
    if(new_vector->data)
        free(new_vector->data);

    new_vector->data = 0;
    new_vector->data = malloc( old_vector->n * old_vector->sizeof_elem );
    if(new_vector->data){
        unsigned int i;
        for(i = 0; i < old_vector->n * old_vector->sizeof_elem; ++i)
            *((char*)new_vector->data + i) = *((char*)old_vector->data + i);//побайтное копирование data
        new_vector->n = old_vector->n;
        new_vector->sizeof_elem = old_vector->sizeof_elem;
        return true;
    }
    return false;
}
bool add_vectors(struct Vector* vector1, struct Vector* vector2, void (*f)(void*, void*)){
    if(vector1->n != vector2->n){
        if(!is_test)
            printf("unsupposed vectors %d and %d elemnets", vector1->n, vector2->n);
        return false;
    }
    int i;
    for(i=0; i< vector1->n; ++i)
        f( ((char*)vector1->data + i*vector1->sizeof_elem), ((char*)vector2->data + i*vector2->sizeof_elem));
    return true;
}
bool scalar_multy_vectors(struct Vector* vector1, struct Vector* vector2, void* rez, void(*f)(void*, void*, void*)){
    if(vector1->n != vector2->n){
        if(!is_test)
            printf("unsupposed vectors %d and %d elemnets", vector1->n, vector2->n);
        return false;
    }
    if(vector1->sizeof_elem != vector2->sizeof_elem){
            if(!is_test)
            printf("unsupposed sizeof elements %d and %d", vector1->sizeof_elem, vector2->sizeof_elem);
        return false;
    }

    int i;
    for(i=0; i<vector1->sizeof_elem; i++)//обнуляем rez
        *((char*)rez + i) = 0;

    for(i=0; i < vector1->n; ++i)
        f(rez, (char*)vector1->data + i*vector1->sizeof_elem,  (char*)vector2->data + i*vector1->sizeof_elem );

    return true;
}
void print_vector(struct Vector* v, void(*print)(void*) ){
    int i;
    printf("\tVector: ");
    for(i=0; i < v->n; i++)
        print((char*)v->data + i*v->sizeof_elem);
    printf("\n");
}
void input_vector(struct Vector* v, void(*scan)(void*) ){
    printf("enter count element: ");
    scanf("%d", &v->n);
    if(! allocate_vector(v, v->n, v->sizeof_elem) )
        return;
    int i;
    printf("Enter %d number/(pair if complex type) : ", v->n);
    for(i=0; i < v->n; ++i)
        scan( (char*)v->data + i*v->sizeof_elem );
}
void delete_vector(struct Vector* v){
    free(v->data);
    v->n = 0;
    v->sizeof_elem = 0;
}
void print_menu(){
    printf("\n\tYo can:\n1.Add vectors\n2.Scalar multy vectors\n3.Begin test programm\n4.Quit\nWhat do you want? ");
}

int main(){
    int answer = 0;
    int type = 0;
    char c;
    struct Vector v1, v2;
    struct complex num;
    while(answer != 4){
        print_menu();
        scanf("%d", &answer);
        switch(answer){
            case 1:
            case 2:
                printf("\tEnter type of vector elements:\n\t  1.int\n\t  2.float\n\t  3.complex number\n\t\tWhat yo want?\t");
                scanf("%d", &type);
                printf("First vector:\n");
                if(type == 1){
                    input_vector(&v1, scan_int);
                    printf("\nSecond vector:\n");
                    input_vector(&v2, scan_int);
                    printf("You result:\t");

                    if(answer == 1){
                        if(add_vectors(&v1, &v2, add_int)){
                            print_vector(&v1, print_int_num);
                        }
                    }else{
                        int res;
                        if(scalar_multy_vectors(&v1, &v2, &res, mult_int_num) ){
                            printf("%d", res);
                        }
                    }
                }else if(type == 2){
                    input_vector(&v1, scan_float);
                    printf("\nSecond vector:\n");
                    input_vector(&v2, scan_float);
                    printf("You result:\t");

                    if(answer == 1){
                        if(add_vectors(&v1, &v2, add_float)){
                            print_vector(&v1, print_float_num);
                        }
                    }else{
                        float res;
                        if(scalar_multy_vectors(&v1, &v2, &res, mult_float_num) ){
                            printf("%f", res);
                        }
                    }
                }else if(type == 3){
                    input_vector(&v1, scan_complex);
                    printf("\nSecond vector:\n");
                    input_vector(&v2, scan_complex);
                    printf("You result:\t");

                    if(answer == 1){
                        if(add_vectors(&v1, &v2, add_complex)){
                            print_vector(&v1, print_complex_num);
                        }
                    }else{
                        struct complex res;
                        if(scalar_multy_vectors(&v1, &v2, &res, mult_complex_num)){
                            print_complex_num(&res);
                        }
                    }
                }

                break;
            case 3:
                test();
                break;
            case 4:
                printf("You want Quit from programm(y/n)?\t");
                c = _getch();
                printf("\n");
                if(c != 'y' && c != 'Y')
                    answer = 0;
                break;
            default:
                printf("bad input\n");
        }

    }

    printf("\a\n=======================================\npress any key ");
    _getch();
}


void test(){
is_test = true;
    int good_tests = 0, count_tests = 0;
    int size_now = sizeof(int);
    struct Vector v1,v2,v3,v4;

    int int_values1[5] = {2,3,5,7,11};
    int int_values2[5] = {1,2,3,5,8};
    int int_values3[4] = {10, 20, 30,40};
    int int_result1[5] = {3,5,8,12,19};
    const int scalar_multyply_v1_v2 = 146;

    v1.sizeof_elem = size_now;
    v2.sizeof_elem = size_now;
    v3.sizeof_elem = size_now;
    v1.n = v2.n = 5;
    v3.n = 4;
    v1.data = int_values1;
    v2.data = int_values2;
    v3.data = int_values3;


//===============================================COPY INT TEST
printf("\n\nINT VECTORS TEST:\n****************************************************\n");
    printf("COPY INT TEST\t\t\t\t");
    copy_vector(&v4, &v1);
    if( eq_array_int(v1.data, v4.data, 5) && v1.n==v2.n && v1.sizeof_elem==v2.sizeof_elem ){
        printf("Test passed\n");
        good_tests++;
    }else{
        printf("Test falled\n");
        printf("got:     ");
        print_vector(&v4, print_int_num);
        printf("Expected:");
        print_vector(&v1, print_int_num);
    }
    count_tests++;

//==================================================ADD INT TEST
    printf("ADD INT TEST\t\t\t\t");//v4 + v2;
    if(add_vectors(&v4, &v2, add_int) ){

        if( eq_array_int(v4.data, int_result1, 5)){
            printf("Test passed\n");
            good_tests++;
        }else{
            printf("Test falled\n");
            printf("got:     ");
            print_vector(&v4, print_int_num);
            printf("Expected:");

            struct Vector v_res;
            v_res.data = int_result1;
            v_res.n = 5;
            v_res.sizeof_elem = sizeof(int);

            print_vector(&v_res, print_int_num);
        }
    }else{
        printf("Test falled\n");
        printf("Bad vectors sizes: %d and %d", v4.n, v3.n);
    }
    count_tests++;
//==================================================BADD VECTORS ADD FLOAT TEST
    printf("BADD VECTORS ADD INT TEST\t\t");//v4 + v3;
    copy_vector(&v4, &v1);
    if(! add_vectors(&v4, &v3, add_int) ){
        printf("Test passed\n");
        good_tests++;
    }else{
        printf("Test falled\n");
        printf("got      : v4.n == v3.n == %d\nExpected: bad input\n", v4.n);
    }
    count_tests++;
//============================================================SCALAR MULTYPLY FLOAT TEST
    printf("SCALAR MULTYPLY INT TEST\t\t");//v4 + v2;
    int res = 0;
    if( scalar_multy_vectors(&v1, &v2, &res, mult_int_num ) ){

        if( res == scalar_multyply_v1_v2){
            printf("Test passed\n");
            good_tests++;
        }else{
            printf("Test falled\n");
            printf("got:      %d\n", res);
            printf("Expected: %d", scalar_multyply_v1_v2);
        }
    }else{
        printf("Test falled\n");
        printf("Bad vectors sizes: %d and %d", v4.n, v3.n);
    }
    count_tests++;
//==================================================BAD SCALAR MULTY VECTORS INT TEST
    printf("BAD SCALAR MULTY VECTORS INT TEST\t");//v1 + v3;

    if(! scalar_multy_vectors(&v1, &v3, &res, mult_int_num) ){
        printf("Test passed\n");
        good_tests++;
    }else{
        printf("Test falled\n");
        printf("got      : v1.n == v3.n == %d\nExpected: bad input\n", v4.n);
    }
    count_tests++;
printf("\n****************************************************\n");
if(count_tests == good_tests)
    printf("\tso good!!!!\n");
else
    printf("falled %d tests \n", count_tests - good_tests);
printf("\n\n");

//#########################################################################################################
//#########################################################################################################


    good_tests = count_tests = 0;
    size_now = sizeof(float);


    float float_values1[5] = {2.2,3.3,5.5,7.7,11.11};
    float float_values2[5] = {1.1,2.2,3.3,5.5,8.8};
    float float_values3[4] = {10.5, 20.25, 30125,40.0625};
    float float_result1[5] = {3.3,5.5,8.8,13.2,19.91};
    const float scalar_multyply_v1_v2_float = 167.948;

    v1.sizeof_elem = size_now;
    v2.sizeof_elem = size_now;
    v3.sizeof_elem = size_now;
    v1.n = v2.n = 5;
    v3.n = 4;
    v1.data = float_values1;
    v2.data = float_values2;
    v3.data = float_values3;


//===============================================COPY FLOAT TEST
printf("FLOAT VECTORS TEST:\n****************************************************\n");
    printf("COPY FLOAT TEST\t\t\t\t");
    copy_vector(&v4, &v1);
    if( eq_array_float(v1.data, v4.data, 5) && v1.n==v2.n && v1.sizeof_elem==v2.sizeof_elem ){
        printf("Test passed\n");
        good_tests++;
    }else{
        printf("Test falled\n");
        printf("got:     ");
        print_vector(&v4, print_float_num);
        printf("Expected:");
        print_vector(&v1, print_float_num);
    }
    count_tests++;

//==================================================ADD FLOAT TEST
    printf("ADD FLOAT TEST\t\t\t\t");//v4 + v2;
    if(add_vectors(&v4, &v2, add_float) ){
        if( eq_array_float(v4.data, float_result1, 5)){
            printf("Test passed\n");
            good_tests++;
        }else{
            printf("Test falled\n");
            printf("got:     ");
            print_vector(&v4, print_float_num);
            printf("Expected:");

            struct Vector v_res;
            v_res.data = float_result1;
            v_res.n = 5;
            v_res.sizeof_elem = sizeof(float);

            print_vector(&v_res, print_float_num);
        }
    }else{
        printf("Test falled\n");
        printf("Bad vectors sizes: %d and %d", v4.n, v3.n);
    }
    count_tests++;
//==================================================BADD VECTORS ADD FLOAT TEST
    printf("BADD VECTORS ADD FLAOT TEST\t\t");//v4 + v3;
    copy_vector(&v4, &v1);
    if(! add_vectors(&v4, &v3, add_float) ){
        printf("Test passed\n");
        good_tests++;
    }else{
        printf("Test falled\n");
        printf("got      : v4.n == v3.n == %d\nExpected: bad input\n", v4.n);
    }
    count_tests++;
//============================================================SCALAR MULTYPLY FLOAT TEST
    printf("SCALAR MULTYPLY FLOAT TEST\t\t");//v4 + v2;
    float res_f = 0;
    if( scalar_multy_vectors(&v1, &v2, &res_f, mult_float_num) ){
        if( abs(res - scalar_multyply_v1_v2) < Eps ) {
            printf("Test passed\n");
            good_tests++;
        }else{
            printf("Test falled\n");
            printf("got:      %d\n", res);
            printf("Expected: %d", scalar_multyply_v1_v2_float);
        }
    }else{
        printf("Test falled\n");
        printf("Bad vectors sizes: %d and %d", v4.n, v3.n);
    }
    count_tests++;


//==================================================BAD SCALAR MULTY VECTORS FLOAT TEST
    printf("BAD SCALAR MULTY VECTORS FLOAT TEST\t");//v1 + v3;

    if(! scalar_multy_vectors(&v1, &v3, &res, mult_float_num) ){
        printf("Test passed\n");
        good_tests++;
    }else{
        printf("Test falled\n");
        printf("got      : v1.n == v3.n == %d\nExpected: bad input\n", v4.n);
    }
    count_tests++;
printf("\n****************************************************\n");
if(count_tests == good_tests)
    printf("\tso good!!!!\n");
else
    printf("falled %d tests \n", count_tests - good_tests);
printf("\n\n");

//#########################################################################################################
//#########################################################################################################

    good_tests = count_tests = 0;
    size_now = sizeof(struct complex);



    struct complex complex_values1[2] = { {1.1, 2.2}, {3.3, 5.5} };
    struct complex complex_values2[2] = { {7.7, 2.2}, {3.3, 4.4} };
    struct complex complex_values3[3] = { {10.5, 20.25}, {30125,40.0625}, {2.2, 7.58}};
    struct complex complex_result1[2] = { {8.8, 4.4}, {6.6, 9.9} };
    struct complex scalar_multyply_v1_v2_complex = {-9.68, 52.03};

    v1.sizeof_elem = size_now;
    v2.sizeof_elem = size_now;
    v3.sizeof_elem = size_now;
    v1.n = v2.n = 2;
    v3.n = 3;
    v1.data = complex_values1;
    v2.data = complex_values2;
    v3.data = complex_values3;


//===============================================COPY COMOPLEX TEST
printf("COMPLEX VECTORS TEST:\n****************************************************\n");
    printf("COPY COMPLEX TEST\t\t\t");
    copy_vector(&v4, &v1);
    if( eq_array_complex(v1.data, v4.data, 2) && v1.n==v2.n && v1.sizeof_elem==v2.sizeof_elem ){
        printf("Test passed\n");
        good_tests++;
    }else{
        printf("Test falled\n");
        printf("got:     ");
        print_vector(&v4, print_float_num);
        printf("Expected:");
        print_vector(&v1, print_complex_num);
    }
    count_tests++;

//==================================================ADD COMPLEX TEST
    printf("ADD COMPLEX TEST\t\t\t");//v4 + v2;
    if(add_vectors(&v4, &v2, add_complex ) ){
        if( eq_array_complex(v4.data, complex_result1, 2)){
            printf("Test passed\n");
            good_tests++;
        }else{
            printf("Test falled\n");
            printf("got:     ");
            print_vector(&v4, print_complex_num);
            printf("Expected:");

            struct Vector v_res;
            v_res.data = complex_result1;
            v_res.n = 2;
            v_res.sizeof_elem = sizeof(float);

            print_vector(&v_res, print_complex_num);
        }
    }else{
        printf("Test falled\n");
        printf("Bad vectors sizes: %d and %d", v4.n, v3.n);
    }
    count_tests++;
//==================================================BADD VECTORS ADD COMPLEX TEST
    printf("BADD VECTORS ADD COMPLEX TEST\t\t");//v4 + v3;
    copy_vector(&v4, &v1);
    if(! add_vectors(&v4, &v3, add_complex) ){
        printf("Test passed\n");
        good_tests++;
    }else{
        printf("Test falled\n");
        printf("got      : v4.n == v3.n == %d\nExpected: bad input\n", v4.n);
    }
    count_tests++;
//============================================================SCALAR MULTYPLY FLOAT TEST
    printf("SCALAR MULTYPLY COMPLEX TEST\t\t");
    struct complex res_comp = {0,0};
    if( scalar_multy_vectors(&v1, &v2, &res_comp, mult_complex_num) ){

        if( abs(res_comp.x - scalar_multyply_v1_v2_complex.x) < Eps
         && abs(res_comp.y - scalar_multyply_v1_v2_complex.y) < Eps ) {
            printf("Test passed\n");
            good_tests++;
        }else{
            printf("Test falled\n");
            printf("got:      ");
            print_complex_num(&res_comp);
            printf("\nExpected: ");
            print_complex_num(&scalar_multyply_v1_v2_complex);
        }
    }else{
        printf("Test falled\n");
        printf("Bad vectors sizes: %d and %d", v4.n, v3.n);
    }
    count_tests++;
//==================================================BAD SCALAR MULTY VECTORS FLOAT TEST
    printf("BAD SCALAR MULTY VECTORS COMPLEX TEST\t");//v1 + v3;

    if(! scalar_multy_vectors(&v1, &v3, &res, mult_complex_num) ){
        printf("Test passed\n");
        good_tests++;
    }else{
        printf("Test falled\n");
        printf("got      : v1.n == v3.n == %d\nExpected: bad input\n", v4.n);
    }
    count_tests++;
    printf("\n****************************************************\n");
    if(count_tests == good_tests)
        printf("\tso good!!!!\n");
    else
        printf("falled %d tests \n", count_tests - good_tests);
    printf("\n\n");

is_test = false;
}

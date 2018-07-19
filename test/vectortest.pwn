#include <vector>

// Required by assert() and assertf()
#pragma tabsize 0

#define assert%2(%0,%1)%3; \
if (!(%0)) \
{ \
    print(%1); \
    return -1; \
}

#define assertf%3(%0,%1,%2)%4; \
if (!(%0)) \
{ \
    printf(%1,%2); \
    return -1; \
}

// Print vector
PrintVector(Vector:vector)
{
    new value[128], value_size;
    printf("Vector 0x%x:", _:vector);
    VECTOR_foreach_ex(v, index : vector)
    {
        value_size = MEM_get_size(v);
        MEM_UM_zero(UnmanagedPointer:MEM_UM_get_addr(value[0]), sizeof value);
        MEM_get_arr(v, _, value, (value_size < sizeof value) ? value_size : sizeof value);
        printf("\t%d: 0x%x, %d, \"%s\"", index, _:v, value_size, value);
    }
}

// Print vectors in vector
PrintVectorsInVector(Vector:vector)
{
    new value[128], value_size, Vector:v;
    printf("Vector 0x%x:", _:vector);
    VECTOR_foreach_ex(vx, index : vector)
    {
        value_size = MEM_get_size(vx);
        if (value_size == 1)
        {
            v = Vector:MEM_get_val(vx);
            if (v != VECTOR_NULL)
            {
                if (MEM_get_size(Pointer:v) == VECTOR_struct)
                {
                    printf("\t%d: Vector 0x%x:", index, _:v);
                    VECTOR_foreach(vy : v)
                    {
                        value_size = MEM_get_size(vy);
                        MEM_UM_zero(UnmanagedPointer:MEM_UM_get_addr(value[0]), sizeof value);
                        MEM_get_arr(vy, _, value, (value_size < sizeof value) ? value_size : sizeof value);
                        printf("\t\t0x%x, %d, \"%s\"", _:vy, value_size, value);
                    }
                }
                else
                {
                    MEM_UM_zero(UnmanagedPointer:MEM_UM_get_addr(value[0]), sizeof value);
                    MEM_get_arr(vx, _, value, (value_size < sizeof value) ? value_size : sizeof value);
                    printf("\tcase 1 => %d: 0x%x, %d, \"%s\"", index, _:vx, value_size, value);
                }
            }
            else
            {
                MEM_UM_zero(UnmanagedPointer:MEM_UM_get_addr(value[0]), sizeof value);
                MEM_get_arr(vx, _, value, (value_size < sizeof value) ? value_size : sizeof value);
                printf("\tcase 2 => %d: 0x%x, %d, \"%s\"", index, _:vx, value_size, value);
            }
        }
        else
        {
            MEM_UM_zero(UnmanagedPointer:MEM_UM_get_addr(value[0]), sizeof value);
            MEM_get_arr(vx, _, value, (value_size < sizeof value) ? value_size : sizeof value);
            printf("\t0x%x, %d, \"%s\"", _:vx, value_size, value);
        }
    }
}

// Entry point
main()
{
    new Vector:test_vector = VECTOR_NULL, Vector:test_vector_a = VECTOR_NULL, Vector:test_vector_b = VECTOR_NULL, sz, pos, str[7];
    print("\r\n=====================");
    print("= Vector unit test  =");
    print("=    Made by BigETI =");
    print("= Loaded!           =");
    print("=====================\r\n");
    print("\r\n[VECTORTEST] Test 1");
    PrintVector(test_vector);
    sz = VECTOR_size(test_vector);
    assertf(sz == 0, "Invalid size (%d, expected 0) : Test #1", sz);
    print("\r\n[VECTORTEST] Test 2");
    VECTOR_push_back_str(test_vector, "ab");
    VECTOR_push_back_str(test_vector, "cdef");
    VECTOR_push_back_str(test_vector, "ghijkl");
    VECTOR_push_back_str(test_vector, "mn");
    VECTOR_push_back_str(test_vector, "opqs");
    VECTOR_push_back_str(test_vector, "tuvwxy");
    PrintVector(test_vector);
    sz = VECTOR_size(test_vector);
    assertf(sz == 6, "Invalid size (%d, expected 6) : Test #2", sz);
    print("\r\n[VECTORTEST] Test 3");
    VECTOR_push_back_str(test_vector, "axy");
    VECTOR_push_back_str(test_vector, "opz");
    PrintVector(test_vector);
    sz = VECTOR_size(test_vector);
    assertf(sz == 8, "Invalid size (%d, expected 8) : Test #3", sz);
    print("\r\n[VECTORTEST] Test 4");
    VECTOR_remove_str(test_vector, "tuvwxy");
    VECTOR_remove_str(test_vector, "aaa");
    PrintVector(test_vector);
    sz = VECTOR_size(test_vector);
    assertf(sz == 7, "Invalid size (%d, expected 7) : Test #4", sz);
    print("\r\n[VECTORTEST] Test 5");
    pos = VECTOR_find_str(test_vector, "ghijkl");
    assertf(pos == 2, "Invalid index for \"ghijkl\" (%d, expected 2) : Test #5", pos);
    print("\r\n[VECTORTEST] Test 6");
    VECTOR_clear(test_vector);
    PrintVector(test_vector);
    sz = VECTOR_size(test_vector);
    assertf(sz == 0, "Invalid size (%d, expected 0) : Test #6", sz);
    print("\r\n[VECTORTEST] Test 7");
    VECTOR_resize(test_vector_a, 3);
    VECTOR_resize(test_vector_b, 3);
    VECTOR_set_str(test_vector_a, 0, "ab");
    VECTOR_set_str(test_vector_a, 1, "cdef");
    VECTOR_set_str(test_vector_a, 2, "ghijkl");
    VECTOR_set_str(test_vector_b, 0, "mn");
    VECTOR_set_str(test_vector_b, 1, "opqs");
    VECTOR_set_str(test_vector_b, 2, "tuvwxy");
    VECTOR_resize(test_vector, 2);
    VECTOR_set_val(test_vector, 0, _:test_vector_a);
    VECTOR_set_val(test_vector, 1, _:test_vector_b);
    PrintVectorsInVector(test_vector);
    VECTOR_get_arr(test_vector_a, 2, str);
    assertf(strcmp("ghijkl", str) == 0, "Invalid string \"%s\" (expected \"ghijkl\") : Test #7", str);
    VECTOR_get_arr(test_vector_b, 2, str);
    assertf(strcmp("tuvwxy", str) == 0, "Invalid string \"%s\" (expected \"tuvwxy\") : Test #7", str);
    sz = VECTOR_size(test_vector);
    assertf(sz == 2, "Invalid size (%d, expected 2) : Test #7", sz);
    sz = VECTOR_size(test_vector_a);
    assertf(sz == 3, "Invalid size (%d, expected 3) : Test #7", sz);
    sz = VECTOR_size(test_vector_b);
    assertf(sz == 3, "Invalid size (%d, expected 3) : Test #7", sz);
    print("\r\n[VECTORTEST] Test 8");
    VECTOR_clear(test_vector);
    VECTOR_clear(test_vector_a);
    VECTOR_clear(test_vector_b);
    sz = VECTOR_size(test_vector);
    assertf(sz == 0, "Invalid size (%d, expected 0) : Test #8", sz);
    sz = VECTOR_size(test_vector_a);
    assertf(sz == 0, "Invalid size (%d, expected 0) : Test #8", sz);
    sz = VECTOR_size(test_vector_b);
    assertf(sz == 0, "Invalid size (%d, expected 0) : Test #8", sz);
    print("[VECTORTEST] Test completed.");
    return 1;
}

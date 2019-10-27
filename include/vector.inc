#if defined __VECTOR_INCLUDED__
    #endinput
#endif
#define __VECTOR_INCLUDED__

#include <memory>
#include <string>

#define VECTOR_NULL                     (Vector:MEM_NULLPTR)
#define INVALID_VECTOR_INDEX            (-1)
#define VECTOR_foreach%2(%0:%1)         VECTOR_foreach_ex%2(%0,__vector__%0:%1)
#define VECTOR_foreach_ex%3(%0,%1:%2)   for%3(new Pointer:%0, %1; VECTOR_foreach_cmp(%2, %1, %0); %1++)
#define __vector__%0\32;                __vector__

// Vector structure
MEM_struct VECTOR_struct
{
    Pointer:VECTOR_struct_data,
    VECTOR_struct_size
}

// Vector push back
static stock VECTOR_push_back_ptr(&Vector:vector, Pointer:value_ptr)
{
    new v[VECTOR_struct], new_index;
    if (vector == VECTOR_NULL)
    {
        VECTOR_resize(vector, 1);
    }
    else
    {
        MEM_get_arr(Pointer:vector, _, v);
        new_index = v[VECTOR_struct_size];
        if (v[VECTOR_struct_size] >= MEM_get_size(v[VECTOR_struct_data]))
        {
            VECTOR_resize(vector, v[VECTOR_struct_size] * 2);
        }
    }
    MEM_get_arr(Pointer:vector, _, v);
    MEM_set_val(v[VECTOR_struct_data], new_index, _:value_ptr);
    MEM_set_val(Pointer:vector, VECTOR_struct_size, new_index + 1);
}

// Vector set pointer
static stock VECTOR_set_ptr(Vector:vector, index, Pointer:value_ptr)
{
    new v[VECTOR_struct];
    MEM_get_arr(Pointer:vector, _, v);
    if (index < v[VECTOR_struct_size])
    {
        new Pointer:val_ptr = Pointer:MEM_get_val(v[VECTOR_struct_data], index);
        MEM_set_val(v[VECTOR_struct_data], index, _:value_ptr);
        MEM_delete(val_ptr);
    }
}

// Vector compare
static stock bool:VECTOR_compare(Pointer:left, Pointer:right)
{
    new bool:ret = false, size = MEM_get_size(left);
    if (size == MEM_get_size(right))
    {
        ret = true;
        for (new i; i < size; i++)
        {
            if (MEM_get_val(left, i) != MEM_get_val(right, i))
            {
                ret = false;
                break;
            }
        }
    }
    return ret;
}

// Vector remove pointer
static stock VECTOR_remove_ptr(&Vector:vector, Pointer:value_ptr)
{
    new v[VECTOR_struct], new_sz, Pointer:data;
    MEM_get_arr(Pointer:vector, _, v);
    data = MEM_new(v[VECTOR_struct_size]);
    for (new i, sz = v[VECTOR_struct_size], Pointer:ptr; i < sz; i++)
    {
        ptr = Pointer:MEM_get_val(v[VECTOR_struct_data], i);
        if (VECTOR_compare(value_ptr, ptr))
        {
            MEM_delete(ptr);
        }
        else
        {
            MEM_set_val(data, new_sz, _:ptr);
            ++new_sz;
        }
    }
    MEM_delete(v[VECTOR_struct_data]);
    if (new_sz > 0)
    {
        v[VECTOR_struct_data] = data;
        v[VECTOR_struct_size] = new_sz;
        MEM_set_arr(Pointer:vector, _, v);
    }
    else
    {
        MEM_delete(Pointer:vector);
        vector = VECTOR_NULL;
    }
}

// Vector find pointer
static stock VECTOR_find_ptr(Vector:vector, Pointer:value_ptr, from_index)
{
    new ret = INVALID_VECTOR_INDEX, v[VECTOR_struct];
    MEM_get_arr(Pointer:vector, _, v);
    for (new i = from_index, sz = v[VECTOR_struct_size], Pointer:ptr; i < sz; i++)
    {
        ptr = Pointer:MEM_get_val(v[VECTOR_struct_data], i);
        if (VECTOR_compare(value_ptr, ptr))
        {
            ret = i;
            break;
        }
    }
    return ret;
}

// Vector push back value
stock VECTOR_push_back_val(&Vector:vector, value)
{
    new Pointer:value_ptr = MEM_new_val(value);
    if (value_ptr != MEM_NULLPTR)
    {
        VECTOR_push_back_ptr(vector, value_ptr);
    }
}

// Vector push back array
stock VECTOR_push_back_arr(&Vector:vector, const value[], value_len = sizeof value)
{
    if (value_len > 0)
    {
        new Pointer:value_ptr = MEM_new_arr(value, value_len);
        if (value_ptr != MEM_NULLPTR)
        {
            VECTOR_push_back_ptr(vector, value_ptr);
        }
    }
}

// Vector push back string
stock VECTOR_push_back_str(&Vector:vector, const value[])
{
    VECTOR_push_back_arr(vector, value, strlen(value) + 1);
}

// Vector get pointer
stock Pointer:VECTOR_get_ptr(Vector:vector, index)
{
    new Pointer:ret = MEM_NULLPTR;
    if ((vector != VECTOR_NULL) && (index >= 0))
    {
        new v[VECTOR_struct];
        MEM_get_arr(Pointer:vector, _, v);
        if (index < v[VECTOR_struct_size])
        {
            ret = Pointer:MEM_get_val(v[VECTOR_struct_data], index);
        }
    }
    return ret;
}

// Vector get value
stock VECTOR_get_val(Vector:vector, index)
{
    new ret, Pointer:value_ptr = VECTOR_get_ptr(vector, index);
    if (value_ptr != MEM_NULLPTR)
    {
        ret = MEM_get_val(value_ptr);
    }
    return ret;
}

// Vector get array
stock VECTOR_get_arr(Vector:vector, index, value[], value_len = sizeof value)
{
    if (value_len > 0)
    {
        new Pointer:value_ptr = VECTOR_get_ptr(vector, index);
        if (value_ptr != MEM_NULLPTR)
        {
            MEM_get_arr(value_ptr, _, value, value_len);
        }
    }
}

// Vector get string
stock VECTOR_get_str(Vector:vector, index, value[], value_len = sizeof value)
{
    if (value_len > 0)
    {
        new Pointer:value_ptr = VECTOR_get_ptr(vector, index);
        if (value_ptr != MEM_NULLPTR)
        {
            new len = MEM_get_size(value_ptr);
            MEM_get_arr(value_ptr, _, value, (len < value_len) ? len : value_len);
            value[value_len - 1] = '\0';
        }
    }
}

// Vector set value
stock VECTOR_set_val(Vector:vector, index, value)
{
    if ((vector != VECTOR_NULL) && (index >= 0))
    {
        VECTOR_set_ptr(vector, index, MEM_new_val(value));
    }
}

// Vector set array
stock VECTOR_set_arr(Vector:vector, index, const value[], value_len = sizeof value)
{
    if ((vector != VECTOR_NULL) && (index >= 0) && (value_len > 0))
    {
        VECTOR_set_ptr(vector, index, MEM_new_arr(value, value_len));
    }
}

// Vector set string
stock VECTOR_set_str(Vector:vector, index, const value[])
{
    VECTOR_set_arr(vector, index, value, strlen(value) + 1);
}

// Vector size
stock VECTOR_size(Vector:vector)
{
    return ((vector == VECTOR_NULL) ? 0 : MEM_get_val(Pointer:vector, VECTOR_struct_size));
}

// Vector size
stock VECTOR_alloc_size(Vector:vector)
{
    return ((vector == VECTOR_NULL) ? 0 : MEM_get_size(Pointer:MEM_get_val(Pointer:vector, VECTOR_struct_data)));
}

// Vector resize
stock VECTOR_resize(&Vector:vector, size)
{
    if (size == 0)
    {
        VECTOR_clear(vector);
    }
    else if (size > 0)
    {
        new v[VECTOR_struct];
        if (vector == VECTOR_NULL)
        {
            v[VECTOR_struct_data] = MEM_new(size);
            v[VECTOR_struct_size] = size;
            for (new i; i < size; i++)
            {
                MEM_set_val(v[VECTOR_struct_data], i, _:MEM_new_val(0));
            }
            vector = Vector:MEM_new_arr(v);
        }
        else
        {
            MEM_get_arr(Pointer:vector, _, v);
            if (v[VECTOR_struct_size] != size)
            {
                new Pointer:data = MEM_new(size);
                MEM_copy(data, v[VECTOR_struct_data], (v[VECTOR_struct_size] < size) ? v[VECTOR_struct_size] : size);
                for (new i = size, sz = v[VECTOR_struct_size]; i < sz; i++)
                {
                    MEM_delete(Pointer:MEM_get_val(v[VECTOR_struct_data], i));
                }
                for (new i = v[VECTOR_struct_size]; i < size; i++)
                {
                    MEM_set_val(data, i, _:MEM_new_val(0));
                }
                v[VECTOR_struct_size] = size;
                MEM_delete(v[VECTOR_struct_data]);
                v[VECTOR_struct_data] = data;
                MEM_set_arr(Pointer:vector, _, v);
            }
        }
    }
}

// Vector clear
stock VECTOR_clear(&Vector:vector)
{
    if (vector != VECTOR_NULL)
    {
        new v[VECTOR_struct];
        MEM_get_arr(Pointer:vector, _, v);
        for (new i, sz = v[VECTOR_struct_size]; i < sz; i++)
        {
            MEM_delete(Pointer:MEM_get_val(v[VECTOR_struct_data], i));
        }
        MEM_delete(v[VECTOR_struct_data]);
        MEM_delete(Pointer:vector);
        vector = VECTOR_NULL;
    }
}

// Vector remove value
stock VECTOR_remove_val(&Vector:vector, value)
{
    if (vector != VECTOR_NULL)
    {
        new Pointer:ptr = MEM_new_val(value);
        VECTOR_remove_ptr(vector, ptr);
        MEM_delete(ptr);
    }
}

// Vector remove array
stock VECTOR_remove_arr(&Vector:vector, const value[], value_len = sizeof value)
{
    if ((vector != VECTOR_NULL) && (value_len > 0))
    {
        new Pointer:ptr = MEM_new_arr(value, value_len);
        VECTOR_remove_ptr(vector, ptr);
        MEM_delete(ptr);
    }
}

// Vector remove string
stock VECTOR_remove_str(&Vector:vector, const value[])
{
    VECTOR_remove_arr(vector, value, strlen(value) + 1);
}

// Vector find value
stock VECTOR_find_val(Vector:vector, value, from_index = 0)
{
    new ret = INVALID_VECTOR_INDEX;
    if ((vector != VECTOR_NULL) && (from_index >= 0))
    {
        new Pointer:ptr = MEM_new_val(value);
        ret = VECTOR_find_ptr(vector, ptr, from_index);
        MEM_delete(ptr);
    }
    return ret;
}

// Vector find array
stock VECTOR_find_arr(Vector:vector, const value[], value_len = sizeof value, from_index = 0)
{
    new ret = INVALID_VECTOR_INDEX;
    if ((vector != VECTOR_NULL) && (value_len > 0) && (from_index >= 0))
    {
        new Pointer:ptr = MEM_new_arr(value, value_len);
        ret = VECTOR_find_ptr(vector, ptr, from_index);
        MEM_delete(ptr);
    }
    return ret;
}

// Vector find string
stock VECTOR_find_str(Vector:vector, const value[], from_index = 0)
{
    return VECTOR_find_arr(vector, value, strlen(value) + 1, from_index);
}

// Vector foreach compare
stock bool:VECTOR_foreach_cmp(Vector:vector, index, &Pointer:ptr)
{
    ptr = VECTOR_get_ptr(vector, index);
    return (ptr != MEM_NULLPTR);
}

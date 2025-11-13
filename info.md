## Desired API

```c++
// create a context to capture types into
Context context;

struct Position 
{
    float x, y, z;
}

reflex::capture(context)
    .type()
       

// or

struct [[reflex]] Color
{
    uint8_t r, g, b, a;
}

// [[reflex]] attribute generates the registration code if using the toolchain


// we can then fetch type information as such
auto t = reflex::get<Position>(context);
auto t = reflex::get(context, "Position");

for (const auto& field: t.fields()) { 
    log(field.name); 
}

for (const auto& func: t.functions()) { 
    log(func.name); 
}


```
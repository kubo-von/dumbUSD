#ifndef STAGE_STRUCT_HPP
#define STAGE_STRUCT_HPP

#include <optional>
#include <string>
#include <vector>
#include "rust/cxx.h"

struct AttrStruct {
    rust::String attr_name;
    rust::String attr_type;
    rust::Vec<bool> attr_bool;
    rust::Vec<int> attr_int;
    rust::Vec<float> attr_float;
    rust::Vec<rust::String> attr_string;
};

struct PrimStruct {
    rust::String path;
    rust::String prim_type;
    rust::Vec<AttrStruct> attributes;
};

struct StageStruct {
    rust::Vec<PrimStruct> prims;
};


StageStruct load_file(rust::String file_path);

#endif // MY_STRUCT_HPP

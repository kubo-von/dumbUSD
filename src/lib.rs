#[cxx::bridge]
pub mod ffi {

    pub struct AttrStruct {
        pub attr_name: String,
        pub attr_type: String,
        pub attr_bool: Vec<bool>,
        pub attr_int: Vec<i32>,
        pub attr_float: Vec<f32>,
        pub attr_string: Vec<String>,
    }

    pub struct PrimStruct {
        pub path: String,
        pub prim_type: String,
        pub attributes: Vec<AttrStruct>,
    }

    pub struct StageStruct {
        pub prims: Vec<PrimStruct>,
    }

    unsafe extern "C++" {
        include!("stage_struct.hpp");
        type AttrStruct;
        type PrimStruct;
        type StageStruct;
        pub fn load_file(file_path: String) -> StageStruct;
    }
}

pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
}

use dumbUSD::*;

fn main() {
    println!("running example");

    unsafe {
        let file_path = "/Users/jakubvondra/dev/lumik-renderer/src/assets/usd/toy.usd".to_string();

        let stage = dumbUSD::ffi::load_file(file_path);
        println!("len {:?}", stage.prims.len());
        for prim in stage.prims.iter() {
            println!("{:?} : {:?} ", prim.prim_type, prim.path);
            for attr in &prim.attributes {
                if !attr.attr_bool.is_empty() {
                    println!(
                        "  attr: {:?} : {:?} bool value: {:?} ",
                        attr.attr_type,
                        attr.attr_name,
                        attr.attr_bool[0..10.min(attr.attr_bool.len())].to_vec()
                    )
                };

                if !attr.attr_int.is_empty() {
                    println!(
                        "  attr: {:?} : {:?} int value: {:?} ",
                        attr.attr_type,
                        attr.attr_name,
                        attr.attr_int[0..10.min(attr.attr_int.len())].to_vec()
                    )
                };

                if !attr.attr_float.is_empty() {
                    println!(
                        "  attr: {:?} : {:?} float value: {:?} ...",
                        attr.attr_type,
                        attr.attr_name,
                        attr.attr_float[0..10.min(attr.attr_float.len())].to_vec()
                    )
                };
            }
        }
    }
}
